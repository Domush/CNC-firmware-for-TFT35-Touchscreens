#include "interfaceCmd.h"
#include "includes.h"

QUEUE infoCmd;       //
QUEUE infoCacheCmd;  // Only when heatHasWaiting() is false the cmd in this cache will move to infoCmd queue.

static u8 cmd_index = 0;

// Is there a code character in the current gcode command.
static bool cmd_seen(char code) {
  for (cmd_index = 0; infoCmd.queue[infoCmd.index_r].gcode[cmd_index] != 0 && cmd_index < CMD_MAX_CHAR; cmd_index++) {
    if (infoCmd.queue[infoCmd.index_r].gcode[cmd_index] == code) {
      cmd_index += 1;
      return true;
    }
  }
  return false;
}

// Get the int after 'code', Call after cmd_seen('code').
static u32 cmd_value(void) {
  return (strtol(&infoCmd.queue[infoCmd.index_r].gcode[cmd_index], NULL, 10));
}

// Get the float after 'code', Call after cmd_seen('code').
static float cmd_float(void) {
  return (strtod(&infoCmd.queue[infoCmd.index_r].gcode[cmd_index], NULL));
}

// Store gcode cmd to infoCmd queue, this cmd will be sent by UART in sendQueueCmd(),
// If the infoCmd queue is full, reminde in title bar.
bool storeCmd(const char *format, ...) {
  QUEUE *pQueue = &infoCmd;

  if (pQueue->count >= CMD_MAX_LIST) {
    reminderMessage(LABEL_BUSY, STATUS_BUSY);
    return false;
  }

  my_va_list ap;
  my_va_start(ap, format);
  my_vsprintf(pQueue->queue[pQueue->index_w].gcode, format, ap);
  my_va_end(ap);
  pQueue->queue[pQueue->index_w].src = SERIAL_PORT;

  pQueue->index_w = (pQueue->index_w + 1) % CMD_MAX_LIST;
  pQueue->count++;

  return true;
}

// Store gcode cmd to infoCmd queue, this cmd will be sent by UART in sendQueueCmd(),
// If the infoCmd queue is full, reminde in title bar,  waiting for available queue and store the command.
void mustStoreCmd(const char *format, ...) {
  QUEUE *pQueue = &infoCmd;

  if (pQueue->count >= CMD_MAX_LIST) reminderMessage(LABEL_BUSY, STATUS_BUSY);

  while (pQueue->count >= CMD_MAX_LIST) {
    loopProcess();
  }

  my_va_list ap;
  my_va_start(ap, format);
  my_vsprintf(pQueue->queue[pQueue->index_w].gcode, format, ap);
  my_va_end(ap);
  pQueue->queue[pQueue->index_w].src = SERIAL_PORT;

  pQueue->index_w = (pQueue->index_w + 1) % CMD_MAX_LIST;
  pQueue->count++;
}

// Store from UART cmd(such as: ESP3D, OctoPrint, else TouchScreen) to infoCmd queue, this cmd will be sent by UART in sendQueueCmd(),
// If the infoCmd queue is full, reminde in title bar.
bool storeCmdFromUART(uint8_t port, const char *gcode) {
  QUEUE *pQueue = &infoCmd;

  if (pQueue->count >= CMD_MAX_LIST) {
    reminderMessage(LABEL_BUSY, STATUS_BUSY);
    return false;
  }

  strcpy(pQueue->queue[pQueue->index_w].gcode, gcode);

  pQueue->queue[pQueue->index_w].src = port;
  pQueue->index_w = (pQueue->index_w + 1) % CMD_MAX_LIST;
  pQueue->count++;

  return true;
}

// Store gcode cmd to infoCacheCmd queue, this cmd will be move to infoCmd in getGcodeFromFile() -> moveCacheToCmd(),
// this function is only for restore printing status after power failed.
void mustStoreCacheCmd(const char *format, ...) {
  QUEUE *pQueue = &infoCacheCmd;

  if (pQueue->count == CMD_MAX_LIST) reminderMessage(LABEL_BUSY, STATUS_BUSY);

  while (pQueue->count >= CMD_MAX_LIST) {
    loopProcess();
  }

  my_va_list ap;
  my_va_start(ap, format);
  my_vsprintf(pQueue->queue[pQueue->index_w].gcode, format, ap);
  my_va_end(ap);

  pQueue->index_w = (pQueue->index_w + 1) % CMD_MAX_LIST;
  pQueue->count++;
}

