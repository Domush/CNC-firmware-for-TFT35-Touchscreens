#include "Language.h"
#include "includes.h"
#include "language_en.h"
#include "language_de.h"
#include "language_es.h"
#include "language_fr.h"

const char *const en_pack[LABEL_NUM] = {
    EN_LANGUAGE,
    EN_MOVE,
    EN_HOME,
    EN_CNC,
    EN_ROUTER,
    EN_SETTINGS,
    EN_SCREEN_SETTINGS,
    EN_MACHINE_SETTINGS,
    EN_FEATURE_SETTINGS,
    EN_SOUND_OFF,
    EN_SOUND_ON,
    EN_SHUT_DOWN,
    EN_RGB_SETTINGS,
    EN_RGB_OFF,
    EN_GCODE,
    EN_CUSTOM,
    EN_LEVELING,
    EN_DISABLE_XY,
    EN_DISABLE_Z,
    EN_POINT_3,
    EN_POINT_4,
    EN_ABL,
    EN_ROUTER_OFF,
    EN_ZERO_X,
    EN_ZERO_Y,
    EN_ZERO_Z,
    EN_ZERO_ALL,
    EN_EEPROM_SAVE,
    EN_INC,
    EN_DEC,
    EN_SPINDLE,
    EN_STOP,
    EN_BACK,
    EN_PAGE_UP,
    EN_PAGE_DOWN,
    EN_PAUSE,
    EN_RESUME,
    EN_SLOW_SPEED,
    EN_NORMAL_SPEED,
    EN_FAST_SPEED,
    EN_ROUTER_ON,
    EN_CHANGE_BIT,
    EN_ROTATE_UI,
    EN_TOUCHSCREEN_ADJUST,
    EN_MORE,
    EN_SCREEN_INFO,
    EN_WHITE,
    EN_BLACK,
    EN_BLUE,
    EN_RED,
    EN_GREEN,
    EN_CYAN,
    EN_YELLOW,
    EN_BROWN,
    EN_GRAY,
    EN_ORANGE,
    EN_INDIGO,
    EN_VIOLET,
    EN_DISCONNECT,
    EN_BAUDRATE_115200,
    EN_BAUDRATE_250000,
    EN_RUNOUT_OFF,
    EN_RUNOUT_ON,
    EN_SMART_RUNOUT_ON,
    EN_PERCENTAGE,
    EN_BABYSTEP,
    EN_PERCENTAGE_SPEED,
    EN_VALUE_ZERO,
    EN_X_INC,
    EN_Y_INC,
    EN_Z_INC,
    EN_X_DEC,
    EN_Y_DEC,
    EN_Z_DEC,
    EN_X_HOME,
    EN_Y_HOME,
    EN_Z_HOME,
    EN_COORD_1,
    EN_COORD_2,
    EN_COORD_3,
    EN_001_MM,
    EN_01_MM,
    EN_1_MM,
    EN_5_MM,
    EN_10_MM,
    EN_100_MM,
    EN_1_PERCENT,
    EN_5_PERCENT,
    EN_10_PERCENT,
    EN_READY,
    EN_BUSY,
    EN_UNCONNECTED,
    EN_DISCONNECT_INFO,
    EN_LOADING,
    EN_POWER_FAILED,
    EN_CONTINUE,
    EN_CANCEL,
    EN_ADJUST_TITLE,
    EN_ADJUST_INFO,
    EN_ADJUST_OK,
    EN_ADJUST_FAILED,
    EN_WARNING,
    EN_STOP_CNC,
    EN_CONFIRM,
    EN_TFTSD,
    EN_READ_TFTSD_ERROR,
    EN_TFTSD_INSERTED,
    EN_TFTSD_REMOVED,
    EN_U_DISK,
    EN_READ_U_DISK_ERROR,
    EN_U_DISK_INSERTED,
    EN_U_DISK_REMOVED,
    EN_ONBOARDSD,
    EN_READ_ONBOARDSD_ERROR,
    EN_FILAMENT_RUNOUT,
    EN_IS_PAUSE,
    EN_AUTO_SHUT_DOWN,
    EN_MANUAL_SHUT_DOWN,
    EN_UNIFIEDMOVE,
    EN_COOLDOWN,
    EN_EMERGENCYSTOP,
    EN_TOUCH_TO_EXIT,
    EN_MAINMENU,
    EN_WAIT_TEMP_SHUT_DOWN,
    EN_FORCE_SHUT_DOWN,
    EN_SHUTTING_DOWN,
    EN_ON,
    EN_OFF,
    EN_AUTO,
    EN_SMART,
    EN_TERMINAL_ACK,
    EN_INVERT_YAXIS,
    EN_INVERT_ZAXIS,
    EN_MOVE_SPEED,
    EN_ROUTER_CONTROL,
    EN_DISABLED,
    EN_M3M5,
    EN_FAN0,
    EN_KNOB_LED,
    EN_M0_PAUSE,
    EN_SEND_START_GCODE,
    EN_SEND_END_GCODE,
    EN_PERSISTANT_STATUS_INFO,
    EN_FILE_LISTMODE,
};

