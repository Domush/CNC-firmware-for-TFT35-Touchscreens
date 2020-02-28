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

QUEUE gcodeCommand;       // Current gcode command
QUEUE gcodeCommandQueue;  // Pending gcode commands

static u8 gcodeIndex = 0;
u8 curRouterSpeed;

// Is there a code character in the current gcode command.
static bool
cmd_seen(char code) {
  for (gcodeIndex = 0; gcodeCommand.queue[gcodeCommand.index_r].gcode[gcodeIndex] != 0 && gcodeIndex < GCODE_MAX_CHARACTERS; gcodeIndex++) {
    if (gcodeCommand.queue[gcodeCommand.index_r].gcode[gcodeIndex] == code) {
      gcodeIndex += 1;
      return true;
    }
  }
  return false;
}

// Get the int after 'code', Call after cmd_seen('code').
static u32 cmd_value(void) {
  return (strtol(&gcodeCommand.queue[gcodeCommand.index_r].gcode[gcodeIndex], NULL, 10));
}

// Get the float after 'code', Call after cmd_seen('code').
static float cmd_float(void) {
  return (strtod(&gcodeCommand.queue[gcodeCommand.index_r].gcode[gcodeIndex], NULL));
}

// Store gcode cmd to gcodeCommand queue, this cmd will be sent by UART in sendQueueCmd(),
// If the gcodeCommand queue is full, reminde in title bar.
bool storeCmd(const char *format, ...) {
  QUEUE *pQueue = &gcodeCommand;

  if (pQueue->count >= GCODE_QUEUE_MAX) {
    reminderMessage(LABEL_BUSY, STATUS_BUSY);
    return false;
  }

  my_va_list ap;
  my_va_start(ap, format);
  my_vsprintf(pQueue->queue[pQueue->index_w].gcode, format, ap);
  my_va_end(ap);
  pQueue->queue[pQueue->index_w].src = SERIAL_PORT;

  pQueue->index_w = (pQueue->index_w + 1) % GCODE_QUEUE_MAX;
  pQueue->count++;

  return true;
}

// Store gcode cmd to gcodeCommand queue, this cmd will be sent by UART in sendQueueCmd(),
// If the gcodeCommand queue is full, display alert in title bar: Waiting for available queue and store the command.
void mustStoreCmd(const char *format, ...) {
  QUEUE *pQueue = &gcodeCommand;

  if (pQueue->count >= GCODE_QUEUE_MAX) reminderMessage(LABEL_BUSY, STATUS_BUSY);

  while (pQueue->count >= GCODE_QUEUE_MAX) {
    loopProcess();
  }

  my_va_list ap;
  my_va_start(ap, format);
  my_vsprintf(pQueue->queue[pQueue->index_w].gcode, format, ap);
  my_va_end(ap);
  pQueue->queue[pQueue->index_w].src = SERIAL_PORT;

  pQueue->index_w = (pQueue->index_w + 1) % GCODE_QUEUE_MAX;
  pQueue->count++;
}

// Store from UART cmd(such as: ESP3D, OctoPrint, else TouchScreen) to gcodeCommand queue, this cmd will be sent by UART in sendQueueCmd(),
// If the gcodeCommand queue is full, reminde in title bar.
bool storeCmdFromUART(uint8_t port, const char *gcode) {
  QUEUE *pQueue = &gcodeCommand;

  if (pQueue->count >= GCODE_QUEUE_MAX) {
    reminderMessage(LABEL_BUSY, STATUS_BUSY);
    return false;
  }

  strcpy(pQueue->queue[pQueue->index_w].gcode, gcode);

  pQueue->queue[pQueue->index_w].src = port;
  pQueue->index_w = (pQueue->index_w + 1) % GCODE_QUEUE_MAX;
  pQueue->count++;

  return true;
}

// Store gcode cmd to gcodeCommandQueue queue, this cmd will be move to gcodeCommand in getGcodeFromFile() -> moveCacheToCmd(),
// this function is only for restore printing status after power failed.
void mustStoreCacheCmd(const char *format, ...) {
  QUEUE *pQueue = &gcodeCommandQueue;

  if (pQueue->count == GCODE_QUEUE_MAX) reminderMessage(LABEL_BUSY, STATUS_BUSY);

  while (pQueue->count >= GCODE_QUEUE_MAX) {
    loopProcess();
  }

  my_va_list ap;
  my_va_start(ap, format);
  my_vsprintf(pQueue->queue[pQueue->index_w].gcode, format, ap);
  my_va_end(ap);

  pQueue->index_w = (pQueue->index_w + 1) % GCODE_QUEUE_MAX;
  pQueue->count++;
}

