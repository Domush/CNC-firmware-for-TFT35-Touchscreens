#ifndef _ROUTER_H_
#define _ROUTER_H_
#include "includes.h"
// #include "stdint.h"
// #include "stdbool.h"

// const char* routerID;
// const uint8_t routerMaxPWM;

void menuRouter(void);

void routerControl(uint8_t speed);
void routerChangeBit(void);

#endif
