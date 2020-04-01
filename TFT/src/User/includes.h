#ifndef _INCLUDES_H_
#define _INCLUDES_H_

// C helper classes
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "API/helperFunctions.h"

// Model specific configuration
#include "System/variants.h"
#include "Configuration.h"

// Timing functions
#include "System/os_timer.h"
#include "System/delay.h"
#include "System/boot.h"

// LCD init functions
#include "lcd.h"
#include "Hal/LCD_Init.h"
#include "lcd_dma.h"
#include "UI/GUI.h"

// Multi-language support
#include "Language/Language.h"
#include "Language/utf8_decode.h"

// Chip specific includes
#include "usart.h"
#include "Serial.h"
#include "spi.h"
#include "sw_spi.h"
#include "spi_slave.h"

// USB drive support (select TFT models)
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"

// SD card support
#include "Hal/sd.h"
#include "Hal/w25qxx.h"
#include "Hal/xpt2046.h"

// File handling
#include "System/LCD_Encoder.h"
#include "ST7920_Simulator.h"
#include "Fatfs/ff.h"
#include "Vfs/vfs.h"
#include "Fatfs/myfatfs.h"

// UI handling
#include "UI/ui_draw.h"
#include "UI/list_item.h"
#include "UI/touch_process.h"
#include "System/flashStorage.h"
#include "System/emulationToggle.h"

// Base API functions
#include "API/coordinate.h"
#include "API/gantry.h"
#include "API/menu.h"
#include "API/extendedFeatures.h"

// Gcode processing
#include "Gcode/gcodeFileHandler.h"
#include "Gcode/gcodeRequests.h"
#include "Gcode/gcodeSender.h"
#include "Gcode/gcodeResponseHandler.h"
// #include "interfaceCmd.h"
// #include "parseACK.h"

// Menus
#include "Menu/menuMain.h"   // Home screen
#include "Menu/menuMove.h"
#include "Menu/menuBabystepping.h"
#include "Menu/menuCustomGcode.h"
#include "Menu/menuFileSelect.h"
#include "Menu/menuHoming.h"
#include "Menu/menuJobSetup.h"
#include "Menu/menuJobStatus.h"
#include "Menu/menuLEDColor.h"
#include "Menu/menuPause.h"
#include "Menu/menuPopup.h"
#include "Menu/menuPower.h"
#include "Menu/menuRouter.h"
#include "Menu/menuSettingsFeatures.h"
#include "Menu/menuSettingsHome.h"
#include "Menu/menuSettingsMisc.h"
#include "Menu/menuSettingsTFT.h"
#include "Menu/menuSettingsTMC.h"
#include "Menu/menuSpeed.h"
#include "Menu/menuTerminal.h"

#define MAX_MENU_DEPTH 10   // max sub menu depth
typedef void (*FP_MENU)(void);

typedef struct
{
  FP_MENU menu[MAX_MENU_DEPTH];   // Menu function buffer
  u8 active;                      // Current menu index in buffer
} MENU;

extern MENU infoMenu;

typedef struct
{
  bool waiting;                        //Whether wait for Marlin's response
  bool responseReceived[_USART_CNT];   //Whether receive Marlin's response or get Gcode by other UART(ESP3D/OctoPrint)
  bool connected;                      //Whether have connected to Marlin
  bool jobInProgress;                  //Whether the host is busy in printing execution. ( USB serial printing and GCODE print from onboard)
} HOST;

extern HOST infoHost;

#endif
