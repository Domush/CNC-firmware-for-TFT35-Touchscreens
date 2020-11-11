#include "gcodeResponseHandler.h"
#include "includes.h"

// Multi-language support
#include "Language/Language.h"

// Chip specific includes
#include "Serial.h"
#include "usart.h"

// File handling
#include "list_item.h"

// Gcode processing
#include "Gcode/gcodeSender.h"
#include "Gcode/gcodeRequests.h"

// Base API functions
#include "API/gantry.h"

// Timing functions
#include "System/os_timer.h"

// Menus
#include "includesMenus.h" // All menu headers

// RESPONSE_QUEUE gcodeResponse[3];   // Last gCode responses
static char responseLine[RESPONSE_MAX_CHARS];   // Current response being processed
static u8 responseLineIndex = 0;                // Char reference # of responseLine
static u8 curSerialPort     = SERIAL_PORT;      // Defaults to direct CNC serial

// *Ignore reply "echo:" messages containing these messages
// *(Don't display as a popup)
const char *const echoStringsToIgnore[] = {
    "Now fresh file:",
    "Probe Z Offset:",
    "echo:LPC1768",
};

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
  uint8_t port = 0;
  for (port = 0; port < _USART_CNT; port++) {
    if (port != SERIAL_PORT && infoHost.responseReceived[port] == true) {
      queueCncResponses(port);
      while (gcodeResponse[port].count > 0) {
        strncpy(responseLine, gcodeResponse[port].queue[(gcodeResponse[port].queueIndex - gcodeResponse[port].count) % RESPONSE_QUEUE_SIZE].response, RESPONSE_MAX_CHARS - 1);
        queueCommandFromSerial(port, responseLine);
        gcodeResponse[port].count--;
      }
      infoHost.responseReceived[port] = false;   // *All response data has been processed
    }
  }
#endif
}

void setGcodeCommandSource(uint8_t src) {
  curSerialPort = src;
}

// *Check if responseLine contains string, then record the location using responseLineIndex
static bool responseContains(const char *string, u8 beginAtChar) {
  const char *match;
  match = strstr(&responseLine[beginAtChar], string);
  if (match) {
    responseLineIndex = strlen(responseLine) - strlen(match) + strlen(string);
    if (responseLine[responseLineIndex] == ' ') {
      responseLineIndex++;
    }
    return true;
  } else {
    return false;
  }
}

// // *Check if responseLine is *exactly* string
// static bool responseIsExactly(const char *string) {
//   if ((const char *)responseLine == string) {
//     return true;
//   } else {
//     return false;
//   }
// }

// *Check if responseLine begins with string
static bool responseBeginsWith(const char *string) {
  if (strstr(responseLine, string) == responseLine) {
    return true;
  } else {
    return false;
  }
}

// *Get the float value from responseLine at the current gcodeLineIndex
static float responseValueFloat(void) {
  return (strtod(&responseLine[responseLineIndex], NULL));
}

// *Get the integer value from responseLine at the current gcodeLineIndex
static int responseValue(void) {
  return (strtol(&responseLine[responseLineIndex], NULL, 10));
}