// Move gcode cmd from gcodeCommandQueue to gcodeCommand queue.
bool moveCacheToCmd(void) {
  if (gcodeCommand.count >= GCODE_QUEUE_MAX) return false;
  if (gcodeCommandQueue.count == 0) return false;

  storeCmd("%s", gcodeCommandQueue.queue[gcodeCommandQueue.index_r].gcode);
  gcodeCommandQueue.count--;
  gcodeCommandQueue.index_r = (gcodeCommandQueue.index_r + 1) % GCODE_QUEUE_MAX;
  return true;
}

// Clear all gcode cmd in gcodeCommand queue for abort printing.
void clearCmdQueue(void) {
  gcodeCommand.count = gcodeCommand.index_w = gcodeCommand.index_r = 0;
  gcodeCommandQueue.count = gcodeCommandQueue.index_w = gcodeCommandQueue.index_r = 0;
}

/**
 * Parse and send gcode stored in gcodeCommand.
 *
 * @version	v1.0.0	Wednesday, February 26th, 2020.
 * @global
 * @param	mixed	void
 * @return	void
 */
void sendQueueCmd(void) {
  if (infoPrinting.m0_pause == true) return;
  //if (infoHost.waiting == true) return;
  if (gcodeCommand.count == 0) return;

  bool avoid_terminal = false;
  u16 cmd = 0;
  // u8 curRouterSpeed;
  switch (gcodeCommand.queue[gcodeCommand.index_r].gcode[0]) {
    case 'M':
      cmd = strtol(&gcodeCommand.queue[gcodeCommand.index_r].gcode[1], NULL, 10);
      switch (cmd) {
        case 0:  // M0/1 Stop and wait for user.
        case 1:
          break;  // *No need to do anything special - All functions are handled by the CNC

        case 3:   //M3 Set the spindle CW speed or laser power
        case 5:   //M5 Turn off spindle or laser
          break;  // *No need to do anything special - All functions are handled by the CNC

        case 18:  //M18 Disable steppers (same as M84).
        case 84:
          coordinateSetClear(false);
          break;

        case 24:  //M24 - Resume SD print
          setPrintPause(false);
          break;

        case 25:  //M25 - Pause SD print
          setPrintPause(true);
          break;

        case 27:  //M27 - Report SD print status
          printSetUpdateWaiting(false);
          break;

        case 80:  //M80 Turn on the power supply
#ifdef PS_ON_PIN
          PS_ON_On();
#endif
          break;

        case 81:  //M81 Turn off the power supply.
#ifdef PS_ON_PIN
          PS_ON_Off();
#endif
          break;

        case 106:  //M106 Turn on a fan/router and set its speed
          // // u8 i = 0;
          // if (cmd_seen('P')) i = cmd_value();
          // if (cmd_seen('S')) {
          //   routerSetSpeed(i, cmd_value());
          // } else {
          //   char buf[12];
          //   sprintf(buf, "S%d\n", routerGetSpeed(i));
          //   strcat(gcodeCommand.queue[gcodeCommand.index_r].gcode, (const char *)buf);
          //   routerSetSendWaiting(i, false);
          // }
          break;  // *No need to do anything special - All functions are handled by the CNC

        case 107:  //M107 Turn off a fan/router
          // // u8 i = 0;
          // if (cmd_seen('P')) i = cmd_value();
          // routerSetSpeed(i, 0);
          break;  // *No need to do anything special - All functions are handled by the CNC

        case 114:  //M114 Report the current tool position to the host.
#ifdef FIL_RUNOUT_PIN
          positionSetUpdateWaiting(false);
#endif
          break;

        case 117:  //M117 Set the message line on the LCD.
          popupReminder((u8 *)"M117 Notice", (u8 *)&gcodeCommand.queue[gcodeCommand.index_r].gcode[5]);
          break;

        case 220:  //M220 Set the global CNC speed percentage.
          if (cmd_seen('S')) {
            setCNCSpeedOverride(cmd_value());
          } else {
            char buf[12];
            sprintf(buf, "S%d\n", getCNCSpeedOverride());
            strcat(gcodeCommand.queue[gcodeCommand.index_r].gcode, (const char *)buf);
          }
          break;
      }
      break;

    case 'G':
      cmd = strtol(&gcodeCommand.queue[gcodeCommand.index_r].gcode[1], NULL, 10);
      switch (cmd) {
        case 0:  //G0 Fast move
        case 1:  //G1 Cut move
        {
          AXIS axis;
          for (axis = X_AXIS; axis < TOTAL_AXIS; axis++) {
            if (cmd_seen(axis_id[axis])) {
              coordinateSetAxisTarget(axis, cmd_float());
            }
          }
          if (cmd_seen('F')) {
            coordinateSetGantrySpeed(cmd_value());
          }
          break;
        }

        case 28:  //G28 Auto home one or more axes.
          coordinateSetClear(true);
          break;

        case 90:  //G90 Absolute Positioning
          coorSetRelative(false);
          break;

        case 91:  //G91 Relative Positioning
          coorSetRelative(true);
          break;

        case 92:  //G92 Set the current position of one or more axes.
        {
          AXIS axis;
          bool coorRelative = coorGetRelative();
          bool eRelative = eGetRelative();
          // Set to absolute mode
          coorSetRelative(false);
          eSetRelative(false);
          for (axis = X_AXIS; axis < TOTAL_AXIS; axis++) {
            if (cmd_seen(axis_id[axis])) {
              coordinateSetAxisTarget(axis, cmd_float());
            }
          }
          // Restore mode
          coorSetRelative(coorRelative);
          eSetRelative(eRelative);
          break;
        }
      }
      break;

    case 'T':
      cmd = strtol(&gcodeCommand.queue[gcodeCommand.index_r].gcode[1], NULL, 10);
      break;
  }

  setCurrentAckSrc(gcodeCommand.queue[gcodeCommand.index_r].src);
  Serial_Puts(SERIAL_PORT, gcodeCommand.queue[gcodeCommand.index_r].gcode);  // send the command to the CNC

  showGcodeStatus(gcodeCommand.queue[gcodeCommand.index_r].gcode, TERMINAL_GCODE);  // display sent gcode in the status line
  if (avoid_terminal != true) {
    sendGcodeTerminalCache(gcodeCommand.queue[gcodeCommand.index_r].gcode, TERMINAL_GCODE);
  }
  gcodeCommand.count--;
  gcodeCommand.index_r = (gcodeCommand.index_r + 1) % GCODE_QUEUE_MAX;

  infoHost.waiting = infoHost.connected;  //

  powerFailedEnable(true);
}

