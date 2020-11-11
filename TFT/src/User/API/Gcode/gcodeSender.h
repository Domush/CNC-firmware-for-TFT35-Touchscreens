#ifndef _GCODESENDER_H_
#define _GCODESENDER_H_
#include "includes.h"

// Chip specific includes
#include "Serial.h"
#include "usart.h"

// File handling
#include "list_item.h"

#define GCODE_QUEUE_SIZE     20
#define GCODE_MAX_CHARACTERS 99

typedef struct GCODE {
  char gcode[GCODE_MAX_CHARACTERS];
  uint8_t src;  // 0: TouchScreen Cmd, 1: Serial Port 2 rx Cmd, 2: Serial Port 3 rx Cmd
} GCODE;

typedef struct GCODE_QUEUE {
  GCODE queue[GCODE_QUEUE_SIZE];
  uint8_t queueIndex;  // Current queue position
  uint8_t count;       // Count of unsent commands
} GCODE_QUEUE;

GCODE_QUEUE gcodeOutgoing;  // Outgoing gcode queue

bool addGcodeCommand(bool skipIfQueueFull, const char* gcodeString, uint8_t port);
bool queueCommand(bool skipIfQueueFull, char* gcodeString, ...);

void queueCommandFromSerial(uint8_t port, const char* gcode);

bool isQueueFull(void);
void clearGcodeQueue(void);

void parseGcodeOutgoing(void);

void menuM0Pause(void);
void menuChangeBit(void);

int lastGcodeQueueValue = 0;
u16 queueTextColor      = MAT_LOWWHITE;
void gcodeQueueStatus(void);

#endif  // _GCODESENDER_H_