const char *const de_pack[LABEL_NUM] = {
    DE_LANGUAGE,
    DE_MOVE,
    DE_HOME,
    DE_CNC,
    DE_ROUTER,
    DE_SETTINGS,
    DE_SCREEN_SETTINGS,
    DE_MACHINE_SETTINGS,
    DE_FEATURE_SETTINGS,
    DE_SOUND_OFF,
    DE_SOUND_ON,
    DE_SHUT_DOWN,
    DE_RGB_SETTINGS,
    DE_RGB_OFF,
    DE_GCODE,
    DE_CUSTOM,
    DE_LEVELING,
    DE_DISABLE_XY,
    DE_DISABLE_Z,
    DE_POINT_3,
    DE_POINT_4,
    DE_ABL,
    DE_ROUTER_OFF,
    DE_ZERO_X,
    DE_ZERO_Y,
    DE_ZERO_Z,
    DE_ZERO_ALL,
    DE_EEPROM_SAVE,
    DE_INC,
    DE_DEC,
    DE_SPINDLE,
    DE_STOP,
    DE_BACK,
    DE_PAGE_UP,
    DE_PAGE_DOWN,
    DE_PAUSE,
    DE_RESUME,
    DE_SLOW_SPEED,
    DE_NORMAL_SPEED,
    DE_FAST_SPEED,
    DE_ROUTER_ON,
    DE_CHANGE_BIT,
    DE_ROTATE_UI,
    DE_TOUCHSCREEN_ADJUST,
    DE_MORE,
    DE_SCREEN_INFO,
    DE_WHITE,
    DE_BLACK,
    DE_BLUE,
    DE_RED,
    DE_GREEN,
    DE_CYAN,
    DE_YELLOW,
    DE_BROWN,
    DE_GRAY,
    DE_ORANGE,
    DE_INDIGO,
    DE_VIOLET,
    DE_DISCONNECT,
    DE_BAUDRATE_115200,
    DE_BAUDRATE_250000,
    DE_RUNOUT_OFF,
    DE_RUNOUT_ON,
    DE_SMART_RUNOUT_ON,
    DE_PERCENTAGE,
    DE_BABYSTEP,
    DE_PERCENTAGE_SPEED,
    DE_VALUE_ZERO,
    DE_X_INC,
    DE_Y_INC,
    DE_Z_INC,
    DE_X_DEC,
    DE_Y_DEC,
    DE_Z_DEC,
    DE_X_HOME,
    DE_Y_HOME,
    DE_Z_HOME,
    DE_COORD_1,
    DE_COORD_2,
    DE_COORD_3,
    DE_001_MM,
    DE_01_MM,
    DE_1_MM,
    DE_5_MM,
    DE_10_MM,
    DE_100_MM,
    DE_1_PERCENT,
    DE_5_PERCENT,
    DE_10_PERCENT,
    DE_READY,
    DE_BUSY,
    DE_UNCONNECTED,
    DE_DISCONNECT_INFO,
    DE_LOADING,
    DE_POWER_FAILED,
    DE_CONTINUE,
    DE_CANCEL,
    DE_ADJUST_TITLE,
    DE_ADJUST_INFO,
    DE_ADJUST_OK,
    DE_ADJUST_FAILED,
    DE_WARNING,
    DE_STOP_CNC,
    DE_CONFIRM,
    DE_TFTSD,
    DE_READ_TFTSD_ERROR,
    DE_TFTSD_INSERTED,
    DE_TFTSD_REMOVED,
    DE_U_DISK,
    DE_READ_U_DISK_ERROR,
    DE_U_DISK_INSERTED,
    DE_U_DISK_REMOVED,
    DE_ONBOARDSD,
    DE_READ_ONBOARDSD_ERROR,
    DE_FILAMENT_RUNOUT,
    DE_IS_PAUSE,
    DE_AUTO_SHUT_DOWN,
    DE_MANUAL_SHUT_DOWN,
    DE_UNIFIEDMOVE,
    DE_COOLDOWN,
    DE_EMERGENCYSTOP,
    DE_TOUCH_TO_EXIT,
    DE_MAINMENU,
    DE_WAIT_TEMP_SHUT_DOWN,
    DE_FORCE_SHUT_DOWN,
    DE_SHUTTING_DOWN,
    DE_ON,
    DE_OFF,
    DE_AUTO,
    DE_SMART,
    DE_TERMINAL_ACK,
    DE_INVERT_YAXIS,
    DE_INVERT_ZAXIS,
    DE_MOVE_SPEED,
    DE_ROUTER_CONTROL,
    DE_DISABLED,
    DE_M3M5,
    DE_FAN0,
    DE_KNOB_LED,
    DE_M0_PAUSE,
    DE_SEND_START_GCODE,
    DE_SEND_END_GCODE,
    DE_PERSISTANT_STATUS_INFO,
    DE_FILE_LISTMODE,

};

