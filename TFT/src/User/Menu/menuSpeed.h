#ifndef _MENUSPEED_H_
#define _MENUSPEED_H_
#include "includes.h"


void setCNCSpeedOverride(uint16_t percent);
uint16_t getCNCSpeedOverride(void);
void menuSpeed(void);

#endif