void menuM0Pause(void) {
  extern PRINTING infoPrinting;
  if (infoPrinting.coordSpace < 53) infoPrinting.coordSpace = 53;
  // extern char *popup_title;
  extern char *popup_message;
  // void menuM0Pause(void) {
  u16 key_num = IDLE_TOUCH;
  Serial_Puts(SERIAL_PORT, "M108\n");  // Remove the M0 freeze from the CNC
  setPrintPause(true);
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
    loopProcess();
  }
}

void menuChangeBit(void) {
  extern PRINTING infoPrinting;
  if (infoPrinting.coordSpace < 53) infoPrinting.coordSpace = 53;
  // extern char *popup_title;
  extern char *popup_message;
  if (strlen(popup_message) < 5) {
    popup_message = "Change the bit; continue when finished. -- All actions are disabled until complete.";
  }
  u16 key_num = IDLE_TOUCH;
  setPrintPause(true);
  Serial_Puts(SERIAL_PORT, "G59\n");
  Serial_Puts(SERIAL_PORT, "G0 X20 Y200 Z160 F%d\n", SPEED_MOVE_FAST);
  // mustStoreCmd("G53 G0 X20 Y200 Z100 F%d\n", SPEED_MOVE_FAST);
  // mustStoreCmd("M0 Replace the bit and press Confirm when finished\n");

  popupDrawPage(bottomDoubleBtn, (u8 *)"Bit Change", (u8 *)popup_message, (u8 *)"Move Gantry", (u8 *)"Resume");
  // popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_PAUSE), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while (infoMenu.menu[infoMenu.active] == menuChangeBit) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        infoPrinting.m0_pause = false;
        mustStoreCmd("G%d\n", infoPrinting.coordSpace);
        mustStoreCmd("G0 X0 Y0\n");
        infoMenu.menu[infoMenu.active] = menuMove;
        // menuMove();
        break;

      case KEY_POPUP_CANCEL:
        // routerControl(curRouterSpeed);
        infoPrinting.m0_pause = false;
        mustStoreCmd("G%d\n", infoPrinting.coordSpace);
        setPrintPause(false);
        // mustStoreCmd("G0 X0 Y0\n");
        infoMenu.active--;
        break;
    }
    loopProcess();
  }
}
