#ifndef _PREHEATMENU_H_
#define _PREHEATMENU_H_

#include "Configuration.h"

typedef enum
{
  BOTH = 0,
  BED_PREHEAT = 1,
  SPINDLE0_PREHEAT = 2,
}TOOLPREHEAT;


void menuPreheat(void);

#endif