const char *const es_pack[LABEL_NUM] = {
    ES_LANGUAGE,
    ES_MOVE,
    ES_HOME,
    ES_CNC,
    ES_ROUTER,
    ES_SETTINGS,
    ES_SCREEN_SETTINGS,
    ES_MACHINE_SETTINGS,
    ES_FEATURE_SETTINGS,
    ES_SOUND_OFF,
    ES_SOUND_ON,
    ES_SHUT_DOWN,
    ES_RGB_SETTINGS,
    ES_RGB_OFF,
    ES_GCODE,
    ES_CUSTOM,
    ES_LEVELING,
    ES_DISABLE_XY,
    ES_DISABLE_Z,
    ES_POINT_3,
    ES_POINT_4,
    ES_ABL,
    ES_ROUTER_OFF,
    ES_ZERO_X,
    ES_ZERO_Y,
    ES_ZERO_Z,
    ES_ZERO_ALL,
    ES_EEPROM_SAVE,
    ES_INC,
    ES_DEC,
    ES_SPINDLE,
    ES_STOP,
    ES_BACK,
    ES_PAGE_UP,
    ES_PAGE_DOWN,
    ES_PAUSE,
    ES_RESUME,
    ES_SLOW_SPEED,
    ES_NORMAL_SPEED,
    ES_FAST_SPEED,
    ES_ROUTER_ON,
    ES_CHANGE_BIT,
    ES_ROTATE_UI,
    ES_TOUCHSCREEN_ADJUST,
    ES_MORE,
    ES_SCREEN_INFO,
    ES_WHITE,
    ES_BLACK,
    ES_BLUE,
    ES_RED,
    ES_GREEN,
    ES_CYAN,
    ES_YELLOW,
    ES_BROWN,
    ES_GRAY,
    ES_ORANGE,
    ES_INDIGO,
    ES_VIOLET,
    ES_DISCONNECT,
    ES_BAUDRATE_115200,
    ES_BAUDRATE_250000,
    ES_RUNOUT_OFF,
    ES_RUNOUT_ON,
    ES_SMART_RUNOUT_ON,
    ES_PERCENTAGE,
    ES_BABYSTEP,
    ES_PERCENTAGE_SPEED,
    ES_VALUE_ZERO,
    ES_X_INC,
    ES_Y_INC,
    ES_Z_INC,
    ES_X_DEC,
    ES_Y_DEC,
    ES_Z_DEC,
    ES_X_HOME,
    ES_Y_HOME,
    ES_Z_HOME,
    ES_COORD_1,
    ES_COORD_2,
    ES_COORD_3,
    ES_001_MM,
    ES_01_MM,
    ES_1_MM,
    ES_5_MM,
    ES_10_MM,
    ES_100_MM,
    ES_1_PERCENT,
    ES_5_PERCENT,
    ES_10_PERCENT,
    ES_READY,
    ES_BUSY,
    ES_UNCONNECTED,
    ES_DISCONNECT_INFO,
    ES_LOADING,
    ES_POWER_FAILED,
    ES_CONTINUE,
    ES_CANCEL,
    ES_ADJUST_TITLE,
    ES_ADJUST_INFO,
    ES_ADJUST_OK,
    ES_ADJUST_FAILED,
    ES_WARNING,
    ES_STOP_CNC,
    ES_CONFIRM,
    ES_TFTSD,
    ES_READ_TFTSD_ERROR,
    ES_TFTSD_INSERTED,
    ES_TFTSD_REMOVED,
    ES_U_DISK,
    ES_READ_U_DISK_ERROR,
    ES_U_DISK_INSERTED,
    ES_U_DISK_REMOVED,
    ES_ONBOARDSD,
    ES_READ_ONBOARDSD_ERROR,
    ES_FILAMENT_RUNOUT,
    ES_IS_PAUSE,
    ES_AUTO_SHUT_DOWN,
    ES_MANUAL_SHUT_DOWN,
    ES_UNIFIEDMOVE,
    ES_COOLDOWN,
    ES_EMERGENCYSTOP,
    ES_TOUCH_TO_EXIT,
    ES_MAINMENU,
    ES_WAIT_TEMP_SHUT_DOWN,
    ES_FORCE_SHUT_DOWN,
    ES_SHUTTING_DOWN,
    ES_ON,
    ES_OFF,
    ES_AUTO,
    ES_SMART,
    ES_TERMINAL_ACK,
    ES_INVERT_YAXIS,
    ES_INVERT_ZAXIS,
    ES_MOVE_SPEED,
    ES_ROUTER_CONTROL,
    ES_DISABLED,
    ES_M3M5,
    ES_FAN0,
    ES_KNOB_LED,
    ES_M0_PAUSE,
    ES_SEND_START_GCODE,
    ES_SEND_END_GCODE,
    ES_PERSISTANT_STATUS_INFO,
    ES_FILE_LISTMODE,

};

