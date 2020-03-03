#ifndef _INTERFACECMD_H_
#define _INTERFACECMD_H_

#include "stdint.h"
#include "stdbool.h"

#define GCODE_QUEUE_MAX 2
#define GCODE_MAX_CHARACTERS 150

typedef struct
{
  char gcode[GCODE_MAX_CHARACTERS];
  uint8_t src;  // 0: TouchScreen Cmd, 1: Serial Port 2 rx Cmd, 2: Serial Port 3 rx Cmd
} GCODE;

typedef struct
{
  GCODE queue[GCODE_QUEUE_MAX];
  uint8_t readIndex;   // Ring buffer read position
  uint8_t writeIndex;  // Ring buffer write position
  uint8_t count;       // Count of commands in the queue
} QUEUE;

extern QUEUE gcodeCommand;       // Outgoing gcode command
extern QUEUE gcodeCommandQueue;  // Cache for gcode commands waiting for gcodeCommand to empty
uint8_t curRouterSpeed;

bool storeCmd(const char* format, ...);
void mustStoreCmd(const char* format, ...);

bool storeCmdFromUART(uint8_t port, const char* gcode);
void mustStoreCacheCmd(const char* format, ...);

bool moveCacheToCmd(void);
void clearCmdQueue(void);

void parseQueueCmd(void);
void sendGcodeCommands(void);

void menuM0Pause(void);
void menuChangeBit(void);

#endif
