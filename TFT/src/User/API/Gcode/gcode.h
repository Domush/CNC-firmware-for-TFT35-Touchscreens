#ifndef _GCODE_H_
#define _GCODE_H_
#include "stdbool.h"
#include "interfaceCmd.h"

#define RESPONSE_MAX_CHARS 5000

typedef struct {
  char command[GCODE_MAX_CHARACTERS];     // The command sent to printer
  char startMagic[GCODE_MAX_CHARACTERS];  // The magic to identify the start
  char stopMagic[GCODE_MAX_CHARACTERS];   // The magic to identify the stop
  char replyError[GCODE_MAX_CHARACTERS];  // The magic to identify the error response
  bool inResponse;                        // true if between start and stop magic
  bool inWaitResponse;                    // true if waiting for start magic
  bool done;                              // true if command is executed and response is received
  bool inError;                           // true if error response
  char *response;                         // buffer where store the command response
} REQUEST_COMMAND_INFO;

extern REQUEST_COMMAND_INFO requestCommandInfo;

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
