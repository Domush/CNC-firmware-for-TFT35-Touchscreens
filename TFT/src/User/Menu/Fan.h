#ifndef _ROUTER_H_
#define _ROUTER_H_
#include "stdint.h"
#include "stdbool.h"

extern const char* routerID[];
extern const char* routerCmd[];
extern const uint8_t routerMaxPWM[];
// extern uint8_t curIndex;

void menuRouter(void);

void routerSetSpeed(uint8_t i, uint8_t speed);
uint8_t routerGetSpeed(uint8_t i);
void routerSetCurIndex(uint8_t i);
uint8_t routerGetCurIndex(uint8_t i);
void routerSetSendWaiting(uint8_t i, bool isWaiting);
void routerControl(uint8_t speed);
void routerChangeBit(void);

#endif
