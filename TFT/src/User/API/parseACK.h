#ifndef _PARSEACK_H_
#define _PARSEACK_H_

#include "stdint.h"
#include "Configuration.h"

static const char replyError[] = "Error:";
static const char replyEcho[] = "echo:";
static const char replyBusy[] = "busy:";
static const char unknowmagic[] = "Unknown command:";
#ifdef ONBOARD_SD_SUPPORT
static const char replySDPrinting[] = "SD printing byte";
static const char replySDNotPrinting[] = "Not SD printing";
#endif

#define ACK_MAX_SIZE 1024
extern int MODEselect;
char *popup_title;
char *popup_message;

void setCurrentAckSrc(uint8_t src);
void parseACK(void);
void parseRcvGcode(void);

#endif
