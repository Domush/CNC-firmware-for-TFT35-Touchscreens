#include "gcodeSender.h"
#include "includes.h"

// LCD init functions
#include "GUI.h"


// Chip specific includes
#include "Serial.h"
#include "usart.h"


// UI handling
#include "touch_process.h"
#include "ledcolor.h"
#include "emulationToggle.h"

// File handling
#include "list_item.h"

// Gcode processing
#include "Gcode/gcodeResponseHandler.h"

// Base API functions
#include "API/coordinate.h"
#include "API/gantry.h"
#include "API/extendedFeatures.h"

// Menus
#include "includesMenus.h" // All menu headers

// //1 title, ITEM_PER_PAGE items(icon+label)
// MENUITEMS M0PauseItems = {
//     //title
//     LABEL_M0_PAUSE,
//     //icon                        label
//     {
//         {ICON_PAUSE, LABEL_PAUSE},
//         {ICON_RESUME, LABEL_RESUME},
//         {ICON_BACK, LABEL_BACK}}};

// const ITEM itemM0Pause[2] = {
//     // icon                       label
//     {ICON_PAUSE, LABEL_PAUSE},
//     {ICON_RESUME, LABEL_RESUME}};

// Current gcode line for processing
static char gcodeLine[GCODE_MAX_CHARACTERS];
static u8 gcodeLineIndex = 0;

// u8 curRouterSpeed;
// extern JOBSTATUS infoJobStatus;

// *Check if gcodeLine contains string, then record the location using gcodeLineIndex
static bool gcodeContains(u8 *string, u8 beginAtChar) {
  char *match;
  match = strstr(&gcodeLine[beginAtChar], (char *)string);
  if (match) {
    gcodeLineIndex = strlen(gcodeLine) - strlen(match) + strlen((char *)string);
    if (gcodeLine[gcodeLineIndex] == ' ') {
      gcodeLineIndex++;
    }
    return true;
  } else {
    return false;
  }
}

// // *Check if gcodeLine is *exactly* string
// static bool gcodeIsExactly(u8 *string) {
//   if ((char *)gcodeLine == (char *)string) {
//     return true;
//   } else {
//     return false;
//   }
// }

// // *Check if gcodeLine begins with string
// static bool gcodeBeginsWith(u8 *string) {
//   if (strstr(gcodeLine, (char *)string) == gcodeLine) {
//     return true;
//   } else {
//     return false;
//   }
// }

// *Get the float value from gcodeLine at the current gcodeLineIndex
static float gcodeValueFloat(void) {
  return strtof(&gcodeLine[gcodeLineIndex], NULL);
}

// *Get the integer value from gcodeLine at the current gcodeLineIndex
static u16 gcodeValue(void) {
  return strtol(&gcodeLine[gcodeLineIndex], NULL, 10);
}

// *Add a Gcode command to the gcodeOutgoing queue.
// If the gcodeOutgoing queue is full, display a "waiting" banner.
bool addGcodeCommand(bool skipIfQueueFull, const char *gcodeString, uint8_t port) {
  if (gcodeOutgoing.count >= GCODE_QUEUE_SIZE) {
    if (skipIfQueueFull) {
      return false;
    }
    while (gcodeOutgoing.count >= GCODE_QUEUE_SIZE) {
      timedMessage(1, TIMED_ERROR, "Queue is full");
      runUpdateLoop();
    }
  }
  strncpy(gcodeOutgoing.queue[gcodeOutgoing.queueIndex].gcode, gcodeString, GCODE_MAX_CHARACTERS - 1);
  gcodeOutgoing.queue[gcodeOutgoing.queueIndex].src = port;
  gcodeOutgoing.queueIndex                          = (gcodeOutgoing.queueIndex + 1) % GCODE_QUEUE_SIZE;
  gcodeOutgoing.count++;
  return true;
}

