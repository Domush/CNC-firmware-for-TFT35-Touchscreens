#ifndef _SPEED_H_
#define _SPEED_H_

#include "stdint.h"
#include "stdbool.h"

void setCNCSpeedOverride(uint16_t percent);
uint16_t getCNCSpeedOverride(void);
void menuSpeed(void);

#endif
