#include "parseACK.h"
#include "includes.h"

char cncResponse[MAX_RESPONSE_SIZE];
RESPONSE_QUEUE gcodeResponse;   // Current gcode response
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
    "echo:LPC1768",
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
      // responseIndex += i;
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

void showPopupMessage(char *info) {
  const char ch[2] = "\n";
  char *token;
  popup_title   = info;
  popup_message = "No reason given. Continue when ready.";
  if (infoMenu.menu[infoMenu.active] == parametersetting) return;
  if (infoMenu.menu[infoMenu.active] == menuTerminal) return;
  /*
  "Load V-Bit -  0.5\" Dia., then Pos@ 0:0:1mm\r
  \n//action:prompt_end
  \n//action:prompt_begin M0/1 Break Called
  \n//action:prompt_button Continue
  \n//action:prompt_show
  \n"
  */
  if (strstr((const char *)cncResponse + responseIndex, "//action:prompt_end")) {
    if (strstr((const char *)cncResponse + responseIndex, "M0/1")) {
      setPrintPause(true);
      popup_message = &cncResponse[responseIndex];
      if (infoMenu.menu[infoMenu.active] != menuM0Pause) {
        infoMenu.menu[++infoMenu.active] = menuM0Pause;
      }
    } else {
      char *prompt_text = strstr((const char *)cncResponse + responseIndex, "//action:prompt_end") + 19;
      // *get the first token
      token         = strtok(prompt_text, ch);
      popup_message = token;
      popupReminder((u8 *)popup_title, (u8 *)popup_message);
    }
  } else if (strstr((const char *)cncResponse + responseIndex, "//action:notification")) {
    char *prompt_text = strstr((const char *)cncResponse + responseIndex, "//action:notification") + 21;
    // *get the first token
    token         = strtok(prompt_text, ch);
    popup_message = token;
    popupReminder((u8 *)popup_title, (u8 *)popup_message);
  } else if (strstr((const char *)cncResponse + responseIndex, "echo:")) {
    // *Break it up into useful sections
    char *prompt_text = strstr((const char *)cncResponse + responseIndex, "echo:") + 5;
    // *get the first token
    token         = strtok(prompt_text, ch);
    popup_message = token;

    popupReminder((u8 *)popup_title, (u8 *)popup_message);
  } else {
    popupReminder((u8 *)info, (u8 *)cncResponse + responseIndex);
  }
}

void copyIncomingToResponse(uint8_t port) {
  uint16_t i = 0;
  for (i = 0; cncIncoming[port].processedIndex != cncIncoming[port].pendingIndex; i++) {
    cncResponse[i]                   = cncIncoming[port].responseBuffer[cncIncoming[port].processedIndex];
    cncIncoming[port].processedIndex = (cncIncoming[port].processedIndex + 1) % MAX_RESPONSE_SIZE;
  }
  cncResponse[i] = 0;   // End character
}

void addGcodeResponse(const char *gcodeString, uint8_t port) {
  strncpy(gcodeResponse.queue[gcodeResponse.queueIndex].response, gcodeString, MAX_RESPONSE_SIZE - 1);
  gcodeResponse.queue[gcodeResponse.queueIndex].src = port;

  gcodeResponse.queueIndex = (gcodeResponse.queueIndex + 1) % RESPONSE_QUEUE_SIZE;
  if (gcodeResponse.count < RESPONSE_QUEUE_SIZE) gcodeResponse.count++;
  cncIncoming[port].processedIndex = (cncIncoming[port].processedIndex + strlen(gcodeString) + 1) % MAX_RESPONSE_SIZE;
}

// *Add a Gcode response to the gcodeResponse queue.
bool getNextResponse(uint8_t port) {
  // *Queue each response line individually
  const char ch[2] = "\n";
  char *token;
  // *get the first token
  token = strtok(&cncIncoming[port].responseBuffer[cncIncoming[port].processedIndex], ch);
  // *walk through other tokens
  // while (token != NULL) {
  addGcodeResponse(token, port);
  // token = strtok(NULL, ch);
  // }
  return true;
}

