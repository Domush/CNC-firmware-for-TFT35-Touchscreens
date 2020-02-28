#include "parseACK.h"
#include "includes.h"

char dmaL2Cache[ACK_MAX_SIZE];
static u16 ack_index = 0;
static u8 ack_cur_src = SERIAL_PORT;
static u16 connectionRetryDelay = 2;  // # of seconds to wait before retrying to connect
static u16 connectionRetryTime = 0;   // stored timestamp for reconnect attempt
int MODEselect;

// Ignore reply "echo:" message (Don't display as a popup)
const char *const ignoreEcho[] = {
    "Now fresh file:",
    "Probe Z Offset:",
};

void setCurrentAckSrc(uint8_t src) {
  ack_cur_src = src;
}

static char responseMatch(const char *str) {
  u16 i;
  for (ack_index = 0; ack_index < ACK_MAX_SIZE && dmaL2Cache[ack_index] != 0; ack_index++) {
    for (i = 0; str[i] != 0 && dmaL2Cache[ack_index + i] != 0 && dmaL2Cache[ack_index + i] == str[i]; i++) {
    }
    if (str[i] == 0) {
      ack_index += i;
      return true;
    }
  }
  return false;
}

static char responseCompare(const char *str) {
  u16 i;
  for (i = 0; i < ACK_MAX_SIZE && str[i] != 0 && dmaL2Cache[i] != 0; i++) {
    if (str[i] != dmaL2Cache[i])
      return false;
  }
  if (i < ACK_MAX_SIZE && dmaL2Cache[i] != 0) return false;
  return true;
}

static float responseValue() {
  return (strtod(&dmaL2Cache[ack_index], NULL));
}
/*
// Read the value after the / if exists
static float ack_second_value() {
  char *secondValue = strchr(&dmaL2Cache[ack_index], '/');
  if (secondValue != NULL) {
    return (strtod(secondValue + 1, NULL));
  } else {
    return -0.5;
  }
}
 */
void ackPopupInfo(const char *info) {
  char *popup_title;
  char *popup_message;
  if (infoMenu.menu[infoMenu.active] == parametersetting) return;
  if (infoMenu.menu[infoMenu.active] == menuTerminal) return;
  if (strstr((char *)dmaL2Cache + ack_index, "//action:prompt_end")) {
    strcpy(popup_title, info);
    strcpy(popup_message, (const char *)dmaL2Cache + ack_index);
    if (infoMenu.menu[infoMenu.active] != menuM0Pause) {
      infoMenu.menu[++infoMenu.active] = menuM0Pause;
    }
    // menuM0Pause((char *)info, (char *)dmaL2Cache + ack_index);
  } else {
    popupReminder((u8 *)info, (u8 *)dmaL2Cache + ack_index);
  }
}

void syncL2CacheFromL1(uint8_t port) {
  uint16_t i = 0;
  for (i = 0; dmaL1Data[port].rIndex != dmaL1Data[port].wIndex; i++) {
    dmaL2Cache[i] = dmaL1Data[port].cache[dmaL1Data[port].rIndex];
    dmaL1Data[port].rIndex = (dmaL1Data[port].rIndex + 1) % DMA_TRANS_LEN;
  }
  dmaL2Cache[i] = 0;  // End character
}