// *Add a Gcode command to the gcodeOutgoing queue.
// *Will be sent to UART in parseGcodeOutgoing()
// If the gcodeOutgoing queue is full, display a "waiting" banner.
bool queueCommand(bool skipIfQueueFull, char *gcodeString, ...) {
  // *Process formatted text (if applicable)
  char gcodeCommandSet[1024];
  gcodeCommandSet[0] = 0;
  my_va_list ap;
  my_va_start(ap, gcodeString);
  my_vsprintf(gcodeCommandSet, gcodeString, ap);
  my_va_end(ap);

  //* If there are multiple commands, queue each command individually
  char gcodeCommand[1024];
  gcodeCommand[0] = 0;
  int lineIndex   = 0;
  for (u8 setIndex = 0; gcodeCommandSet[setIndex] != 0; setIndex++) {
    gcodeCommand[lineIndex] = gcodeCommandSet[setIndex];
    if (gcodeCommand[lineIndex] == '\n' || gcodeCommandSet[setIndex + 1] == 0) {
      if (gcodeCommand[lineIndex] == '\n') {
        gcodeCommand[lineIndex] = 0;   // End character
      } else {
        gcodeCommand[lineIndex + 1] = 0;
      }
      while (gcodeCommand[strlen(gcodeCommand) - 1] == ' ') {
        //* Remove trailing spaces
        gcodeCommand[strlen(gcodeCommand) - 1] = 0;   // Remove last character
      }
      if (strlen(gcodeCommand) > 1) {
        if (!addGcodeCommand(skipIfQueueFull, gcodeCommand, SERIAL_PORT)) {
          return false;
        }
      }
      lineIndex = -1;   // Reset lineIndex for next command
    } else if (lineIndex == 0 && gcodeCommand[lineIndex] == ' ') {
      //* Ignore leading spaces
      lineIndex = -1;   // Reset lineIndex to ignore leading space
    }
    lineIndex++;
  }
  return true;
}

// Store from UART cmd(such as: ESP3D, OctoPrint, else TouchScreen) to gcodeOutgoing queue, this cmd will be sent by UART in sendGcodeCommands(),
// If the gcodeOutgoing queue is full, display warning and wait.
void queueCommandFromSerial(uint8_t port, const char *gcode) {
  while (gcodeOutgoing.count >= GCODE_QUEUE_SIZE) {
    timedMessage(1, TIMED_ERROR, "Queue is full");
    runUpdateLoop();
  }
  addGcodeCommand(false, gcode, port);
}

// *Check if the gcodeOutgoing queue is full
bool isQueueFull(void) {
  if (gcodeOutgoing.count >= GCODE_QUEUE_SIZE) return true;
  return false;
}

//* Clear all gcode cmd in gcodeOutgoing queue for abort printing.
void clearGcodeQueue(void) {
  gcodeOutgoing.count = gcodeOutgoing.queueIndex = 0;
}

