#ifndef _BOOT_H_
#define _BOOT_H_

#include "variants.h"
#include "stdbool.h"

#define W25QXX_SECTOR_SIZE (0x1000)   // 4096-4K

//address in spiflash W25Qxx
#define LOGO_ADDR 0x0
#define WORD_UNICODE 0x4B000                        // unicode (+0x480000 4.5M)
#define BYTE_ASCII_ADDR (WORD_UNICODE + 0x480000)   // ascii (+0x1000 4K)
//#define BYTE_RESERVE_ADDR      0x710000
#define ICON_ADDR(num) ((num)*0x5000 + BYTE_ASCII_ADDR + 0x1000)
#define INFOBOX_ADDR (ICON_ADDR(ICON_PREVIEW) + 0x5000)   // total byte size 0xA7F8
#define SMALL_ICON_START_ADDR (INFOBOX_ADDR + 0xA7F8)
#define SMALL_ICON_ADDR(num) ((num)*0x1000 + SMALL_ICON_START_ADDR)

#define BMP (1 << 1)
#define FONT (1 << 2)

#define BMP_ROOT_DIR "0:" ROOT_DIR "/bmp"
#define FONT_ROOT_DIR "0:" ROOT_DIR "/font"
#define TFT_RESET_FILE "0:reset.txt"

enum {
  ICON_MOVE = 0,
  ICON_HOME,
  ICON_CNC,
  ICON_ROUTER,
  ICON_SETTINGS,
  ICON_JOBSETUP,
  ICON_INC,
  ICON_DEC,
  ICON_SPINDLE,
  ICON_STOP,
  ICON_BACK,
  ICON_X_INC,
  ICON_Y_INC,
  ICON_Z_INC,
  ICON_001_MM,
  ICON_01_MM,
  ICON_1_MM,
  ICON_10_MM,
  ICON_100_MM,
  ICON_X_DEC,
  ICON_Y_DEC,
  ICON_Z_DEC,
  ICON_X_HOME,
  ICON_Y_HOME,
  ICON_Z_HOME,
  ICON_FOLDER,
  ICON_FILE,
  ICON_PAGE_UP,
  ICON_PAGE_DOWN,
  ICON_PAUSE,
  ICON_RESUME,
  ICON_SLOW_SPEED,
  ICON_NORMAL_SPEED,
  ICON_FAST_SPEED,
  ICON_E_1_MM,
  ICON_E_5_MM,
  ICON_E_10_MM,
  ICON_ROUTER_ON,
  ICON_CHANGE_BIT,
  ICON_ROTATE_UI,
  ICON_LANGUAGE,
  ICON_TOUCHSCREEN_ADJUST,
  ICON_MORE,
  ICON_SCREEN_INFO,
  ICON_BKCOLOR,
  ICON_FONTCOLOR,
  ICON_DISCONNECT,
  ICON_BAUDRATE,
  ICON_PERCENTAGE,
  ICON_BABYSTEP,
  ICON_BSD_SOURCE,
  ICON_SD_SOURCE,
  ICON_U_DISK,
  ICON_DISABLE_XY,
  ICON_DISABLE_Z,
  ICON_POINT_3,
  ICON_POINT_4,
  ICON_MARLIN,
  ICON_BIGTREE,
  ICON_GCODE,
  ICON_ROUTER_OFF,
  ICON_ZERO_X,
  ICON_ZERO_Y,
  ICON_ZERO_Z,
  ICON_ZERO_ALL,
  ICON_SCREEN_SETTINGS,
  ICON_MACHINE_SETTINGS,
  ICON_FEATURE_SETTINGS,
  ICON_EEPROM_SAVE,
  ICON_SOUND_OFF,
  ICON_SHUT_DOWN,
  ICON_RGB_SETTINGS,
  ICON_RGB_RED,
  ICON_RGB_GREEN,
  ICON_RGB_BLUE,
  ICON_RGB_WHITE,
  ICON_RGB_OFF,
  ICON_CUSTOM,
  ICON_CUSTOM_0,
  ICON_CUSTOM_1,
  ICON_CUSTOM_2,
  ICON_CUSTOM_3,
  ICON_CUSTOM_4,
  ICON_CUSTOM_5,
  ICON_CUSTOM_6,
  ICON_HOME_MOVE,
  ICON_SOUND_ON,
  ICON_MAINMENU,

  //add new icons above this line  only
  //  ICON_RESERVE

  // Preview should be in the last place before ICON_BACKGROUND to save flash storage space
  ICON_PREVIEW,
  // Back ground sign
  ICON_BACKGROUND
};

typedef union {
  u16 color;
  struct {
    u16 b : 5;
    u16 g : 6;
    u16 r : 5;
  } RGB;
} GUI_COLOR;

void scanUpdates(void);
bool bmpDecode(char *bmp, u32 addr);

#endif