const char *const fr_pack[LABEL_NUM] = {
    FR_LANGUAGE,
    FR_MOVE,
    FR_HOME,
    FR_CNC,
    FR_ROUTER,
    FR_SETTINGS,
    FR_SCREEN_SETTINGS,
    FR_MACHINE_SETTINGS,
    FR_FEATURE_SETTINGS,
    FR_SOUND_OFF,
    FR_SOUND_ON,
    FR_SHUT_DOWN,
    FR_RGB_SETTINGS,
    FR_RGB_OFF,
    FR_GCODE,
    FR_CUSTOM,
    FR_LEVELING,
    FR_DISABLE_XY,
    FR_DISABLE_Z,
    FR_POINT_3,
    FR_POINT_4,
    FR_ABL,
    FR_ROUTER_OFF,
    FR_ZERO_X,
    FR_ZERO_Y,
    FR_ZERO_Z,
    FR_ZERO_ALL,
    FR_EEPROM_SAVE,
    FR_INC,
    FR_DEC,
    FR_SPINDLE,
    FR_STOP,
    FR_BACK,
    FR_PAGE_UP,
    FR_PAGE_DOWN,
    FR_PAUSE,
    FR_RESUME,
    FR_SLOW_SPEED,
    FR_NORMAL_SPEED,
    FR_FAST_SPEED,
    FR_ROUTER_ON,
    FR_CHANGE_BIT,
    FR_ROTATE_UI,
    FR_TOUCHSCREEN_ADJUST,
    FR_MORE,
    FR_SCREEN_INFO,
    FR_WHITE,
    FR_BLACK,
    FR_BLUE,
    FR_RED,
    FR_GREEN,
    FR_CYAN,
    FR_YELLOW,
    FR_BROWN,
    FR_GRAY,
    FR_ORANGE,
    FR_INDIGO,
    FR_VIOLET,
    FR_DISCONNECT,
    FR_BAUDRATE_115200,
    FR_BAUDRATE_250000,
    FR_RUNOUT_OFF,
    FR_RUNOUT_ON,
    FR_SMART_RUNOUT_ON,
    FR_PERCENTAGE,
    FR_BABYSTEP,
    FR_PERCENTAGE_SPEED,
    FR_VALUE_ZERO,
    FR_X_INC,
    FR_Y_INC,
    FR_Z_INC,
    FR_X_DEC,
    FR_Y_DEC,
    FR_Z_DEC,
    FR_X_HOME,
    FR_Y_HOME,
    FR_Z_HOME,
    FR_COORD_1,
    FR_COORD_2,
    FR_COORD_3,
    FR_001_MM,
    FR_01_MM,
    FR_1_MM,
    FR_5_MM,
    FR_10_MM,
    FR_100_MM,
    FR_1_PERCENT,
    FR_5_PERCENT,
    FR_10_PERCENT,
    FR_READY,
    FR_BUSY,
    FR_UNCONNECTED,
    FR_DISCONNECT_INFO,
    FR_LOADING,
    FR_POWER_FAILED,
    FR_CONTINUE,
    FR_CANCEL,
    FR_ADJUST_TITLE,
    FR_ADJUST_INFO,
    FR_ADJUST_OK,
    FR_ADJUST_FAILED,
    FR_WARNING,
    FR_STOP_CNC,
    FR_CONFIRM,
    FR_TFTSD,
    FR_READ_TFTSD_ERROR,
    FR_TFTSD_INSERTED,
    FR_TFTSD_REMOVED,
    FR_U_DISK,
    FR_READ_U_DISK_ERROR,
    FR_U_DISK_INSERTED,
    FR_U_DISK_REMOVED,
    FR_ONBOARDSD,
    FR_READ_ONBOARDSD_ERROR,
    FR_FILAMENT_RUNOUT,
    FR_IS_PAUSE,
    FR_AUTO_SHUT_DOWN,
    FR_MANUAL_SHUT_DOWN,
    FR_UNIFIEDMOVE,
    FR_COOLDOWN,
    FR_EMERGENCYSTOP,
    FR_TOUCH_TO_EXIT,
    FR_MAINMENU,
    FR_WAIT_TEMP_SHUT_DOWN,
    FR_FORCE_SHUT_DOWN,
    FR_SHUTTING_DOWN,
    FR_ON,
    FR_OFF,
    FR_AUTO,
    FR_SMART,
    FR_TERMINAL_ACK,
    FR_INVERT_YAXIS,
    FR_INVERT_ZAXIS,
    FR_MOVE_SPEED,
    FR_ROUTER_CONTROL,
    FR_DISABLED,
    FR_M3M5,
    FR_FAN0,
    FR_KNOB_LED,
    FR_M0_PAUSE,
    FR_SEND_START_GCODE,
    FR_SEND_END_GCODE,
    FR_PERSISTANT_STATUS_INFO,
    FR_FILE_LISTMODE,

};

u8 *textSelect(u8 sel) {
  switch (infoSettings.language) {
    case ENGLISH:
      return (u8 *)en_pack[sel];
    case GERMAN:
      return (u8 *)de_pack[sel];
    case SPAIN:
      return (u8 *)es_pack[sel];
    case FRENCH:
      return (u8 *)fr_pack[sel];

    default:
      return NULL;
  }
}
