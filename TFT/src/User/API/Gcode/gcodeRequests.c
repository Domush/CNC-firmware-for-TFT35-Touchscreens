//#include "Gcode/gcodeRequests.h"
#include "includes.h"

REQUEST_COMMAND_INFO requestCommandInfo;
bool WaitingGcodeResponse = false;

static void resetRequestCommandInfo(void) {
  requestCommandInfo.commandResponse = malloc(PREPARED_RESPONSE_MAX_CHARS);
  while (!requestCommandInfo.commandResponse)
    ;   // malloc failed
  memset(requestCommandInfo.commandResponse, 0, PREPARED_RESPONSE_MAX_CHARS);
  requestCommandInfo.waitingForResponse     = true;
  requestCommandInfo.responseInProgress     = false;
  requestCommandInfo.commandComplete        = false;
  requestCommandInfo.responseErrorTriggered = false;
}

bool RequestCommandInfoIsRunning(void) {
  return WaitingGcodeResponse;   //i try to use requestCommandInfo.commandComplete but does not work as expected ...
}

void clearRequestCommandInfo(void) {
  free(requestCommandInfo.commandResponse);
}

/*
    Send M21 command and wait for response

    >>> M21
    SENDING:M21
    echo:SD card ok
    echo:SD init fail

*/
bool request_M21(void) {
  strcpy(requestCommandInfo.command, "M21\n");
  strcpy(requestCommandInfo.responseBegin, "SD");
  strcpy(requestCommandInfo.responseEnd, "card ok");
  strcpy(requestCommandInfo.responseError, "init fail");

  resetRequestCommandInfo();
  queueCommand(false, requestCommandInfo.command);
  // Wait for response
  WaitingGcodeResponse = true;
  while (!requestCommandInfo.commandComplete) {
    runUpdateLoop();
  }
  WaitingGcodeResponse = false;
  clearRequestCommandInfo();
  // Check reponse
  return !requestCommandInfo.responseErrorTriggered;
}

/*
SENDING:M20
Begin file list
PI3MK2~1.GCO 11081207
/YEST~1/TEST2/PI3MK2~1.GCO 11081207
/YEST~1/TEST2/PI3MK2~3.GCO 11081207
/YEST~1/TEST2/PI3MK2~2.GCO 11081207
/YEST~1/TEST2/PI3MK2~4.GCO 11081207
/YEST~1/TEST2/PI3MK2~5.GCO 11081207
/YEST~1/PI3MK2~1.GCO 11081207
/YEST~1/PI3MK2~3.GCO 11081207
/YEST~1/PI3MK2~2.GCO 11081207
End file list
*/
char *request_M20(void) {
  strcpy(requestCommandInfo.command, "M20\n");
  strcpy(requestCommandInfo.responseBegin, "Begin file list");
  strcpy(requestCommandInfo.responseEnd, "End file list");
  strcpy(requestCommandInfo.responseError, "Error");
  resetRequestCommandInfo();
  queueCommand(false, requestCommandInfo.command);
  // Wait for response
  WaitingGcodeResponse = true;
  while (!requestCommandInfo.commandComplete) {
    runUpdateLoop();
  }
  WaitingGcodeResponse = false;
  //clearRequestCommandInfo(); //shall be call after copying the buffer ...
  return requestCommandInfo.commandResponse;
}

/*
 * M33 retrieve long filename from short file name
 *   M33 miscel~1/armchair/armcha~1.gco
 * Output:
 *   /Miscellaneous/Armchair/Armchair.gcode
*/
char *request_M33(char *filename) {
  sprintf(requestCommandInfo.command, "M33 %s\n", filename);
  strcpy(requestCommandInfo.responseBegin, "/");   //un caractere qui est dans la ligne a traiter
  strcpy(requestCommandInfo.responseEnd, "ok");
  strcpy(requestCommandInfo.responseError, "Cannot open subdir");
  resetRequestCommandInfo();
  queueCommand(false, requestCommandInfo.command);
  // Wait for response
  WaitingGcodeResponse = true;
  while (!requestCommandInfo.commandComplete) {
    runUpdateLoop();
  }
  WaitingGcodeResponse = false;
  //clearRequestCommandInfo(); //shall be call after copying the buffer ...
  return requestCommandInfo.commandResponse;
}

/**
 * Select the file to print
 *
 * >>> m23 YEST~1/TEST2/PI3MK2~5.GCO
 * SENDING:M23 YEST~1/TEST2/PI3MK2~5.GCO
 * echo:Now fresh file: YEST~1/TEST2/PI3MK2~5.GCO
 * File opened: PI3MK2~5.GCO Size: 11081207
 * File selected
 **/
long request_M23(char *filename) {
  sprintf(requestCommandInfo.command, "M23 %s\n", filename);
  strcpy(requestCommandInfo.responseBegin, "File opened");
  strcpy(requestCommandInfo.responseEnd, "File selected");
  strcpy(requestCommandInfo.responseError, "open failed");
  resetRequestCommandInfo();
  queueCommand(false, requestCommandInfo.command);
  // Wait for response
  WaitingGcodeResponse = true;
  while (!requestCommandInfo.commandComplete) {
    runUpdateLoop();
  }
  WaitingGcodeResponse = false;
  // Find file size and report its.
  char *ptr;
  long size = strtol(strstr(requestCommandInfo.commandResponse, "Size:") + 5, &ptr, 10);
  clearRequestCommandInfo();
  return size;
}

/**
 * Start or resume print
 **/
bool request_M24(int file_position) {
  if (file_position == 0) {
    queueCommand(false, "M24\n");
  } else {
    char command[100];
    sprintf(command, "M24 S%d\n", file_position);
    queueCommand(false, command);
  }
  return true;
}

/**
 * Abort print
 **/
bool request_M524(void) {
  queueCommand(false, "M524\n");
  return true;
}
/**
 * Pause print
 **/
bool request_M25(void) {
  queueCommand(false, "M25\n");
  return true;
}

/**
 * Print status ( start auto report)
 * ->  SD printing byte 123/12345
 * ->  Not SD printing
 **/
bool request_M27(int update_delay_seconds) {
  char command[10];
  sprintf(command, "M27 S%d\n", update_delay_seconds);
  queueCommand(false, command);
  return true;
}