// Move gcode cmd from infoCacheCmd to infoCmd queue.
bool moveCacheToCmd(void) {
  if (infoCmd.count >= CMD_MAX_LIST) return false;
  if (infoCacheCmd.count == 0) return false;

  storeCmd("%s", infoCacheCmd.queue[infoCacheCmd.index_r].gcode);
  infoCacheCmd.count--;
  infoCacheCmd.index_r = (infoCacheCmd.index_r + 1) % CMD_MAX_LIST;
  return true;
}

// Clear all gcode cmd in infoCmd queue for abort printing.
void clearCmdQueue(void) {
  infoCmd.count = infoCmd.index_w = infoCmd.index_r = 0;
  infoCacheCmd.count = infoCacheCmd.index_w = infoCacheCmd.index_r = 0;
  heatSetUpdateWaiting(false);
}

// Parse and send gcode cmd in infoCmd.
void sendQueueCmd(void) {
  if (infoHost.wait == true) return;
  if (infoCmd.count == 0) return;

  bool avoid_terminal = false;
  u16 cmd = 0;
  switch (infoCmd.queue[infoCmd.index_r].gcode[0]) {
    case 'M':
      cmd = strtol(&infoCmd.queue[infoCmd.index_r].gcode[1], NULL, 10);
      switch (cmd) {
        case 0:  // M0/1 Stop and wait for user.
        case 1:
          if (isPrinting()) {
            setPrintPause(true, true);
          }
          break;

        case 3:  //M3 Set the spindle CW speed or laser power
          if (cmd_seen('S')) {
            routerSetSpeed(0, cmd_value());
          } else {
            char buf[12];
            sprintf(buf, "S%d\n", routerGetSpeed(0));
            strcat(infoCmd.queue[infoCmd.index_r].gcode, (const char *)buf);
            routerSetSendWaiting(0, false);
          }
          break;

        case 5:  //M5 Turn off spindle or laser
          routerSetSpeed(0, 0);
          break;

        case 18:  //M18 Disable steppers (same as M84).
        case 84:
          coordinateSetClear(false);
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

        case 82:  //M82 Set E to absolute positioning.
          eSetRelative(false);
          break;

        case 83:  //M83 Set E to relative positioning.
          eSetRelative(true);
          break;

        case 104:  //M104 Set a new target hot end temperature.
          TOOL i = heatGetCurrentToolSpindle();
          if (cmd_seen('T')) i = (TOOL)(cmd_value() + SPINDLE0);
          if (cmd_seen('S')) {
            heatSyncTargetTemp(i, cmd_value());
          } else {
            char buf[12];
            sprintf(buf, "S%d\n", heatGetTargetTemp(i));
            strcat(infoCmd.queue[infoCmd.index_r].gcode, (const char *)buf);
            heatSetSendWaiting(i, false);
          }
          break;

        case 105:  //M105 Send a temperature report to the host.
          break;   // *disabled for CNC
          heatSetUpdateWaiting(false);
          avoid_terminal = infoSettings.terminalACK;
          break;

        case 106:  //M106 Turn on the fan/router and set its speed
          u8 i = 0;
          if (cmd_seen('P')) i = cmd_value();
          if (cmd_seen('S')) {
            routerSetSpeed(i, cmd_value());
          } else {
            char buf[12];
            sprintf(buf, "S%d\n", routerGetSpeed(i));
            strcat(infoCmd.queue[infoCmd.index_r].gcode, (const char *)buf);
            routerSetSendWaiting(i, false);
          }
          break;

        case 107:  //M107 Turn off a fan/router
          u8 i = 0;
          if (cmd_seen('P')) i = cmd_value();
          routerSetSpeed(i, 0);
          break;

        case 109:  //M109 Wait for the hot end to reach its target.
          TOOL i = heatGetCurrentToolSpindle();
          if (cmd_seen('T')) i = (TOOL)(cmd_value() + SPINDLE0);
          infoCmd.queue[infoCmd.index_r].gcode[3] = '4';
          heatSetIsWaiting(i, true);
          break;

        case 114:  //M114 Report the current tool position to the host.
#ifdef FIL_RUNOUT_PIN
          positionSetUpdateWaiting(false);
#endif
          break;

        case 117:  //M117 Set the message line on the LCD.
          popupReminder((u8 *)"M117", (u8 *)&infoCmd.queue[infoCmd.index_r].gcode[5]);
          break;

        case 140:  //M140 Set a new target bed temperature.
          if (cmd_seen('S')) {
            heatSyncTargetTemp(BED, cmd_value());
          } else {
            char buf[12];
            sprintf(buf, "S%d\n", heatGetTargetTemp(BED));
            strcat(infoCmd.queue[infoCmd.index_r].gcode, (const char *)buf);
            heatSetSendWaiting(BED, false);
          }
          break;

        case 190:  //M190 Wait for the bed to reach target temperature.
          infoCmd.queue[infoCmd.index_r].gcode[2] = '4';
          heatSetIsWaiting(BED, true);
          break;

        case 220:  //M220 Set the global feedrate percentage.
          if (cmd_seen('S')) {
            speedSetPercent(0, cmd_value());
          } else {
            char buf[12];
            sprintf(buf, "S%d\n", speedGetPercent(0));
            strcat(infoCmd.queue[infoCmd.index_r].gcode, (const char *)buf);
            speedSetSendWaiting(0, false);
          }
          break;

        case 221:  //M221 Set the flow percentage, which applies to all E moves.
          if (cmd_seen('S')) {
            speedSetPercent(1, cmd_value());
          } else {
            char buf[12];
            sprintf(buf, "S%d\n", speedGetPercent(1));
            strcat(infoCmd.queue[infoCmd.index_r].gcode, (const char *)buf);
            speedSetSendWaiting(1, false);
          }
          break;
      }
      break;

    case 'G':
      cmd = strtol(&infoCmd.queue[infoCmd.index_r].gcode[1], NULL, 10);
      switch (cmd) {
        case 0:  //G0 Fast move
        case 1:  //G1 Cut move
          AXIS i;
          for (i = X_AXIS; i < TOTAL_AXIS; i++) {
            if (cmd_seen(axis_id[i])) {
              coordinateSetAxisTarget(i, cmd_float());
            }
          }
          if (cmd_seen('F')) {
            coordinateSetFeedRate(cmd_value());
          }
          break;

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
          AXIS i;
          bool coorRelative = coorGetRelative();
          bool eRelative = eGetRelative();
          // Set to absolute mode
          coorSetRelative(false);
          eSetRelative(false);
          for (i = X_AXIS; i < TOTAL_AXIS; i++) {
            if (cmd_seen(axis_id[i])) {
              coordinateSetAxisTarget(i, cmd_float());
            }
          }
          // Restore mode
          coorSetRelative(coorRelative);
          eSetRelative(eRelative);
          break;
      }
      break;

    case 'T':
      cmd = strtol(&infoCmd.queue[infoCmd.index_r].gcode[1], NULL, 10);
      heatSetCurrentToolSpindle((TOOL)(cmd + SPINDLE0));
      break;
  }

  // Show sent g-code on the status line
  // char sent_gcode[60] = "Sent: ";
  setCurrentAckSrc(infoCmd.queue[infoCmd.index_r].src);
  Serial_Puts(SERIAL_PORT, infoCmd.queue[infoCmd.index_r].gcode);  //
  // strcat(sent_gcode, (const char *)infoCmd.queue[infoCmd.index_r].gcode);

  /*
  char ignored_commands[] = {"M105", "M118", "M114", "M117"};  // G-code commands which won't be displayed on the 'Sent:' screen
  bool show_gcode = true;
  for (u8 i = 0; i < COUNT(ignored_commands); i++) {
    if (strstr(infoCmd.queue[infoCmd.index_r].gcode, ignored_commands[i])) {
      show_gcode = false;
      break;
    }
  }

  if (show_gcode && strlen(infoCmd.queue[infoCmd.index_r].gcode) > 1) {
    GUI_ClearRect(0, BYTE_HEIGHT * 2, (LCD_WIDTH / 3) * 2, BYTE_HEIGHT * 3);
    GUI_DispLenString(0, BYTE_HEIGHT * 2, (u8 *)sent_gcode, (LCD_WIDTH / 3) * 2);  // Display sent g-code in status
  }
  //  end show gcode
 */
  if (avoid_terminal != true) {
    // showGcodeStatus(infoCmd.queue[infoCmd.index_r].gcode, TERMINAL_GCODE);
    sendGcodeTerminalCache(infoCmd.queue[infoCmd.index_r].gcode, TERMINAL_GCODE);
  }
  infoCmd.count--;
  infoCmd.index_r = (infoCmd.index_r + 1) % CMD_MAX_LIST;

  infoHost.wait = infoHost.connected;  //

  powerFailedEnable(true);
}