void parseGcodeResponse(void) {
  bool hideResponsesInTerminal = false;
  if (infoHost.responseReceived[SERIAL_PORT] != true) return;   // *Only process response data from the correct serial port

  // copyIncomingToResponse(SERIAL_PORT);
  getNextResponse(SERIAL_PORT);
  char *responseLine;
  responseLine                           = gcodeResponse.queue[gcodeResponse.queueIndex].response;
  infoHost.responseReceived[SERIAL_PORT] = false;   // *All response data has been moved to cncResponse

  // *Look for Marlin and wake it up if sleeping
  if (infoHost.connected == false) {
    timedMessage(1, TIMED_CRITICAL, (char *)textSelect(LABEL_UNCONNECTED));
    static u8 connectionRetryDelay = 2;   // # of seconds to wait before retrying to connect
    static u16 connectionRetryTime = 0;   // stored timestamp for reconnect attempt
    if (responseMatch("ok") || responseMatch("wait") || responseMatch("echo")) {
      // *Connection established! Let the Gcode Games begin!
      infoHost.connected = true;
    } else {
      if (OS_GetTime() - connectionRetryDelay > connectionRetryTime) {
        connectionRetryTime = OS_GetTime();
        storeCmd("G53\n");   // *Attempts to send a "wake up" packet to trigger a connection
      }
      goto parse_end;
    }
#ifdef AUTO_SAVE_LOAD_LEVELING_VALUE
    storeCmd("M420 S1\n");
#endif
  }

  // GCode command response
  if (requestCommandInfo.waitingForResponse || requestCommandInfo.responseInProgress) {
    if (responseMatch(requestCommandInfo.responseBegin)) {
      // *Found the response we wanted
      requestCommandInfo.responseInProgress = true;
      requestCommandInfo.waitingForResponse = false;
      if (strlen(requestCommandInfo.commandResponse) + strlen(responseLine) < RESPONSE_MAX_CHARS) {
        strcat(requestCommandInfo.commandResponse, responseLine);
        if (responseMatch(requestCommandInfo.responseEnd)) {
          requestCommandInfo.responseInProgress = false;
          requestCommandInfo.commandComplete    = true;
        }
      } else {
        requestCommandInfo.responseInProgress = false;
        showPopupMessage((char *)replyError);
      }
    } else if (responseMatch(requestCommandInfo.responseError)) {
      requestCommandInfo.commandComplete        = true;
      requestCommandInfo.responseInProgress     = false;
      requestCommandInfo.responseErrorTriggered = true;
    }
    infoHost.waiting = false;
    goto parse_end;
  }
  // end

  if (responseCompare("ok\n")) {
    infoHost.waiting = false;
  } else {
    // if (responseMatch("ok")) {
    //   infoHost.waiting = false;
    // }
    if (responseMatch("X:")) {
      storegantry(0, responseValue());
      if (responseMatch("Y:")) {
        storegantry(1, responseValue());
        if (responseMatch("Z:")) {
          storegantry(2, responseValue());
        }
      }

    } else if (responseMatch("Mean:")) {
      popupReminder((u8 *)"Repeatability Test", (u8 *)responseLine + responseIndex - 5);

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
        infoHost.printing                = true;
      }
      // Parsing printing data
      // Example: SD printing byte 123/12345
      char *ptr;
      u32 position = strtol(strstr(responseLine, "byte ") + 5, &ptr, 10);
      setPrintCur(position);
      //      powerFailedCache(position);
    }
#endif
    else if (responseMatch("//action:")) {
      showPopupMessage((char *)"Notice:");
    } else if (responseMatch("error:")) {
      showPopupMessage((char *)"Error detected:");
    } else if (responseMatch("echo:")) {
      // *Skip over useless echo notices
      for (u8 i = 0; i < COUNT(echoStringsToIgnore); i++) {
        if (strstr(responseLine, echoStringsToIgnore[i])) {
          goto parse_end;
        }
      }
      // *Show any echo notice that hasn't been skipped
      showPopupMessage((char *)"CNC message:");
    }
  }
  if (responseMatch(" F0:")) {
    routerControl(responseValue());
  }

parse_end:
  if (curGcodeSource != SERIAL_PORT) {
    Serial_Puts(curGcodeSource, responseLine);
  }
  showGcodeStatus(responseLine, CNC_SOURCE);
  if (hideResponsesInTerminal != true) {
    sendGcodeTerminalCache(responseLine, CNC_SOURCE);
  }
}

void parseSerialGcode(void) {
#ifdef SERIAL_PORT_2
  uint8_t i = 0;
  for (i = 0; i < _USART_CNT; i++) {
    if (i != SERIAL_PORT && infoHost.responseReceived[i] == true) {
      infoHost.responseReceived[i] = false;
      // copyIncomingToResponse(i);
      // storeCmdFromUART(i, cncResponse);
      getNextResponse(i);
      storeCmdFromUART(i, &gcodeResponse.queue[gcodeResponse.queueIndex].response[0]);
    }
  }
#endif
}
