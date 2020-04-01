#ifndef _SEND_GCODE_H_
#define _SEND_GCODE_H_
#include "includes.h"

// #include "menu.h"

typedef enum {
  GKEY_0 = 0,
  GKEY_1,
  GKEY_2,
  GKEY_3,
  GKEY_4,
  GKEY_DEL,
  GKEY_5,
  GKEY_6,
  GKEY_7,
  GKEY_8,
  GKEY_9,
  GKEY_SPACE,
  GKEY_10,
  GKEY_11,
  GKEY_12,
  GKEY_13,
  GKEY_14,
  GKEY_ABC_123,
  GKEY_15,
  GKEY_16,
  GKEY_17,
  GKEY_18,
  GKEY_19,
  GKEY_20,
  GKEY_BACK,
  GKEY_SEND,
  GKEY_IDLE = IDLE_TOUCH,
} GKEY_VALUES;

typedef enum {
  TFT_SOURCE,
  CNC_SOURCE,
} COMMAND_SOURCE;

void menuSendGcode(void);
void menuTerminal(void);
void showInTerminal(char *serial_text, COMMAND_SOURCE src);
void showGcodeStatus(char *serial_text, COMMAND_SOURCE src);

#endif
