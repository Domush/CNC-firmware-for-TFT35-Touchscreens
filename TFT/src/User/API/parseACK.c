#include "parseACK.h"
#include "includes.h"

char cncResponse[MAX_RESPONSE_SIZE];
static u16 responseIndex = 0;
static u8 curGcodeSource = SERIAL_PORT;
int MODEselect;
char *popup_title;
char *popup_message;

// *Ignore reply "echo:" messages containing these messages
// *(Don't display as a popup)
const char *const echoStringsToIgnore[] = {
    "Now fresh file:",
    "Probe Z Offset:",
};

void setGcodeCommandSource(uint8_t src) {
  curGcodeSource = src;
}

static char responseMatch(const char *str) {
  u16 i;
  for (responseIndex = 0; responseIndex < MAX_RESPONSE_SIZE && cncResponse[responseIndex] != 0; responseIndex++) {
    for (i = 0; str[i] != 0 && cncResponse[responseIndex + i] != 0 && cncResponse[responseIndex + i] == str[i]; i++) {
    }
    if (str[i] == 0) {
      responseIndex += i;
      return true;
    }
  }
  return false;
}

static char responseCompare(const char *str) {
  u16 i;
  for (i = 0; i < MAX_RESPONSE_SIZE && str[i] != 0 && cncResponse[i] != 0; i++) {
    if (str[i] != cncResponse[i])
      return false;
  }
  if (i < MAX_RESPONSE_SIZE && cncResponse[i] != 0) return false;
  return true;
}

static float responseValue() {
  return (strtod(&cncResponse[responseIndex], NULL));
}
/*
// Read the value after the / if exists
static float ack_second_value() {
  char *secondValue = strchr(&cncResponse[responseIndex], '/');
  if (secondValue != NULL) {
    return (strtod(secondValue + 1, NULL));
  } else {
    return -0.5;
  }
}
 */
void showPopupMessage(char *info) {
  popup_title = "Notice:";
  popup_message = "No reason given. Continue when ready.";
  if (infoMenu.menu[infoMenu.active] == parametersetting) return;
  if (infoMenu.menu[infoMenu.active] == menuTerminal) return;
  if (strstr((const char *)cncResponse + responseIndex, "//action:prompt_end")) {
    setPrintPause(true);
    popup_title = &info[0];
    popup_message = &cncResponse[responseIndex];
    if (infoMenu.menu[infoMenu.active] != menuM0Pause) {
      infoMenu.menu[++infoMenu.active] = menuM0Pause;
    }
  } else {
    popupReminder((u8 *)info, (u8 *)cncResponse + responseIndex);
  }
}

void copyIncomingToResponse(uint8_t port) {
  uint16_t i = 0;
  for (i = 0; cncIncoming[port].parsedIndex != cncIncoming[port].pendingIndex; i++) {
    cncResponse[i] = cncIncoming[port].cache[cncIncoming[port].parsedIndex];
    cncIncoming[port].parsedIndex = (cncIncoming[port].parsedIndex + 1) % MAX_RESPONSE_SIZE;
  }
  cncResponse[i] = 0;  // End character
}

