#ifndef _ROUTER_H_
#define _ROUTER_H_
#include "stdbool.h"
#include "stdint.h"

extern const char* routerID[];
extern const char* routerCmd[];

void menuRouter(void);

void routerSetSpeed(uint8_t i, uint8_t speed);
void routerSetSendWaiting(uint8_t i, bool isWaiting);
uint8_t routerGetCurIndex(uint8_t i);
void routerSetCurIndex(uint8_t i);
uint8_t routerGetSpeed(uint8_t i);
void routerControl(uint8_t speed);
void routerChangeBit(void);

#endif