//* Parse and send gcode stored in gcodeOutgoing.
void parseGcodeOutgoing(void) {
  if (infoHost.waitForResponse == true || gcodeOutgoing.count == 0) {
    return;
  }
  // *Wait for an 'ok' or error response before sending next command (only if connected)
  infoHost.waitForResponse = infoHost.connected;
  bool skipCommand         = false;
  u16 cmd                  = 0;
  u8 activeQueueIndex      = (gcodeOutgoing.queueIndex - (gcodeOutgoing.count)) % GCODE_QUEUE_SIZE;
  strcpy(gcodeLine, gcodeOutgoing.queue[activeQueueIndex].gcode);
  gcodeLineIndex = 0;
  switch (gcodeLine[0]) {
    case 'M':
      cmd = strtol(&gcodeLine[1], NULL, 10);
      switch (cmd) {
        case 0:   // M0/1 Stop and wait for user.
        case 1:
          popup_message                    = &gcodeLine[3];
          infoMenu.menu[++infoMenu.active] = menuM0Pause;
          infoJobStatus.isM0Paused         = true;
          setPrintPause(true);
          skipCommand = true;   //* Don't sent the command
          break;

        case 3:    //M3 Set the spindle CW speed or laser power
        case 5:    //M5 Turn off spindle or laser
          break;   // *No need to do anything special - All functions are handled by the CNC

        case 18:   //M18 Disable steppers (same as M84).
        case 84:
          coordinateSetClear(false);
          break;

        case 24:   //M24 - Resume SD print
          setPrintPause(false);
          skipCommand = true;   //* Don't sent the command
          break;

        case 25:   //M25 - Pause SD print
          setPrintPause(true);
          skipCommand = true;   //* Don't sent the command
          break;

        case 27:   //M27 - Report SD print status
          jobNeedUpdate(true);
          break;

        case 80:   //M80 Turn on the power supply
#ifdef PS_ON_PIN
          PS_ON_On();
#endif
          break;

        case 81:   //M81 Turn off the power supply.
#ifdef PS_ON_PIN
          PS_ON_Off();
#endif
          break;

        case 106:   //M106 Turn on a fan/router and set its speed
          break;    // *No need to do anything special - All functions are handled by the CNC

        case 107:   //M107 Turn off a fan/router
          break;    // *No need to do anything special - All functions are handled by the CNC

        case 114:   //M114 Report the current tool position to the host.
          break;

        case 117:   //M117 Set the message line on the LCD.
          popupReminder((u8 *)"M117 Notice", (u8 *)&gcodeLine[5]);
          break;

        case 220:   //M220 Set the global CNC speed percentage.
          if (gcodeContains((u8 *)'S', 4)) {
            setCNCSpeedOverride(gcodeValue());
          } else {
            char buf[12];
            sprintf(buf, " S%d\n", getCNCSpeedOverride());
            strcat(gcodeLine, (const char *)buf);
          }
          break;

        case 500:   //M500 Save EEPROM
        case 501:   //M501 Load EEPROM
        case 502:   //M502 Load default (firmware) settings
          infoHost.waitForResponse = false;
          break;
      }
      break;

    case 'G':
      cmd = strtol(&gcodeLine[1], NULL, 10);
      switch (cmd) {
        case 0:   //G0 Fast move
        case 1:   //G1 Cut move
        {
          AXIS axis;
          for (axis = X_AXIS; axis < TOTAL_AXIS; axis++) {
            if (gcodeContains((u8 *)&axis_id[axis], gcodeLineIndex)) {
              coordinateSetAxisTarget(axis, gcodeValueFloat());
            }
          }
          if (gcodeContains((u8 *)'F', 3)) {
            coordinateSetGantrySpeed(gcodeValue());
          }
          break;
        }

        case 28:   //G28 Auto home one or more axes.
          coordinateSetClear(true);
          break;

        case 53:   //G53-59 Switch to CNC coordinate set.
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
          infoJobStatus.coordSpace = (u8)cmd;
          break;

        case 90:   //G90 Absolute Positioning
          coorSetRelative(false);
          break;

        case 91:   //G91 Relative Positioning
          coorSetRelative(true);
          break;

        case 92:   //G92 Set the current position of one or more axes.
        {
          AXIS axis;
          bool coorRelative = coorGetRelative();
          // Set to absolute mode
          coorSetRelative(false);
          for (axis = X_AXIS; axis < TOTAL_AXIS; axis++) {
            if (gcodeContains((u8 *)&axis_id[axis], gcodeLineIndex)) {
              coordinateSetAxisTarget(axis, gcodeValueFloat());
            }
          }
          // Restore mode
          coorSetRelative(coorRelative);
          break;
        }
      }
      break;

    case 'T':
      // cmd = strtol(&gcodeLine[1], NULL, 10);
      skipCommand = true;
      break;
  }
  if (!skipCommand) {
    setGcodeCommandSource(gcodeOutgoing.queue[activeQueueIndex].src);
    sendCommand(SERIAL_PORT, gcodeLine);   // Send the command to the CNC
  }
  showGcodeStatus(gcodeLine, TFT_SOURCE);   // Display sent gcode on the status line
  if (infoSettings.showResponseInTerminal) {
    showInTerminal(gcodeLine, TFT_SOURCE);   // Display sent gcode on the terminal screen
  }
  gcodeOutgoing.count--;
  // powerFailedEnable(true);
}