void showPopupMessage(char *title) {
  const char ch[2] = "\n";
  char *token;
  token       = 0;
  popup_title = title;
  if (!popup_message) {
    popup_message = "No reason given. Continue when ready.";
  }
  if (infoMenu.menu[infoMenu.active] == menuSettingsTMC) return;
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

//* Add CNC response lines to the gcodeResponse queue
void addCncResponseToQueue(char *gcodeString, uint8_t port) {
  strncpy(gcodeResponse[port].queue[gcodeResponse[port].queueIndex].response, gcodeString, RESPONSE_MAX_CHARS - 1);
  gcodeResponse[port].queueIndex = (gcodeResponse[port].queueIndex + 1) % RESPONSE_QUEUE_SIZE;
  if (gcodeResponse[port].count < RESPONSE_QUEUE_SIZE) gcodeResponse[port].count++;

  cncIncoming[port].processedIndex = (cncIncoming[port].processedIndex + strlen(gcodeString) + 1) % RESPONSE_BUFFER_SIZE;
}

//* Split incoming serial responses by '\n' and send them to the gcodeResponse queue
void queueCncResponses(uint8_t port) {
  char cncResponse[RESPONSE_MAX_CHARS];
  int i;
  for (i = 0; cncIncoming[port].processedIndex != cncIncoming[port].pendingIndex && gcodeResponse[port].count < RESPONSE_QUEUE_SIZE; i++) {
    cncResponse[i] = cncIncoming[port].responseBuffer[cncIncoming[port].processedIndex + i];
    if (cncResponse[i] == '\n') {
      cncResponse[i] = 0;   // End character
      while (cncResponse[strlen(cncResponse) - 1] == ' ') {
        // Remove trailing spaces
        cncResponse[strlen(cncResponse) - 1] = 0;                                                               // Remove last character
        cncIncoming[port].processedIndex     = (cncIncoming[port].processedIndex + 1) % RESPONSE_BUFFER_SIZE;   // Account for the missing character
      }
      addCncResponseToQueue(cncResponse, port);
      cncResponse[0] = 0;    // Empty out the last response
      i              = -1;   // Reset i to process the next response line
    } else if (i == 0 && cncResponse[i] == ' ') {
      // Ignore leading spaces
      cncIncoming[port].processedIndex = (cncIncoming[port].processedIndex + 1) % RESPONSE_MAX_CHARS;   // Account for the missing character

      i = -1;   // Reset i to ignore leading space
    }
  }
}

//* Process all queued responses
void parseGcodeResponse(void) {
  static u8 connectionRetryDelay = 2;   // # of seconds to wait before retrying to connect
  static u16 connectionRetryTime = 0;   // stored timestamp for reconnect attempt
  bool hideResponsesInTerminal   = false;
  queueCncResponses(SERIAL_PORT);
  // *Only process response data if data is waiting from SERIAL_PORT
  if (infoHost.responseReceived[SERIAL_PORT] != true) {
    // *Look for Marlin and wake it up if sleeping
    if (infoHost.connected == false) {
      timedMessage(1, TIMED_CRITICAL, (char *)textSelect(LABEL_UNCONNECTED));
      if (OS_GetTime() - connectionRetryDelay > connectionRetryTime) {
        connectionRetryTime = OS_GetTime();
        queueCommand(false, "G53\n");   // *Attempts to send a "wake up" packet to trigger a connection
      }
    }
    return;
  }

  while (gcodeResponse[SERIAL_PORT].count > 0) {
    strncpy(responseLine, gcodeResponse[SERIAL_PORT].queue[(gcodeResponse[SERIAL_PORT].queueIndex - gcodeResponse[SERIAL_PORT].count) % RESPONSE_QUEUE_SIZE].response, RESPONSE_MAX_CHARS);

    // *Look for Marlin and wake it up if sleeping
    if (infoHost.connected == false) {
      if (responseBeginsWith("ok") || responseBeginsWith("wait")) {
        // *Connection established! Let the Gcode Games begin!
        infoHost.connected = true;
        // Restore leveling, if enabled
#ifdef AUTO_SAVE_LOAD_LEVELING_VALUE
        queueCommand(false, "M420 S1\n");
#endif
      }

      // Prepared-request response handler
      if (requestCommandInfo.waitingForResponse || requestCommandInfo.responseInProgress) {
        if (responseContains(requestCommandInfo.responseBegin, 0)) {
          // *Found the response we wanted
          requestCommandInfo.responseInProgress = true;
          requestCommandInfo.waitingForResponse = false;
          if (strlen(requestCommandInfo.commandResponse) + strlen(responseLine) < RESPONSE_MAX_CHARS) {
            strcat(requestCommandInfo.commandResponse, responseLine);
            if (responseContains(requestCommandInfo.responseEnd, 0)) {
              requestCommandInfo.responseInProgress = false;
              requestCommandInfo.commandComplete    = true;
            }
          } else {
            requestCommandInfo.responseInProgress = false;
            showPopupMessage((char *)replyError);
          }
        } else if (responseContains(requestCommandInfo.responseError, 0)) {
          requestCommandInfo.commandComplete        = true;
          requestCommandInfo.responseInProgress     = false;
          requestCommandInfo.responseErrorTriggered = true;
        }
        infoHost.waitForResponse = false;
        goto parse_end;
      }
      // end

      if (responseBeginsWith("ok")) {
        infoHost.waitForResponse = false;
      } else {
        // non-"ok" response section
        if (responseContains("X:", 0)) {
          storegantry(0, responseValueFloat());
          if (responseContains("Y:", responseLineIndex)) {
            storegantry(1, responseValueFloat());
            if (responseContains("Z:", responseLineIndex)) {
              storegantry(2, responseValueFloat());
            }
          }
        } else if (responseContains("Mean:", 0)) {
          popupReminder((u8 *)"Repeatability Test", (u8 *)responseLine + responseLineIndex - 5);
        } else if (responseContains(replyEcho, 0) && responseContains(replyBusy, 0) && responseContains("processing", 0)) {
          timedMessage(2, TIMED_WARNNG, (char *)textSelect(LABEL_BUSY));
        } else if (responseContains(replyEcho, 0) && responseContains(replyBusy, 0) && responseContains("paused for user", 0)) {
          timedMessage(2, TIMED_WARNNG, "CNC waiting for user");
          goto parse_end;
        } else if (responseContains("X driver current: ", 0)) {
          Get_parameter_value[0] = responseValue();
          if (responseContains("Y driver current: ", responseLineIndex))
            Get_parameter_value[1] = responseValue();
          if (responseContains("Z driver current: ", responseLineIndex))
            Get_parameter_value[2] = responseValue();
        } else if (responseContains("M92 X", 0)) {
          Get_parameter_value[3] = responseValue();
          if (responseContains("Y", responseLineIndex))
            Get_parameter_value[4] = responseValue();
          if (responseContains("Z", responseLineIndex))
            Get_parameter_value[5] = responseValue();
        }
#ifdef ONBOARD_SD_SUPPORT
        else if (responseContains(replySDNotPrinting, 0) && infoMenu.menu[infoMenu.active] == menuJobStatus) {
          infoHost.jobInProgress = false;
          jobComplete();

        } else if (responseContains(replySDPrinting, 0)) {
          if (infoMenu.menu[infoMenu.active] != menuJobStatus && !infoHost.jobInProgress) {
            infoMenu.menu[++infoMenu.active] = menuJobStatus;
            infoHost.jobInProgress           = true;
          }
          // Parsing printing data
          // Example: SD printing byte 123/12345
          char *ptr;
          u32 position = strtol(strstr(responseLine, "byte ") + 5, &ptr, 10);
          jobSetCurrentLine(position);
          //      powerFailedCache(position);
        }
#endif
        else if (responseContains("//action:", 0)) {
          showPopupMessage((char *)"Notice:");
        } else if (responseContains("error:", 0)) {
          showPopupMessage((char *)"Error detected:");
        } else if (responseContains("echo:", 0)) {
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
      if (responseContains(" F0:", 0)) {
        routerControl(responseValue());
      }

    parse_end:
      if (curSerialPort != SERIAL_PORT) {
        sendCommand(curSerialPort, responseLine);
      }
      showGcodeStatus(responseLine, CNC_SOURCE);
      if (hideResponsesInTerminal != true) {
        showInTerminal(responseLine, CNC_SOURCE);
      }
    }
    gcodeResponse[SERIAL_PORT].count--;
  }
  infoHost.responseReceived[SERIAL_PORT] = false;   // *All response data has been processed
}
