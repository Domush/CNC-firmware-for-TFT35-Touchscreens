#include "parseACK.h"
#include "includes.h"

// RESPONSE_QUEUE gcodeResponse[3];   // Last gCode responses
static char responseLine[MAX_RESPONSE_SIZE];   // Current response being processed
static u8 responseLineIndex = 0;               // Char reference # of responseLine
static u8 curSerialPort     = SERIAL_PORT;     // Defaults to direct CNC serial

char *popup_title;     // Title of popup window
char *popup_message;   // Message text of popup window

// *Ignore reply "echo:" messages containing these messages
// *(Don't display as a popup)
const char *const echoStringsToIgnore[] = {
    "Now fresh file:",
    "Probe Z Offset:",
    "echo:LPC1768",
};

/**
 * parseGcodeResponse.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @global
 * @param	mixed	void
 * @return	void
 */
void parseGcodeResponse(void) {
  static u8 connectionRetryDelay = 2;   // # of seconds to wait before retrying to connect
  static u16 connectionRetryTime = 0;   // stored timestamp for reconnect attempt
  bool hideResponsesInTerminal   = false;
  // *Only process response data if data is waiting from SERIAL_PORT
  if (infoHost.responseReceived[SERIAL_PORT] != true) {
    // *Look for Marlin and wake it up if sleeping
    if (infoHost.connected == false) {
      timedMessage(1, TIMED_CRITICAL, (char *)textSelect(LABEL_UNCONNECTED));
      if (OS_GetTime() - connectionRetryDelay > connectionRetryTime) {
        connectionRetryTime = OS_GetTime();
        storeCmd("G53\n");   // *Attempts to send a "wake up" packet to trigger a connection
      }
      goto parse_end;
    } else {
      return;
    }
  }

  // copyIncomingToResponse(SERIAL_PORT);
  queueCncResponses(SERIAL_PORT);
  while (gcodeResponse[SERIAL_PORT].count > 0) {
    strcpy(&responseLine[0], gcodeResponse[SERIAL_PORT].queue[(gcodeResponse[SERIAL_PORT].queueIndex - gcodeResponse[SERIAL_PORT].count) % RESPONSE_QUEUE_SIZE].response);
    infoHost.responseReceived[SERIAL_PORT] = false;   // *All response data has been processed

    // *Look for Marlin and wake it up if sleeping
    if (infoHost.connected == false) {
      if (responseBeginsWith("ok") || responseBeginsWith("wait")) {
        // *Connection established! Let the Gcode Games begin!
        infoHost.connected = true;
        // Restore leveling, if enabled
#ifdef AUTO_SAVE_LOAD_LEVELING_VALUE
        storeCmd("M420 S1\n");
#endif
      }

      // GCode command response
      if (requestCommandInfo.waitingForResponse || requestCommandInfo.responseInProgress) {
        if (responseContains(requestCommandInfo.responseBegin)) {
          // *Found the response we wanted
          requestCommandInfo.responseInProgress = true;
          requestCommandInfo.waitingForResponse = false;
          if (strlen(requestCommandInfo.commandResponse) + strlen(responseLine) < RESPONSE_MAX_CHARS) {
            strcat(requestCommandInfo.commandResponse, responseLine);
            if (responseContains(requestCommandInfo.responseEnd)) {
              requestCommandInfo.responseInProgress = false;
              requestCommandInfo.commandComplete    = true;
            }
          } else {
            requestCommandInfo.responseInProgress = false;
            showPopupMessage((char *)replyError);
          }
        } else if (responseContains(requestCommandInfo.responseError)) {
          requestCommandInfo.commandComplete        = true;
          requestCommandInfo.responseInProgress     = false;
          requestCommandInfo.responseErrorTriggered = true;
        }
        infoHost.waiting = false;
        goto parse_end;
      }
      // end

      if (responseBeginsWith("ok")) {
        infoHost.waiting = false;
      } else {
        // non-"ok" response section
        if (responseContains("X:")) {
          storegantry(0, responseValue());
          if (responseContains("Y:")) {
            storegantry(1, responseValue());
            if (responseContains("Z:")) {
              storegantry(2, responseValue());
            }
          }
        } else if (responseContains("Mean:")) {
          popupReminder((u8 *)"Repeatability Test", (u8 *)responseLine + responseLineIndex - 5);
        } else if (responseContains(replyEcho) && responseContains(replyBusy) && responseContains("processing")) {
          timedMessage(2, TIMED_WARNNG, (char *)textSelect(LABEL_BUSY));
        } else if (responseContains(replyEcho) && responseContains(replyBusy) && responseContains("paused for user")) {
          timedMessage(2, TIMED_WARNNG, "CNC waiting for user");
          goto parse_end;
        } else if (responseContains("X driver current: ")) {
          Get_parameter_value[0] = responseValue();
          if (responseContains("Y driver current: "))
            Get_parameter_value[1] = responseValue();
          if (responseContains("Z driver current: "))
            Get_parameter_value[2] = responseValue();
        } else if (responseContains("M92 X")) {
          Get_parameter_value[3] = responseValue();
          if (responseContains("Y"))
            Get_parameter_value[4] = responseValue();
          if (responseContains("Z"))
            Get_parameter_value[5] = responseValue();
        }
#ifdef ONBOARD_SD_SUPPORT
        else if (responseContains(replySDNotPrinting) && infoMenu.menu[infoMenu.active] == menuPrinting) {
          infoHost.printing = false;
          completePrinting();

        } else if (responseContains(replySDPrinting)) {
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
        else if (responseContains("//action:")) {
          showPopupMessage((char *)"Notice:");
        } else if (responseContains("error:")) {
          showPopupMessage((char *)"Error detected:");
        } else if (responseContains("echo:")) {
          // *Skip over useless echo notices
          for (u8 i = 0; i < COUNT(echoStringsToIgnore); i++) {
            if (strstr(responseLine, echoStringsToIgnore[i])) {
              goto parse_end;
            }
          }
          // *Show any echo notice that hasn't been skipped
          showPopupMessage((char *)"CNC message:");
        }
      }   // end non-"ok" response section
      if (responseContains(" F0:")) {
        routerControl(responseValue());
      }

    parse_end:
      if (curSerialPort != SERIAL_PORT) {
        Serial_Puts(curSerialPort, responseLine);
      }
      showGcodeStatus(responseLine, CNC_SOURCE);
      if (hideResponsesInTerminal != true) {
        sendGcodeTerminalCache(responseLine, CNC_SOURCE);
      }
    }
  }
}

/**
 * parseSerialGcode.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @global
 * @param	mixed	void
 * @return	void
 */
void parseSerialGcode(void) {
#ifdef SERIAL_PORT_2
  uint8_t i = 0;
  for (i = 0; i < _USART_CNT; i++) {
    if (i != SERIAL_PORT && infoHost.responseReceived[i] == true) {
      infoHost.responseReceived[i] = false;
      // copyIncomingToResponse(i);
      // storeCmdFromUART(i, cncResponse);
      getNextResponse(i);
      storeCmdFromUART(i, &gcodeResponse[port].queue[gcodeResponse[port].queueIndex].response[0]);
    }
  }
#endif
}

/**
 * setGcodeCommandSource.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @global
 * @param	uint8_t	src
 * @return	void
 */
void setGcodeCommandSource(uint8_t src) {
  curSerialPort = src;
}

/**
 * responseContains.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @access	static
 * @global
 * @param	char	*string
 * @return	void
 */
static bool responseContains(const char *string) {
  char *match;
  match = strstr(responseLine, string);
  if (match) {
    responseLineIndex = strlen(responseLine) - strlen(match) + strlen(string);
    if (strchr(responseLine[responseLineIndex], " ")) {
      responseLineIndex++;
    }
    return true;
  } else {
    return false;
  }
}

/**
 * responseIsExactly.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @access	static
 * @global
 * @param	char	*string
 * @return	void
 */
static bool responseIsExactly(const char *string) {
  if (responseLine == string) {
    return true;
  } else {
    return false;
  }
}

/**
 * responseBeginsWith.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @access	static
 * @global
 * @param	char	*string
 * @return	void
 */
static bool responseBeginsWith(const char *string) {
  if (strstr(responseLine, string) == responseLine) {
    return true;
  } else {
    return false;
  }
}

/**
 * responseValue.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @access	static
 * @global
 * @return	mixed
 */
static float responseValue() {
  return (strtod(&responseLine[responseLineIndex], NULL));
}

/**
 * showPopupMessage.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @global
 * @param	char	*title
 * @return	void
 */
void showPopupMessage(char *title) {
  const char ch[2] = "\n";
  char *token;
  token       = 0;
  popup_title = title;
  if (!popup_message) {
    popup_message = "No reason given. Continue when ready.";
  }
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
  if (strstr((const char *)responseLine + responseLineIndex, "//action:prompt_end")) {
    if (strstr((const char *)responseLine + responseLineIndex, "M0/1")) {
      setPrintPause(true);
      popup_message = &responseLine[responseLineIndex];
      if (infoMenu.menu[infoMenu.active] != menuM0Pause) {
        infoMenu.menu[++infoMenu.active] = menuM0Pause;
      }
    } else {
      char *prompt_text = strstr((const char *)responseLine + responseLineIndex, "//action:prompt_end") + 19;
      // *get the first token
      token         = strtok(prompt_text, ch);
      popup_message = token;
      popupReminder((u8 *)popup_title, (u8 *)popup_message);
    }
  } else if (strstr((const char *)responseLine + responseLineIndex, "//action:notification")) {
    char *prompt_text = strstr((const char *)responseLine + responseLineIndex, "//action:notification") + 21;
    // *get the first token
    token         = strtok(prompt_text, ch);
    popup_message = token;
    popupReminder((u8 *)popup_title, (u8 *)popup_message);
  } else if (strstr((const char *)responseLine + responseLineIndex, "echo:")) {
    // *Break it up into useful sections
    char *prompt_text = strstr((const char *)responseLine + responseLineIndex, "echo:") + 5;
    // *get the first token
    token         = strtok(prompt_text, ch);
    popup_message = token;

    popupReminder((u8 *)popup_title, (u8 *)popup_message);
  } else {
    popupReminder((u8 *)title, (u8 *)responseLine + responseLineIndex);
  }
  popup_message = 0;
}

/**
 * queueCncResponses.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @global
 * @param	uint8_t	port
 * @return	void
 */
void queueCncResponses(uint8_t port) {
  char cncResponse[MAX_RESPONSE_SIZE];
  int i;
  for (i = 0; cncIncoming[port].processedIndex != cncIncoming[port].pendingIndex && gcodeResponse[port].count < RESPONSE_QUEUE_SIZE; i++) {
    cncResponse[i] = cncIncoming[port].responseBuffer[cncIncoming[port].processedIndex + i];
    if (strchr(cncResponse[i], "\n")) {
      cncResponse[i] = 0;   // End character
      while (strchr(cncResponse[strlen(cncResponse) - 1], " ")) {
        // Remove trailing spaces
        cncResponse[strlen(cncResponse) - 1] = 0;                                                            // Remove last character
        cncIncoming[port].processedIndex     = (cncIncoming[port].processedIndex + 1) % MAX_RESPONSE_SIZE;   // Account for the missing character
      }
      addCncResponseToQueue(&cncResponse[0], port);

      i = -1;   // Reset i to process the next response line
    } else if (i == 0 && strchr(cncResponse[i], " ")) {
      // Ignore leading spaces
      cncIncoming[port].processedIndex = (cncIncoming[port].processedIndex + 1) % MAX_RESPONSE_SIZE;   // Account for the missing character

      i = -1;   // Reset i to ignore leading space
    }
  }
}

/**
 * addCncResponseToQueue.
 *
 * @version	v1.0.0	Friday, March 27th, 2020.
 * @global
 * @param	char   	*gcodeString
 * @param	uint8_t	port
 * @return	void
 */
void addCncResponseToQueue(char *gcodeString, uint8_t port) {
  gcodeResponse[port].queueIndex = (gcodeResponse[port].queueIndex + 1) % RESPONSE_QUEUE_SIZE;
  strncpy(gcodeResponse[port].queue[gcodeResponse[port].queueIndex].response, gcodeString, MAX_RESPONSE_SIZE - 1);
  if (gcodeResponse[port].count < RESPONSE_QUEUE_SIZE) gcodeResponse[port].count++;

  cncIncoming[port].processedIndex = (cncIncoming[port].processedIndex + strlen(gcodeString) + 1) % MAX_RESPONSE_SIZE;
}
