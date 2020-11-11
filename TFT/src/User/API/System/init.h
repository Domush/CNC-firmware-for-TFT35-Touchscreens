#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_
// C helper classes
#include <stdbool.h>
#include "variants.h"
// Chip specific includes
#include "usart.h"

#define MAX_MENU_DEPTH 10  // max sub menu depth
typedef void (*FP_MENU)(void);

typedef struct
{
  FP_MENU menu[MAX_MENU_DEPTH];  // Menu function buffer
  u8 active;                     // Current menu index in buffer
} MENU;

MENU infoMenu;

typedef struct
{
  bool waitForResponse;               //Whether wait for Marlin's response
  bool responseReceived[_USART_CNT];  //Whether receive Marlin's response or get Gcode by other UART(ESP3D/OctoPrint)
  bool connected;                     //Whether have connected to Marlin
  bool jobInProgress;                 //Whether the host is busy in printing execution. ( USB serial printing and GCODE print from onboard)
} HOST;

HOST infoHost;

void infoMenuSelect(void);
void menuMode(void);

#endif
