#ifndef _GCODERESPONSEHANDLER_H_
#define _GCODERESPONSEHANDLER_H_
#include "includes.h"

static const char replyError[]          = "Error:";
static const char replyEcho[]           = "echo:";
static const char replyBusy[]           = "busy:";
static const char replyUnknownCommand[] = "Unknown command:";
#ifdef ONBOARD_SD_SUPPORT
static const char replySDPrinting[]    = "SD printing byte";
static const char replySDNotPrinting[] = "Not SD printing";
#endif

#define RESPONSE_BUFFER_SIZE 480
#define RESPONSE_MAX_CHARS   120
#define RESPONSE_QUEUE_SIZE  20

typedef struct RESPONSE {
  char response[RESPONSE_MAX_CHARS];   // Stored CNC response message
} RESPONSE;

typedef struct RESPONSE_QUEUE {
  RESPONSE queue[RESPONSE_QUEUE_SIZE];
  uint8_t queueIndex;   // The last queue position
  uint8_t count;        // Count of commands in the queue
  uint16_t timeout;     // How long to wait before timing out and moving on
} RESPONSE_QUEUE;

RESPONSE_QUEUE gcodeResponse[3];   // Incoming gcode response

char *popup_title;     // Title of popup window
char *popup_message;   // Message text of popup window

void setGcodeCommandSource(uint8_t src);
void parseGcodeResponse(void);
void parseSerialGcode(void);
void showPopupMessage(char *title);
void queueCncResponses(uint8_t port);

#endif   // _GCODERESPONSEHANDLER_H_
