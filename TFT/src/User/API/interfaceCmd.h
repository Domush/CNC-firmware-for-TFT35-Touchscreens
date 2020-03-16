#ifndef _INTERFACECMD_H_
#define _INTERFACECMD_H_

#include "stdint.h"
#include "stdbool.h"
#include "parseAck.h"

#define GCODE_QUEUE_MAX      20
#define GCODE_MAX_CHARACTERS 96

// typedef struct
// {
//   char sent[GCODE_MAX_CHARACTERS];
//   uint8_t sentIndex;   // Ring buffer read position
//   char response[MAX_RESPONSE_SIZE];
//   uint8_t responseIndex;   // Ring buffer read position
// } GCODE_LAST;

typedef struct
{
  char gcode[GCODE_MAX_CHARACTERS];
  uint8_t src;   // 0: TouchScreen Cmd, 1: Serial Port 2 rx Cmd, 2: Serial Port 3 rx Cmd
} GCODE;

typedef struct
{
  GCODE queue[GCODE_QUEUE_MAX];
  uint8_t readIndex;    // Ring buffer read position
  uint8_t writeIndex;   // Ring buffer write position
  uint8_t count;        // Count of commands in the queue
  uint16_t timeout;     // How long to wait before timing out and moving on
} QUEUE;

extern QUEUE gcodeCommand;   // Outgoing gcode command
// extern QUEUE gcodeCommandQueue;   // Cache for gcode commands waiting for gcodeCommand to empty
uint8_t curRouterSpeed;
// extern GCODE_LAST gcodeLastCommand;

bool storeCmd(const char* gcodeString, ...);

bool storeCmdFromUART(uint8_t port, const char* gcode);
void clearCmdQueue(void);

// void parseQueueCmd(void);
void sendGcodeCommands(void);

void menuM0Pause(void);
void menuChangeBit(void);

#endif
