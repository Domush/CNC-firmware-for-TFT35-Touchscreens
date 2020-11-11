#ifndef _MENUROUTER_H_
#define _MENUROUTER_H_
#include "includes.h"


// const char* routerID;
// const uint8_t routerMaxPWM;

uint8_t curRouterSpeed;

void menuRouter(void);

void routerControl(uint8_t speed);
void routerChangeBit(void);

#endif
