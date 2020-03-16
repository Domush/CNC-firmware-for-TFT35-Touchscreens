#include "interfaceCmd.h"
#include "Printing.h"
#include "Home.h"
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon+label)
MENUITEMS M0PauseItems = {
    //title
    LABEL_M0_PAUSE,
    //icon                        label
    {
        {ICON_PAUSE, LABEL_PAUSE},
        {ICON_RESUME, LABEL_RESUME},
        {ICON_BACK, LABEL_BACK},
    }};

const ITEM itemM0Pause[2] = {
    // icon                       label
    {ICON_PAUSE, LABEL_PAUSE},
    {ICON_RESUME, LABEL_RESUME},
};

QUEUE gcodeCommand;   // Current gcode command

static u8 gcodeIndex = 0;
u8 curRouterSpeed;
extern PRINTING infoPrinting;

// *Is there a code character in the current gcode command.
static bool isGcodeCommand(char code) {
  for (gcodeIndex = 0; gcodeCommand.queue[gcodeCommand.readIndex].gcode[gcodeIndex] != 0 && gcodeIndex < GCODE_MAX_CHARACTERS; gcodeIndex++) {
    if (gcodeCommand.queue[gcodeCommand.readIndex].gcode[gcodeIndex] == code) {
      gcodeIndex += 1;
      return true;
    }
  }
  return false;
}

// *Get the int after M or G code, Call after isGcodeCommand('code').
static u32 getGcodeValue(void) {
  return (strtol(&gcodeCommand.queue[gcodeCommand.readIndex].gcode[gcodeIndex], NULL, 10));
}

// *Get the float after M or G code, Call after isGcodeCommand('code').
static float getGcodeValueFloat(void) {
  return (strtod(&gcodeCommand.queue[gcodeCommand.readIndex].gcode[gcodeIndex], NULL));
}

void addGcodeCommand(const char *gcodeString, uint8_t port) {
  while (gcodeCommand.count >= GCODE_QUEUE_MAX) {
    timedMessage(1, TIMED_ERROR, "Queue is full");
    runUpdateLoop();
  }
  strncpy(gcodeCommand.queue[gcodeCommand.writeIndex].gcode, gcodeString, GCODE_MAX_CHARACTERS - 1);
  gcodeCommand.queue[gcodeCommand.writeIndex].src = port;

  gcodeCommand.writeIndex = (gcodeCommand.writeIndex + 1) % GCODE_QUEUE_MAX;
  gcodeCommand.count++;
}

// *Add a Gcode command to the gcodeCommand queue.
// *Will be sent to UART in sendGcodeCommands()
// If the gcodeCommand queue is full, display a "waiting" banner.
bool storeCmd(const char *gcodeString, ...) {
  // *Queue each command individually
  const char ch[2] = "\n";
  char *token;
  char *gcodeLine;
  gcodeLine = "";
  // char buffer[GCODE_MAX_CHARACTERS] = "";
  my_va_list ap;
  my_va_start(ap, gcodeString);
  my_vsprintf(gcodeLine, gcodeString, ap);
  my_va_end(ap);

  // *get the first token
  token = strtok(gcodeLine, ch);
  // *walk through other tokens
  while (token != NULL) {
    addGcodeCommand(token, SERIAL_PORT);
    token = strtok(NULL, ch);
  }
  return true;
}

// Store from UART cmd(such as: ESP3D, OctoPrint, else TouchScreen) to gcodeCommand queue, this cmd will be sent by UART in sendGcodeCommands(),
// If the gcodeCommand queue is full, display warning and wait.
bool storeCmdFromUART(uint8_t port, const char *gcode) {
  while (gcodeCommand.count >= GCODE_QUEUE_MAX) {
    timedMessage(1, TIMED_ERROR, "Queue is full");
    runUpdateLoop();
  }
  addGcodeCommand(gcode, port);
  return true;
}

bool isQueueFull(void) {
  if (gcodeCommand.count >= GCODE_QUEUE_MAX) return true;
  return false;
}

// Clear all gcode cmd in gcodeCommand queue for abort printing.
void clearCmdQueue(void) {
  gcodeCommand.count = gcodeCommand.writeIndex = gcodeCommand.readIndex = 0;
}

