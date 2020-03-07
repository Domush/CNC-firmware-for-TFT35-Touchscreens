#ifndef _GCODE_H_
#define _GCODE_H_
#include "stdbool.h"
#include "interfaceCmd.h"

#define RESPONSE_MAX_CHARS 5000

typedef struct {
  char command[GCODE_MAX_CHARACTERS];         // The command sent to printer
  char responseBegin[GCODE_MAX_CHARACTERS];   // The magic to identify the start
  char responseEnd[GCODE_MAX_CHARACTERS];     // The magic to identify the stop
  char responseError[GCODE_MAX_CHARACTERS];   // The magic to identify the error response
  bool responseInProgress;                    // true if between start and stop magic
  bool waitingForResponse;                    // true if waiting for start magic
  bool commandComplete;                       // true if command is executed and response is received
  bool responseErrorTriggered;                // true if error response
  char *commandResponse;                      // buffer where store the CNC response
} REQUEST_COMMAND_INFO;

REQUEST_COMMAND_INFO requestCommandInfo;

void clearRequestCommandInfo(void);
bool RequestCommandInfoIsRunning(void);

char *request_M20(void);
bool request_M21(void);
long request_M23(char *filename);

bool request_M24(int pos);
bool request_M25(void);

bool request_M27(int seconds);

char *request_M33(char *filename);
bool request_M524(void);

#endif
