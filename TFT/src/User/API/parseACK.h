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

#define MAX_RESPONSE_SIZE 512
extern int MODEselect;
char *popup_title;
char *popup_message;

void setGcodeCommandSource(uint8_t src);
void parseGcodeResponse(void);
void parseSerialGcode(void);
void showPopupMessage(char *info);
void copyIncomingToResponse(uint8_t port);

#endif
