#ifndef _POWERFAILED_H_
#define _POWERFAILED_H_

#include "includes.h"
// #include "System/variants.h"
// #include "coordinate.h"
// #include "ff.h"
// #include "Configuration.h"

#define BREAK_POINT_FILE "Printing.sys"

typedef struct
{
  float axis[TOTAL_AXIS];
  u8 gantryspeed;
  u8 speed;
  u8 routerSpeed;
  u32 offset;
  bool relative;
} BREAK_POINT;

void menuPowerOff(void);

void powerFailedSetDriverSource(char *src);
bool powerFailedCreate(char *path);
void powerFailedCache(u32 offset);
void powerFailedClose(void);
void powerFailedDelete(void);

void powerFailedEnable(bool enable);
bool powerFailedlSeek(FIL *fp);

#endif