/**
 * Parse and send gcode stored in gcodeCommand.
 *
 * @version	v1.0.0	Wednesday, February 26th, 2020.
 * @global
 * @param	mixed	void
 * @return	void
 */
void sendGcodeCommands(void) {
  if (infoPrinting.m0_pause == true || infoHost.waiting == true || gcodeCommand.count == 0) {
    return;
  }
  infoHost.waiting         = infoHost.connected;   // *Wait for an 'ok' or error response before sending next command (if connected)
  bool haltQueueProcessing = false;
  bool avoid_terminal      = false;
  u16 cmd                  = 0;
  // u8 curRouterSpeed;
  switch (gcodeCommand.queue[gcodeCommand.readIndex].gcode[0]) {
    case 'M':
      cmd = strtol(&gcodeCommand.queue[gcodeCommand.readIndex].gcode[1], NULL, 10);
      switch (cmd) {
        case 0:   // M0/1 Stop and wait for user.
        case 1:
          popup_message       = &gcodeCommand.queue[gcodeCommand.readIndex].gcode[3];
          haltQueueProcessing = true;
          break;   // *No need to do anything special - All functions are handled by the CNC

        case 3:    //M3 Set the spindle CW speed or laser power
        case 5:    //M5 Turn off spindle or laser
          break;   // *No need to do anything special - All functions are handled by the CNC

        case 18:   //M18 Disable steppers (same as M84).
        case 84:
          coordinateSetClear(false);
          break;

        case 24:   //M24 - Resume SD print
          setPrintPause(false);
          break;

        case 25:   //M25 - Pause SD print
          haltQueueProcessing = true;
          // setPrintPause(true);
          break;

        case 27:   //M27 - Report SD print status
          printSetUpdateWaiting(false);
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
          // // u8 i = 0;
          // if (isGcodeCommand('P')) i = getGcodeValue();
          // if (isGcodeCommand('S')) {
          //   routerSetSpeed(i, getGcodeValue());
          // } else {
          //   char buf[12];
          //   sprintf(buf, "S%d\n", routerGetSpeed(i));
          //   strcat(gcodeCommand.queue[gcodeCommand.readIndex].gcode, (const char *)buf);
          //   routerSetSendWaiting(i, false);
          // }
          break;   // *No need to do anything special - All functions are handled by the CNC

        case 107:   //M107 Turn off a fan/router
          // // u8 i = 0;
          // if (isGcodeCommand('P')) i = getGcodeValue();
          // routerSetSpeed(i, 0);
          break;   // *No need to do anything special - All functions are handled by the CNC

        case 114:   //M114 Report the current tool position to the host.
          break;

        case 117:   //M117 Set the message line on the LCD.
          popupReminder((u8 *)"M117 Notice", (u8 *)&gcodeCommand.queue[gcodeCommand.readIndex].gcode[5]);
          break;

        case 220:   //M220 Set the global CNC speed percentage.
          if (isGcodeCommand('S')) {
            setCNCSpeedOverride(getGcodeValue());
          } else {
            char buf[12];
            sprintf(buf, " S%d\n", getCNCSpeedOverride());
            strcat(gcodeCommand.queue[gcodeCommand.readIndex].gcode, (const char *)buf);
          }
          break;

        case 500:   //M500 Save EEPROM
        case 501:   //M501 Load EEPROM
        case 502:   //M502 Load default (firmware) settings
          infoHost.waiting = false;
          break;
      }
      break;

    case 'G':
      cmd = strtol(&gcodeCommand.queue[gcodeCommand.readIndex].gcode[1], NULL, 10);
      switch (cmd) {
        case 0:   //G0 Fast move
        case 1:   //G1 Cut move
        {
          AXIS axis;
          for (axis = X_AXIS; axis < TOTAL_AXIS; axis++) {
            if (isGcodeCommand(axis_id[axis])) {
              coordinateSetAxisTarget(axis, getGcodeValueFloat());
            }
          }
          if (isGcodeCommand('F')) {
            coordinateSetGantrySpeed(getGcodeValue());
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
          infoPrinting.coordSpace = (u8)cmd;
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
            if (isGcodeCommand(axis_id[axis])) {
              coordinateSetAxisTarget(axis, getGcodeValueFloat());
            }
          }
          // Restore mode
          coorSetRelative(coorRelative);
          break;
        }
      }
      break;

    case 'T':
      cmd = strtol(&gcodeCommand.queue[gcodeCommand.readIndex].gcode[1], NULL, 10);
      break;
  }

  setGcodeCommandSource(gcodeCommand.queue[gcodeCommand.readIndex].src);
  Serial_Puts(SERIAL_PORT, gcodeCommand.queue[gcodeCommand.readIndex].gcode);   // send the command to the CNC

  showGcodeStatus(gcodeCommand.queue[gcodeCommand.readIndex].gcode, TFT_SOURCE);   // display sent gcode in the status line
  if (avoid_terminal != true) {
    sendGcodeTerminalCache(gcodeCommand.queue[gcodeCommand.readIndex].gcode, TFT_SOURCE);
  }
  gcodeCommand.count--;
  gcodeCommand.readIndex = (gcodeCommand.readIndex + 1) % GCODE_QUEUE_MAX;

  if (haltQueueProcessing == true) {
    setPrintPause(true);
  }
  powerFailedEnable(true);
}