void parseGcodeResponse(void) {
  bool hideResponsesInTerminal = false;
  if (infoHost.rx_ok[SERIAL_PORT] != true) return;  // *Only process response data from the correct serial port

  copyIncomingToResponse(SERIAL_PORT);
  infoHost.rx_ok[SERIAL_PORT] = false;  // *All response data has been moved to cncResponse

  // *Look for Marlin and wake it up if sleeping
  if (infoHost.connected == false) {
    timedMessage(1, TIMED_CRITICAL, (char *)textSelect(LABEL_UNCONNECTED));
    static u8 connectionRetryDelay = 2;  // # of seconds to wait before retrying to connect
    static u16 connectionRetryTime = 0;  // stored timestamp for reconnect attempt
    if ((!responseMatch("T:") && !responseMatch("T0:")) || !responseMatch("ok")) {
      if (OS_GetTime() - connectionRetryDelay > connectionRetryTime) {
        connectionRetryTime = OS_GetTime();
        mustStoreCmd("M105\n");  // *Attempts to send a "wake up" packet to trigger a connection
      }
      goto parse_end;  // *Initial Marlin response should be: "T:25/50 ok\n"
    }
    // *Connection established! Let the Gcode Games begin!
    infoHost.connected = true;
    // #ifdef AUTO_SAVE_LOAD_LEVELING_VALUE
    // storeCmd("M420 S1\n");
    // #endif
  }

  // GCode command response
  if (requestCommandInfo.waitingForResponse && responseMatch(requestCommandInfo.responseBegin)) {
    // *Found the response we wanted
    requestCommandInfo.responseInProgress = true;
    requestCommandInfo.waitingForResponse = false;
  }
  if (requestCommandInfo.responseInProgress) {
    if (strlen(requestCommandInfo.commandResponse) + strlen(cncResponse) < RESPONSE_MAX_CHARS) {
      strcat(requestCommandInfo.commandResponse, cncResponse);

      if (responseMatch(requestCommandInfo.responseError)) {
        requestCommandInfo.commandComplete = true;
        requestCommandInfo.responseInProgress = false;
        requestCommandInfo.responseErrorTriggered = true;
      } else if (responseMatch(requestCommandInfo.responseEnd)) {
        requestCommandInfo.commandComplete = true;
        requestCommandInfo.responseInProgress = false;
      }
    } else {
      requestCommandInfo.commandComplete = true;
      requestCommandInfo.responseInProgress = false;
      showPopupMessage((char *)replyError);
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
      if (responseMatch("Y:")) {
        storegantry(1, responseValue());
        if (responseMatch("Z:")) {
          storegantry(2, responseValue());
        }
      }

    } else if (responseMatch("Mean:")) {
      popupReminder((u8 *)"Repeatability Test", (u8 *)cncResponse + responseIndex - 5);

    } else if (responseMatch(replyEcho) && responseMatch(replyBusy) && responseMatch("processing")) {
      timedMessage(2, TIMED_WARNNG, (char *)textSelect(LABEL_BUSY));

    } else if (responseMatch(replyEcho) && responseMatch(replyBusy) && responseMatch("paused for user")) {
      timedMessage(2, TIMED_WARNNG, "CNC waiting for user");
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
      u32 position = strtol(strstr(cncResponse, "byte ") + 5, &ptr, 10);
      setPrintCur(position);
      //      powerFailedCache(position);
    }
#endif
    else if (responseMatch(replyError)) {
      showPopupMessage((char *)replyError);

    } else if (responseMatch(replyEcho)) {
      // *Skip over useless echo notices
      for (u8 i = 0; i < COUNT(echoStringsToIgnore); i++) {
        if (strstr(cncResponse, echoStringsToIgnore[i])) {
          goto parse_end;
        }
      }
      // *Show any echo notice that hasn't been skipped
      showPopupMessage((char *)replyEcho);
    }
  }
  if (responseMatch(" F0:")) {
    routerControl(responseValue());
  }

parse_end:
  if (curGcodeSource != SERIAL_PORT) {
    Serial_Puts(curGcodeSource, cncResponse);
  }
  showGcodeStatus(cncResponse, CNC_SOURCE);
  if (hideResponsesInTerminal != true) {
    sendGcodeTerminalCache(cncResponse, CNC_SOURCE);
  }
}

void parseSerialGcode(void) {
#ifdef SERIAL_PORT_2
  uint8_t i = 0;
  for (i = 0; i < _USART_CNT; i++) {
    if (i != SERIAL_PORT && infoHost.rx_ok[i] == true) {
      infoHost.rx_ok[i] = false;
      copyIncomingToResponse(i);
      storeCmdFromUART(i, cncResponse);
    }
  }
#endif
}
