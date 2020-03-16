#ifndef _INTERFACECMD_H_
#define _INTERFACECMD_H_

#include "stdint.h"
#include "stdbool.h"
#include "parseAck.h"

#define GCODE_QUEUE_MAX      20
#define GCODE_MAX_CHARACTERS 96

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
} QUEUE;

extern QUEUE gcodeCommand;   // Outgoing gcode command
uint8_t curRouterSpeed;

bool storeCmd(const char* gcodeString, ...);

bool storeCmdFromUART(uint8_t port, const char* gcode);

bool isQueueFull(void);
void clearCmdQueue(void);

void sendGcodeCommands(void);

void menuM0Pause(void);
void menuChangeBit(void);

#endif