void parseACK(void) {
  bool avoid_terminal = false;
  if (infoHost.rx_ok[SERIAL_PORT] != true) return;  //not get response data

  syncL2CacheFromL1(SERIAL_PORT);
  infoHost.rx_ok[SERIAL_PORT] = false;

  if (infoHost.connected == false)  //not connected to Marlin
  {
    if ((!responseMatch("T:") && !responseMatch("T0:")) || !responseMatch("ok")) {
      if (OS_GetTime() - connectionRetryDelay > connectionRetryTime) {
        connectionRetryTime = OS_GetTime();
        mustStoreCmd("M105\n");  // Attempts to get a "wake up" response to trigger a connection
      }
      goto parse_end;  //the first response should be such as "T:25/50 ok\n"
    }
    infoHost.connected = true;
#ifdef AUTO_SAVE_LOAD_LEVELING_VALUE
    storeCmd("M420 S1\n");
#endif
  }

  // GCode command response
  if (requestCommandInfo.inWaitResponse && responseMatch(requestCommandInfo.startMagic)) {
    requestCommandInfo.inResponse = true;
    requestCommandInfo.inWaitResponse = false;
  }
  if (requestCommandInfo.inResponse) {
    if (strlen(requestCommandInfo.response) + strlen(dmaL2Cache) < RESPONSE_MAX_CHARS) {
      strcat(requestCommandInfo.response, dmaL2Cache);

      if (responseMatch(requestCommandInfo.replyError)) {
        requestCommandInfo.done = true;
        requestCommandInfo.inResponse = false;
        requestCommandInfo.inError = true;
      } else if (responseMatch(requestCommandInfo.stopMagic)) {
        requestCommandInfo.done = true;
        requestCommandInfo.inResponse = false;
      }
    } else {
      requestCommandInfo.done = true;
      requestCommandInfo.inResponse = false;
      ackPopupInfo(replyError);
    }
    infoHost.waiting = false;
    goto parse_end;
  }
  // end

  if (responseCompare("ok\n")) {
    infoHost.waiting = false;
  } else {
    if (responseMatch("ok")) {
      infoHost.waiting = false;
    }
    if (responseMatch("X:")) {
      storegantry(0, responseValue());
      // storeCmd("M118 %d\n", responseValue());  //update X position
      if (responseMatch("Y:")) {
        storegantry(1, responseValue());
        // storeCmd("M118 %d\n", responseValue());  //update Y position
        if (responseMatch("Z:")) {
          // storeCmd("M118 %d\n", responseValue());  //update Z position
          storegantry(2, responseValue());
        }
      }
    } else if (responseMatch("Mean:")) {
      popupReminder((u8 *)"Repeatability Test", (u8 *)dmaL2Cache + ack_index - 5);
      //popupReminder((u8* )"Standard Deviation", (u8 *)&gcodeCommand.queue[gcodeCommand.index_r].gcode[5]);
    } else if (responseMatch("Probe Offset")) {
      if (responseMatch("Z")) {
        setCurrentOffset(responseValue());
      }
    } else if (responseMatch(replyEcho) && responseMatch(replyBusy) && responseMatch("processing")) {
      busyIndicator(STATUS_BUSY);
    } else if (responseMatch(replyEcho) && responseMatch(replyBusy) && responseMatch("paused for user")) {
      goto parse_end;
    } else if (responseMatch("X driver current: ")) {
      Get_parameter_value[0] = responseValue();

      if (responseMatch("Y driver current: "))
        Get_parameter_value[1] = responseValue();

      if (responseMatch("Z driver current: "))
        Get_parameter_value[2] = responseValue();

    } else if (responseMatch("M92 X")) {
      Get_parameter_value[3] = responseValue();

      if (responseMatch("Y"))
        Get_parameter_value[4] = responseValue();

      if (responseMatch("Z"))
        Get_parameter_value[5] = responseValue();

    }
#ifdef ONBOARD_SD_SUPPORT
    else if (responseMatch(replySDNotPrinting) && infoMenu.menu[infoMenu.active] == menuPrinting) {
      infoHost.printing = false;
      completePrinting();
    } else if (responseMatch(replySDPrinting)) {
      if (infoMenu.menu[infoMenu.active] != menuPrinting && !infoHost.printing) {
        infoMenu.menu[++infoMenu.active] = menuPrinting;
        infoHost.printing = true;
      }
      // Parsing printing data
      // Example: SD printing byte 123/12345
      char *ptr;
      u32 position = strtol(strstr(dmaL2Cache, "byte ") + 5, &ptr, 10);
      setPrintCur(position);
      //      powerFailedCache(position);
    }
#endif
    else if (responseMatch(replyError)) {
      ackPopupInfo(replyError);
    } else if (responseMatch(replyEcho)) {
      for (u8 i = 0; i < COUNT(ignoreEcho); i++) {
        if (strstr(dmaL2Cache, ignoreEcho[i])) {
          goto parse_end;
        }
      }
      ackPopupInfo(replyEcho);
    }
  }
  if (responseMatch(" F0:")) {
    routerControl(responseValue());
  }

parse_end:
  if (ack_cur_src != SERIAL_PORT) {
    Serial_Puts(ack_cur_src, dmaL2Cache);
  }
  showGcodeStatus(dmaL2Cache, TERMINAL_ACK);
  if (avoid_terminal != true) {
    sendGcodeTerminalCache(dmaL2Cache, TERMINAL_ACK);
  }
}

void parseRcvGcode(void) {
#ifdef SERIAL_PORT_2
  uint8_t i = 0;
  for (i = 0; i < _USART_CNT; i++) {
    if (i != SERIAL_PORT && infoHost.rx_ok[i] == true) {
      infoHost.rx_ok[i] = false;
      syncL2CacheFromL1(i);
      storeCmdFromUART(i, dmaL2Cache);
    }
  }
#endif
}
