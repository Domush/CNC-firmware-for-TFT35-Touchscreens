#ifndef _PARSEACK_H_
#define _PARSEACK_H_

#include "stdint.h"
#include "Configuration.h"

static const char replyError[]          = "Error:";
static const char replyEcho[]           = "echo:";
static const char replyBusy[]           = "busy:";
static const char replyUnknownCommand[] = "Unknown command:";
#ifdef ONBOARD_SD_SUPPORT
static const char replySDPrinting[]    = "SD printing byte";
static const char replySDNotPrinting[] = "Not SD printing";
#endif

#define MAX_RESPONSE_SIZE   120
#define RESPONSE_QUEUE_SIZE 20

typedef struct
{
  char response[MAX_RESPONSE_SIZE];
  uint8_t src;   // 0: TouchScreen Cmd, 1: Serial Port 2 rx Cmd, 2: Serial Port 3 rx Cmd
} RESPONSE;

typedef struct
{
  RESPONSE queue[RESPONSE_QUEUE_SIZE];
  uint8_t queueIndex;   // The last queue position
  uint8_t count;        // Count of commands in the queue
  uint16_t timeout;     // How long to wait before timing out and moving on
} RESPONSE_QUEUE;

RESPONSE_QUEUE gcodeResponse;   // Outgoing gcode command

extern int MODEselect;
char *popup_title;
char *popup_message;

void setGcodeCommandSource(uint8_t src);
void parseGcodeResponse(void);
void parseSerialGcode(void);
void showPopupMessage(char *info);
void copyIncomingToResponse(uint8_t port);

#endif