void menuM0Pause(void) {
  // extern char *popup_title;
  extern char *popup_message;
  u16 key_num = IDLE_TOUCH;
  Serial_Puts(SERIAL_PORT, "M108\n");   // Remove the M0 freeze from the CNC
  setPrintPause(true);
  drawXYZ();
  infoPrinting.m0_pause = true;
  // curRouterSpeed = infoPrinting.routerSpeed;
  // if (curRouterSpeed > 0) {
  //   routerControl(0);
  // }
  // Serial_Puts(SERIAL_PORT, "G0 Z%d\n", SPINDLE_PAUSE_Z_RAISE);

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
        // menuChangeBit();
        break;

      case KEY_POPUP_CANCEL:
        // routerControl(curRouterSpeed);
        infoPrinting.m0_pause = false;
        // setPrintPause(true);
        infoMenu.active--;
        break;
    }
    runUpdateLoop();
  }
}

void menuChangeBit(void) {
  extern PRINTING infoPrinting;
  char *confirmText;
  char *cancelText;
  confirmText = "Change bit";
  cancelText  = "Nevermind";
  if (infoPrinting.printing) {
    confirmText = "Change bit";
    cancelText  = "Keep going";
  }
  // extern char *popup_title;
  extern char *popup_message;
  if (strlen(popup_message) < 10) {
    if (infoPrinting.printing) {
      popup_message = "Do you want to change the bit?";
    } else {
      popup_message = "Go change the bit; I'll return to X0,Y0 when you're finished.";
    }
  }
  u16 key_num = IDLE_TOUCH;
  setPrintPause(true);
  Serial_Puts(SERIAL_PORT, "G53\n");
  Serial_Puts(SERIAL_PORT, "G0 X40 Y200 Z120 F%d\n", SPEED_MOVE_FAST);
  drawXYZ();
  // storeCmd("G53 G0 X20 Y200 Z100 F%d\n", SPEED_MOVE_FAST);
  // storeCmd("M0 Replace the bit and press Confirm when finished\n");

  popupDrawPage(bottomDoubleBtn, (u8 *)"Bit Change", (u8 *)popup_message, (u8 *)confirmText, (u8 *)cancelText);
  // popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_PAUSE), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while (infoMenu.menu[infoMenu.active] == menuChangeBit) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        infoPrinting.m0_pause = false;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        storeCmd("G0 X0 Y0\n");
        drawXYZ();
        infoMenu.active--;
        // infoMenu.menu[infoMenu.active] = menuMove;
        // menuMove();
        break;

      case KEY_POPUP_CANCEL:
        // routerControl(curRouterSpeed);
        infoPrinting.m0_pause = false;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        setPrintPause(false);
        drawXYZ();
        // storeCmd("G0 X0 Y0\n");
        infoMenu.active--;
        break;
    }
    runUpdateLoop();
  }
}
