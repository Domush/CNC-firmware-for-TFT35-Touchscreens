#include "includes.h"

// LCD init functions
#include "lcd.h"
#include "Hal/LCD_Init.h"
#include "lcd_dma.h"
#include "GUI.h"

// Multi-language support
#include "Language/Language.h"
#include "Language/utf8_decode.h"

// Chip specific includes
#include "Serial.h"
#include "usart.h"
#include "GPIO_Init.h"
#include "spi.h"
#include "spi_slave.h"
#include "sw_spi.h"

// USB drive support (select TFT models)
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"

// SD card support
#ifdef SD_SPI_SUPPORT
  #include "sd.h"
#else
  #include "sdio_sdcard.h"
#endif

#include "Hal/w25qxx.h"
#include "Hal/xpt2046.h"

// UI handling
#include "LCD_Encoder.h"
#include "ST7920_Simulator.h"
#include "ui_draw.h"
#include "touch_process.h"
#include "ledcolor.h"
#include "emulationToggle.h"

// File handling
#include "Fatfs/ff.h"
#include "Vfs/vfs.h"
#include "Fatfs/myfatfs.h"
#include "list_item.h"
#include "System/flashStorage.h"

// Gcode processing
#include "Gcode/gcodeSender.h"
#include "Gcode/gcodeRequests.h"
#include "Gcode/gcodeFileHandler.h"
#include "Gcode/gcodeResponseHandler.h"

// Base API functions
#include "API/coordinate.h"
#include "API/gantry.h"
#include "API/extendedFeatures.h"
#include "API/menu.h"

// Timing functions
#include "System/delay.h"
#include "System/os_timer.h"
#include "System/boot.h"

// Menus
#include "Menu/menuSettingsHome.h"
#include "Menu/menuSettingsMisc.h"
#include "Menu/menuSettingsFeatures.h"
#include "Menu/menuSettingsTFT.h"
#include "Menu/menuSettingsTMC.h"
#include "Menu/menuPower.h"
#include "Menu/menuPopup.h"
#include "Menu/menuTerminal.h"
#include "Menu/menuMove.h"
#include "Menu/menuHoming.h"
#include "Menu/menuRouter.h"
#include "Menu/menuJobSetup.h"
#include "Menu/menuFileSelect.h"
#include "Menu/menuJobStatus.h"
#include "Menu/menuPause.h"
#include "Menu/menuBabystepping.h"
#include "Menu/menuSpeed.h"
#include "Menu/menuMain.h"  // Home screen
