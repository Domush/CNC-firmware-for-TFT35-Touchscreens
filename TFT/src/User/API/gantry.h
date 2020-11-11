#ifndef _GANTRY_H_
#define _GANTRY_H_
#include "includes.h"

void storegantry(int n, float val);
float getAxisLocation(uint8_t n);
void gantry_dec(int n, float val);
void gantry_inc(int n, float val);

void drawXYZ(void);

#endif