void menuM0Pause(void) {
  // extern char *popup_message;
  u16 key_num = IDLE_TOUCH;
  drawXYZ();
  infoJobStatus.isM0Paused = true;

  // *Break it up into useful sections
  const char ch[2] = "\n";
  char *token;
  char *prompt_title;
  char *prompt_text;
  char *prompt_button;
  prompt_title  = "Pause requested";
  prompt_text   = "No pause reason was given.";
  prompt_button = "Resume";
  // *get the first token
  token = strtok(popup_message, ch);
  // *walk through other tokens
  for (u8 i = 0; token != NULL; i++) {
    if (i == 0) {
      prompt_text = token;
    } else if (i == 2) {
      prompt_title = token + 22;
    } else if (i == 3) {
      prompt_button = token + 23;
      if (strlen(prompt_button) < 2)
        prompt_button = "Resume";
      break;
    }
    token = strtok(NULL, ch);
  }

  // char text[] = "Load V-Bit -  0.5\" Dia., then Pos@ 0:0:1mm\r\n//action:prompt_end\n//action:prompt_begin M0/1 Break Called\n//action:prompt_button Continue\n//action:prompt_show\n";
  // popupDrawPage(&bottomSingleBtn, (u8 *)prompt_title, (u8 *)prompt_text, (u8 *)prompt_button, NULL);
  popupDrawPage(bottomDoubleBtn, (u8 *)prompt_title, (u8 *)prompt_text, (u8 *)"Change bit", (u8 *)prompt_button);

  // popupDrawPage(bottomDoubleBtn, m0_title, m0_message, "Change bit", "Resume");

  while (infoMenu.menu[infoMenu.active] == menuM0Pause) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        infoMenu.menu[infoMenu.active] = menuChangeBit;
        break;

      case KEY_POPUP_CANCEL:
        infoJobStatus.isM0Paused = false;
        infoMenu.active--;
        break;
    }
    runUpdateLoop();
  }
}

void menuChangeBit(void) {
  char *confirmText;
  char *cancelText;
  confirmText = "Change bit";
  cancelText  = "Nevermind";
  if (infoJobStatus.inProgress) {
    confirmText = "Change bit";
    cancelText  = "Keep going";
  }
  // extern char *popup_title;
  // extern char *popup_message;
  if (strlen(popup_message) < 10) {
    if (infoJobStatus.inProgress) {
      popup_message = "Do you want to change the bit?";
    } else {
      popup_message = "Go change the bit; I'll return to X0,Y0 when you're finished.";
    }
  }
  u16 key_num = IDLE_TOUCH;
  setPrintPause(true);
  popupDrawPage(bottomDoubleBtn, (u8 *)"Bit Change", (u8 *)popup_message, (u8 *)confirmText, (u8 *)cancelText);
  // popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_PAUSE), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while (infoMenu.menu[infoMenu.active] == menuChangeBit) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        infoJobStatus.isM0Paused = true;
        queueCommand(false, "G53\n");
        queueCommand(false, "G0 X40 Y200 Z120 F%d\n", SPEED_MOVE_FAST);
        drawXYZ();
        infoMenu.active--;
        infoMenu.menu[++infoMenu.active] = menuJobSetup;
        infoMenu.menu[++infoMenu.active] = menuMove;
        break;

      case KEY_POPUP_CANCEL:
        infoJobStatus.isM0Paused = false;
        queueCommand(false, "G%d\n", infoJobStatus.coordSpace);
        setPrintPause(false);
        drawXYZ();
        infoMenu.active--;
        break;
    }
    runUpdateLoop();
  }
}

void gcodeQueueStatus(void) {
  if (!infoHost.connected || lastGcodeQueueValue == gcodeOutgoing.count) {
    return;
  }
  if (gcodeOutgoing.count >= GCODE_QUEUE_SIZE) {
    timedMessage(3, TIMED_CRITICAL, "gCode queue is full!");
    queueTextColor = MAT_RED;
  } else if (gcodeOutgoing.count > GCODE_QUEUE_SIZE * .9) {
    timedMessage(2, TIMED_ERROR, "gCode queue almost full");
    queueTextColor = COLOR_MAROON;
  } else if (gcodeOutgoing.count > GCODE_QUEUE_SIZE * .7) {
    queueTextColor = MAT_ORANGE;
  } else if (gcodeOutgoing.count > GCODE_QUEUE_SIZE * .5) {
    queueTextColor = MAT_YELLOW;
  } else if (gcodeOutgoing.count > 1) {
    queueTextColor = MAT_GREEN;
  } else {
    queueTextColor = MAT_DARKGRAY;
  }
  GUI_SetColor(queueTextColor);
  GUI_FillCircle(BYTE_HEIGHT / 2, BYTE_HEIGHT / 2, BYTE_HEIGHT / 3);
  GUI_RestoreColorDefault();
}
