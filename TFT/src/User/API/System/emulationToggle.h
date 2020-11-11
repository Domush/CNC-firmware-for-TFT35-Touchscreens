#ifndef _EMULATIONTOGGLE_H_
#define _EMULATIONTOGGLE_H_
#include "includes.h"

#define IDLE_TOUCH 0xFFFF
#define SELECTMODE 2
typedef enum {
  MKEY_0 = 0,
  MKEY_1,
  MKEY_IDLE = IDLE_TOUCH,
} MKEY_VALUES;

bool LCD_ReadPen(uint8_t intervals);
uint8_t LCD_ReadTouch(void);
void Touch_Sw(uint8_t value);
bool LCD_BtnTouch(uint8_t intervals);

//select mode fun
void show_selectICON(void);
MKEY_VALUES MKeyGetValue(void);
void selectmode(int8_t nowMode);

#endif   // _EMULATIONTOGGLE_H_
