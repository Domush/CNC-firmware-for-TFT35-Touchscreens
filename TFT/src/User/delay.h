#ifndef _DELAY_H_
#define _DELAY_H_
#include "stdint.h"

void Delay_init(uint8_t clk);  //initialization
void Delay_ms(uint16_t ms);    //Delay millisecond
void Delay_us(uint32_t us);    //Delay microsecond
void Delay(uint32_t time);     //Software delay

#endif
