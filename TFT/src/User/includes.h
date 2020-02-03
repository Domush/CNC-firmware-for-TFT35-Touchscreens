#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include "variants.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_misc.h"
#include "ff.h"

#include "delay.h"
#include "os_timer.h"
#include "variants.h"

#include "boot.h"

#include "GUI.h"
#include "LCD_Init.h"
#include "Language.h"
#include "lcd.h"
#include "lcd_dma.h"
#include "utf8_decode.h"

#include "Serial.h"
#include "spi.h"
#include "spi_slave.h"
#include "sw_spi.h"
#include "usart.h"

#include "usbh_core.h"
#include "usbh_msc_core.h"
#include "usbh_usr.h"

#include "sd.h"
#include "w25qxx.h"
#include "xpt2046.h"

#include "Gcode/gcode.h"
#include "Gcode/mygcodefs.h"
#include "LCD_Encoder.h"
#include "Parametersetting.h"
#include "ST7920_Simulator.h"
#include "Selectmode.h"
#include "Vfs/vfs.h"
#include "coordinate.h"
#include "flashStore.h"
#include "interfaceCmd.h"
#include "myfatfs.h"
#include "parseACK.h"
#include "touch_process.h"
#include "ui_draw.h"

#include "extend.h"

//menu
#include "BabyStep.h"
#include "Heat.h"
#include "Home.h"
#include "MainPage.h"
#include "More.h"
#include "Move.h"
#include "PreheatMenu.h"
#include "Print.h"
#include "Printing.h"
#include "Speed.h"
#include "ledcolor.h"
#include "menu.h"

#include "Extrude.h"
#include "Fan.h"
#include "FeatureSettings.h"
#include "MachineSettings.h"
#include "PowerFailed.h"
#include "ProbeOffset.h"
#include "ScreenSettings.h"
#include "SendGcode.h"
#include "Settings.h"
#include "leveling.h"

#include "Mode.h"
#include "Popup.h"

#include "StatusScreen.h"
#include "UnifiedHeat.h"
#include "UnifiedMove.h"
#include "list_item.h"

#define MAX_MENU_DEPTH 10  // max sub menu depth
typedef void (*FP_MENU)(void);

typedef struct
{
  FP_MENU menu[MAX_MENU_DEPTH];  // Menu function buffer
  u8 cur;                        // Current menu index in buffer
} MENU;

extern MENU infoMenu;

typedef struct
{
  bool wait;               //Whether wait for Marlin's response
  bool rx_ok[_USART_CNT];  //Whether receive Marlin's response or get Gcode by other UART(ESP3D/OctoPrint)
  bool connected;          //Whether have connected to Marlin
  bool printing;           //Whether the host is busy in printing execution. ( USB serial printing and GCODE print from onboard)
} HOST;

extern HOST infoHost;

#endif
