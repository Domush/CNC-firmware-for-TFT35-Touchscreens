#ifndef _MENUPOWER_H_
#define _MENUPOWER_H_
#include "includes.h"

// Chip specific includes
#include "Serial.h"

// USB drive support (select TFT models)
#include "usbh_usr.h"

// Base API functions
#include "API/coordinate.h"

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
