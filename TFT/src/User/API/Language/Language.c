#include "Language.h"
#include "includes.h"
#include "language_en.h"
#include "language_cn.h"
#include "language_ru.h"
#include "language_jp.h"
#include "language_de.h"
#include "language_am.h"
#include "language_cz.h"
#include "language_es.h"
#include "language_fr.h"
#include "language_pt.h"
#include "language_it.h"
#include "language_pl.h"
#include "language_sk.h"
#include "language_du.h"

const char *const en_pack[LABEL_NUM]={
  EN_LANGUAGE,
  EN_HEAT,
  EN_MOVE,
  EN_HOME,
  EN_PRINT,
  EN_EXTRUDE,
  EN_FAN,
  EN_SETTINGS,
  EN_SCREEN_SETTINGS,
  EN_MACHINE_SETTINGS,
  EN_FEATURE_SETTINGS,
  EN_SILENT_ON,
  EN_SILENT_OFF,
  EN_SHUT_DOWN,
  EN_RGB_SETTINGS,
  EN_RGB_OFF,
  EN_GCODE,
  EN_CUSTOM,
  EN_LEVELING,
  EN_POINT_1,
  EN_POINT_2,
  EN_POINT_3,
  EN_POINT_4,
  EN_ABL,
  EN_BLTOUCH,
  EN_BLTOUCH_TEST,
  EN_BLTOUCH_DEPLOY,
  EN_BLTOUCH_STOW,
  EN_BLTOUCH_REPEAT,
  EN_PROBE_OFFSET,
  EN_EEPROM_SAVE,
  EN_INC,
  EN_DEC,
  EN_NOZZLE,
  EN_BED,
  EN_STOP,
  EN_BACK,
  EN_PAGE_UP,
  EN_PAGE_DOWN,
  EN_PAUSE,
  EN_RESUME,
  EN_LOAD,
  EN_UNLOAD,
  EN_SLOW_SPEED,
  EN_NORMAL_SPEED,
  EN_FAST_SPEED,
  EN_FAN_FULL_SPEED,
  EN_FAN_HALF_SPEED,
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
  EN_PERCENTAGE_FLOW,
  EN_VALUE_ZERO,
  EN_1_DEGREE,
  EN_5_DEGREE,
  EN_10_DEGREE,
  EN_X_INC,
  EN_Y_INC,
  EN_Z_INC,
  EN_X_DEC,
  EN_Y_DEC,
  EN_Z_DEC,
  EN_X_HOME,
  EN_Y_HOME,
  EN_Z_HOME,
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
  EN_CANNEL,
  EN_ADJUST_TITLE,
  EN_ADJUST_INFO,
  EN_ADJUST_OK,
  EN_ADJUST_FAILED,
  EN_WARNING,
  EN_STOP_PRINT,
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
  EN_PREHEAT,
  EN_PREHEAT_BOTH,
  EN_PREHEAT_PLA,
  EN_PREHEAT_PETG,
  EN_PREHEAT_ABS,
  EN_PREHEAT_CUSTOM1,
  EN_PREHEAT_CUSTOM2,
  EN_IS_PAUSE,
  EN_AUTO_SHUT_DOWN,
  EN_MANUAL_SHUT_DOWN,
  EN_UNIFIEDMOVE,
  EN_UNIFIEDHEAT,
  EN_COOLDOWN,
  EN_EMERGENCYSTOP,
  EN_TOUCH_TO_EXIT,
  EN_MAINMENU,
  EN_WAIT_TEMP_SHUT_DOWN,
  EN_FORCE_SHUT_DOWN,
  EN_SHUTTING_DOWN,
  EN_PARAMETER_SETTING,
  EN_ON,
  EN_OFF,
  EN_AUTO,
  EN_SMART,
  EN_TERMINAL_ACK,
  EN_INVERT_YAXIS,
  EN_INVERT_ZAXIS,
  EN_MOVE_SPEED,
  EN_KNOB_LED,
  EN_M0_PAUSE,
};

const char *const cn_pack[LABEL_NUM]={
  CN_LANGUAGE,
  CN_HEAT,
  CN_MOVE,
  CN_HOME,
  CN_PRINT,
  CN_EXTRUDE,
  CN_FAN,
  CN_SETTINGS,
  CN_SCREEN_SETTINGS,
  CN_MACHINE_SETTINGS,
  CN_FEATURE_SETTINGS,
  CN_SILENT_ON,
  CN_SILENT_OFF,
  CN_SHUT_DOWN,
  CN_RGB_SETTINGS,
  CN_RGB_OFF,
  CN_GCODE,
  CN_CUSTOM,
  CN_LEVELING,
  CN_POINT_1,
  CN_POINT_2,
  CN_POINT_3,
  CN_POINT_4,
  CN_ABL,
  CN_BLTOUCH,
  CN_BLTOUCH_TEST,
  CN_BLTOUCH_DEPLOY,
  CN_BLTOUCH_STOW,
  CN_BLTOUCH_REPEAT,
  CN_PROBE_OFFSET,
  CN_EEPROM_SAVE,
  CN_INC,
  CN_DEC,
  CN_NOZZLE,
  CN_BED,
  CN_STOP,
  CN_BACK,
  CN_PAGE_UP,
  CN_PAGE_DOWN,
  CN_PAUSE,
  CN_RESUME,
  CN_LOAD,
  CN_UNLOAD,
  CN_SLOW_SPEED,
  CN_NORMAL_SPEED,
  CN_FAST_SPEED,
  CN_FAN_FULL_SPEED,
  CN_FAN_HALF_SPEED,
  CN_ROTATE_UI,
  CN_TOUCHSCREEN_ADJUST,
  CN_MORE,
  CN_SCREEN_INFO,
  CN_WHITE,
  CN_BLACK,
  CN_BLUE,
  CN_RED,
  CN_GREEN,
  CN_CYAN,
  CN_YELLOW,
  CN_BROWN,
  CN_GRAY,
  CN_ORANGE,
  CN_INDIGO,
  CN_VIOLET,
  CN_DISCONNECT,
  CN_BAUDRATE_115200,
  CN_BAUDRATE_250000,
  CN_RUNOUT_OFF,
  CN_RUNOUT_ON,
  CN_SMART_RUNOUT_ON,
  CN_PERCENTAGE,
  CN_BABYSTEP,
  CN_PERCENTAGE_SPEED,
  CN_PERCENTAGE_FLOW,
  CN_VALUE_ZERO,
  CN_1_DEGREE,
  CN_5_DEGREE,
  CN_10_DEGREE,
  CN_X_INC,
  CN_Y_INC,
  CN_Z_INC,
  CN_X_DEC,
  CN_Y_DEC,
  CN_Z_DEC,
  CN_X_HOME,
  CN_Y_HOME,
  CN_Z_HOME,
  CN_001_MM,
  CN_01_MM,
  CN_1_MM,
  CN_5_MM,
  CN_10_MM,
  CN_1_PERCENT,
  CN_5_PERCENT,
  CN_10_PERCENT,
  CN_READY,
  CN_BUSY,
  CN_UNCONNECTED,
  CN_DISCONNECT_INFO,
  CN_LOADING,
  CN_POWER_FAILED,
  CN_CONTINUE,
  CN_CANNEL,
  CN_ADJUST_TITLE,
  CN_ADJUST_INFO,
  CN_ADJUST_OK,
  CN_ADJUST_FAILED,
  CN_WARNING,
  CN_STOP_PRINT,
  CN_CONFIRM,
  CN_TFTSD,
  CN_READ_TFTSD_ERROR,
  CN_TFTSD_INSERTED,
  CN_TFTSD_REMOVED,
  CN_U_DISK,
  CN_READ_U_DISK_ERROR,
  CN_U_DISK_INSERTED,
  CN_U_DISK_REMOVED,
  CN_ONBOARDSD,
  CN_READ_ONBOARDSD_ERROR,
  CN_FILAMENT_RUNOUT,
  CN_PREHEAT,
  CN_PREHEAT_BOTH,
  CN_PREHEAT_PLA,
  CN_PREHEAT_PETG,
  CN_PREHEAT_ABS,
  CN_PREHEAT_CUSTOM1,
  CN_PREHEAT_CUSTOM2,
  CN_IS_PAUSE,
  CN_AUTO_SHUT_DOWN,
  CN_MANUAL_SHUT_DOWN,
  CN_UNIFIEDMOVE,
  CN_UNIFIEDHEAT,
  CN_COOLDOWN,
  CN_EMERGENCYSTOP,
  CN_TOUCH_TO_EXIT,
  CN_MAINMENU,
  CN_WAIT_TEMP_SHUT_DOWN,
  CN_FORCE_SHUT_DOWN,
  CN_SHUTTING_DOWN,
  CN_PARAMETER_SETTING,
  CN_ON,
  CN_OFF,
  CN_AUTO,
  CN_SMART,
  CN_TERMINAL_ACK,
  CN_INVERT_YAXIS,
  CN_INVERT_ZAXIS,
  CN_MOVE_SPEED,
  CN_KNOB_LED,
  CN_M0_PAUSE,
};

const char *const ru_pack[LABEL_NUM]={
  RU_LANGUAGE,
  RU_HEAT,
  RU_MOVE,
  RU_HOME,
  RU_PRINT,
  RU_EXTRUDE,
  RU_FAN,
  RU_SETTINGS,
  RU_SCREEN_SETTINGS,
  RU_MACHINE_SETTINGS,
  RU_FEATURE_SETTINGS,
  RU_SILENT_ON,
  RU_SILENT_OFF,
  RU_SHUT_DOWN,
  RU_RGB_SETTINGS,
  RU_RGB_OFF,
  RU_GCODE,
  RU_CUSTOM,
  RU_LEVELING,
  RU_POINT_1,
  RU_POINT_2,
  RU_POINT_3,
  RU_POINT_4,
  RU_ABL,
  RU_BLTOUCH,
  RU_BLTOUCH_TEST,
  RU_BLTOUCH_DEPLOY,
  RU_BLTOUCH_STOW,
  RU_BLTOUCH_REPEAT,
  RU_PROBE_OFFSET,
  RU_EEPROM_SAVE,
  RU_INC,
  RU_DEC,
  RU_NOZZLE,
  RU_BED,
  RU_STOP,
  RU_BACK,
  RU_PAGE_UP,
  RU_PAGE_DOWN,
  RU_PAUSE,
  RU_RESUME,
  RU_LOAD,
  RU_UNLOAD,
  RU_SLOW_SPEED,
  RU_NORMAL_SPEED,
  RU_FAST_SPEED,
  RU_FAN_FULL_SPEED,
  RU_FAN_HALF_SPEED,
  RU_ROTATE_UI,
  RU_TOUCHSCREEN_ADJUST,
  RU_MORE,
  RU_SCREEN_INFO,
  RU_WHITE,
  RU_BLACK,
  RU_BLUE,
  RU_RED,
  RU_GREEN,
  RU_CYAN,
  RU_YELLOW,
  RU_BROWN,
  RU_GRAY,
  RU_ORANGE,
  RU_INDIGO,
  RU_VIOLET,
  RU_DISCONNECT,
  RU_BAUDRATE_115200,
  RU_BAUDRATE_250000,
  RU_RUNOUT_OFF,
  RU_RUNOUT_ON,
  RU_SMART_RUNOUT_ON,
  RU_PERCENTAGE,
  RU_BABYSTEP,
  RU_PERCENTAGE_SPEED,
  RU_PERCENTAGE_FLOW,
  RU_VALUE_ZERO,
  RU_1_DEGREE,
  RU_5_DEGREE,
  RU_10_DEGREE,
  RU_X_INC,
  RU_Y_INC,
  RU_Z_INC,
  RU_X_DEC,
  RU_Y_DEC,
  RU_Z_DEC,
  RU_X_HOME,
  RU_Y_HOME,
  RU_Z_HOME,
  RU_001_MM,
  RU_01_MM,
  RU_1_MM,
  RU_5_MM,
  RU_10_MM,
  RU_1_PERCENT,
  RU_5_PERCENT,
  RU_10_PERCENT,
  RU_READY,
  RU_BUSY,
  RU_UNCONNECTED,
  RU_DISCONNECT_INFO,
  RU_LOADING,
  RU_POWER_FAILED,
  RU_CONTINUE,
  RU_CANNEL,
  RU_ADJUST_TITLE,
  RU_ADJUST_INFO,
  RU_ADJUST_OK,
  RU_ADJUST_FAILED,
  RU_WARNING,
  RU_STOP_PRINT,
  RU_CONFIRM,
  RU_TFTSD,
  RU_READ_TFTSD_ERROR,
  RU_TFTSD_INSERTED,
  RU_TFTSD_REMOVED,
  RU_U_DISK,
  RU_READ_U_DISK_ERROR,
  RU_U_DISK_INSERTED,
  RU_U_DISK_REMOVED,
  RU_ONBOARDSD,
  RU_READ_ONBOARDSD_ERROR,
  RU_FILAMENT_RUNOUT,
  RU_PREHEAT,
  RU_PREHEAT_BOTH,
  RU_PREHEAT_PLA,
  RU_PREHEAT_PETG,
  RU_PREHEAT_ABS,
  RU_PREHEAT_CUSTOM1,
  RU_PREHEAT_CUSTOM2,
  RU_IS_PAUSE,
  RU_AUTO_SHUT_DOWN,
  RU_MANUAL_SHUT_DOWN,
  RU_UNIFIEDMOVE,
  RU_UNIFIEDHEAT,
  RU_COOLDOWN,
  RU_EMERGENCYSTOP,
  RU_TOUCH_TO_EXIT,
  RU_MAINMENU,
  RU_WAIT_TEMP_SHUT_DOWN,
  RU_FORCE_SHUT_DOWN,
  RU_SHUTTING_DOWN,
  RU_PARAMETER_SETTING,
  RU_ON,
  RU_OFF,
  RU_AUTO,
  RU_SMART,
  RU_TERMINAL_ACK,
  RU_INVERT_YAXIS,
  RU_INVERT_ZAXIS,
  RU_MOVE_SPEED,
  RU_KNOB_LED,
  RU_M0_PAUSE,
};

const char *const jp_pack[LABEL_NUM]={
  JP_LANGUAGE,
  JP_HEAT,
  JP_MOVE,
  JP_HOME,
  JP_PRINT,
  JP_EXTRUDE,
  JP_FAN,
  JP_SETTINGS,
  JP_SCREEN_SETTINGS,
  JP_MACHINE_SETTINGS,
  JP_FEATURE_SETTINGS,
  JP_SILENT_ON,
  JP_SILENT_OFF,
  JP_SHUT_DOWN,
  JP_RGB_SETTINGS,
  JP_RGB_OFF,
  JP_GCODE,
  JP_CUSTOM,
  JP_LEVELING,
  JP_POINT_1,
  JP_POINT_2,
  JP_POINT_3,
  JP_POINT_4,
  JP_ABL,
  JP_BLTOUCH,
  JP_BLTOUCH_TEST,
  JP_BLTOUCH_DEPLOY,
  JP_BLTOUCH_STOW,
  JP_BLTOUCH_REPEAT,
  JP_PROBE_OFFSET,
  JP_EEPROM_SAVE,
  JP_INC,
  JP_DEC,
  JP_NOZZLE,
  JP_BED,
  JP_STOP,
  JP_BACK,
  JP_PAGE_UP,
  JP_PAGE_DOWN,
  JP_PAUSE,
  JP_RESUME,
  JP_LOAD,
  JP_UNLOAD,
  JP_SLOW_SPEED,
  JP_NORMAL_SPEED,
  JP_FAST_SPEED,
  JP_FAN_FULL_SPEED,
  JP_FAN_HALF_SPEED,
  JP_ROTATE_UI,
  JP_TOUCHSCREEN_ADJUST,
  JP_MORE,
  JP_SCREEN_INFO,
  JP_WHITE,
  JP_BLACK,
  JP_BLUE,
  JP_RED,
  JP_GREEN,
  JP_CYAN,
  JP_YELLOW,
  JP_BROWN,
  JP_GRAY,
  JP_ORANGE,
  JP_INDIGO,
  JP_VIOLET,
  JP_DISCONNECT,
  JP_BAUDRATE_115200,
  JP_BAUDRATE_250000,
  JP_RUNOUT_OFF,
  JP_RUNOUT_ON,
  JP_SMART_RUNOUT_ON,
  JP_PERCENTAGE,
  JP_BABYSTEP,
  JP_PERCENTAGE_SPEED,
  JP_PERCENTAGE_FLOW,
  JP_VALUE_ZERO,
  JP_1_DEGREE,
  JP_5_DEGREE,
  JP_10_DEGREE,
  JP_X_INC,
  JP_Y_INC,
  JP_Z_INC,
  JP_X_DEC,
  JP_Y_DEC,
  JP_Z_DEC,
  JP_X_HOME,
  JP_Y_HOME,
  JP_Z_HOME,
  JP_001_MM,
  JP_01_MM,
  JP_1_MM,
  JP_5_MM,
  JP_10_MM,
  JP_1_PERCENT,
  JP_5_PERCENT,
  JP_10_PERCENT,
  JP_READY,
  JP_BUSY,
  JP_UNCONNECTED,
  JP_DISCONNECT_INFO,
  JP_LOADING,
  JP_POWER_FAILED,
  JP_CONTINUE,
  JP_CANNEL,
  JP_ADJUST_TITLE,
  JP_ADJUST_INFO,
  JP_ADJUST_OK,
  JP_ADJUST_FAILED,
  JP_WARNING,
  JP_STOP_PRINT,
  JP_CONFIRM,
  JP_TFTSD,
  JP_READ_TFTSD_ERROR,
  JP_TFTSD_INSERTED,
  JP_TFTSD_REMOVED,
  JP_U_DISK,
  JP_READ_U_DISK_ERROR,
  JP_U_DISK_INSERTED,
  JP_U_DISK_REMOVED,
  JP_ONBOARDSD,
  JP_READ_ONBOARDSD_ERROR,
  JP_FILAMENT_RUNOUT,
  JP_PREHEAT,
  JP_PREHEAT_BOTH,
  JP_PREHEAT_PLA,
  JP_PREHEAT_PETG,
  JP_PREHEAT_ABS,
  JP_PREHEAT_CUSTOM1,
  JP_PREHEAT_CUSTOM2,
  JP_IS_PAUSE,
  JP_AUTO_SHUT_DOWN,
  JP_MANUAL_SHUT_DOWN,
  JP_UNIFIEDMOVE,
  JP_UNIFIEDHEAT,
  JP_COOLDOWN,
  JP_EMERGENCYSTOP,
  JP_TOUCH_TO_EXIT,
  JP_MAINMENU,
  JP_WAIT_TEMP_SHUT_DOWN,
  JP_FORCE_SHUT_DOWN,
  JP_SHUTTING_DOWN,
  JP_PARAMETER_SETTING,
  JP_ON,
  JP_OFF,
  JP_AUTO,
  JP_SMART,
  JP_TERMINAL_ACK,
  JP_INVERT_YAXIS,
  JP_INVERT_ZAXIS,
  JP_MOVE_SPEED,
  JP_KNOB_LED,
  JP_M0_PAUSE,
};

const char *const am_pack[LABEL_NUM]={
  AM_LANGUAGE,
  AM_HEAT,
  AM_MOVE,
  AM_HOME,
  AM_PRINT,
  AM_EXTRUDE,
  AM_FAN,
  AM_SETTINGS,
  AM_SCREEN_SETTINGS,
  AM_MACHINE_SETTINGS,
  AM_FEATURE_SETTINGS,
  AM_SILENT_ON,
  AM_SILENT_OFF,
  AM_SHUT_DOWN,
  AM_RGB_SETTINGS,
  AM_RGB_OFF,
  AM_GCODE,
  AM_CUSTOM,
  AM_LEVELING,
  AM_POINT_1,
  AM_POINT_2,
  AM_POINT_3,
  AM_POINT_4,
  AM_ABL,
  AM_BLTOUCH,
  AM_BLTOUCH_TEST,
  AM_BLTOUCH_DEPLOY,
  AM_BLTOUCH_STOW,
  AM_BLTOUCH_REPEAT,
  AM_PROBE_OFFSET,
  AM_EEPROM_SAVE,
  AM_INC,
  AM_DEC,
  AM_NOZZLE,
  AM_BED,
  AM_STOP,
  AM_BACK,
  AM_PAGE_UP,
  AM_PAGE_DOWN,
  AM_PAUSE,
  AM_RESUME,
  AM_LOAD,
  AM_UNLOAD,
  AM_SLOW_SPEED,
  AM_NORMAL_SPEED,
  AM_FAST_SPEED,
  AM_FAN_FULL_SPEED,
  AM_FAN_HALF_SPEED,
  AM_ROTATE_UI,
  AM_TOUCHSCREEN_ADJUST,
  AM_MORE,
  AM_SCREEN_INFO,
  AM_WHITE,
  AM_BLACK,
  AM_BLUE,
  AM_RED,
  AM_GREEN,
  AM_CYAN,
  AM_YELLOW,
  AM_BROWN,
  AM_GRAY,
  AM_ORANGE,
  AM_INDIGO,
  AM_VIOLET,
  AM_DISCONNECT,
  AM_BAUDRATE_115200,
  AM_BAUDRATE_250000,
  AM_RUNOUT_OFF,
  AM_RUNOUT_ON,
  AM_SMART_RUNOUT_ON,
  AM_PERCENTAGE,
  AM_BABYSTEP,
  AM_PERCENTAGE_SPEED,
  AM_PERCENTAGE_FLOW,
  AM_VALUE_ZERO,
  AM_1_DEGREE,
  AM_5_DEGREE,
  AM_10_DEGREE,
  AM_X_INC,
  AM_Y_INC,
  AM_Z_INC,
  AM_X_DEC,
  AM_Y_DEC,
  AM_Z_DEC,
  AM_X_HOME,
  AM_Y_HOME,
  AM_Z_HOME,
  AM_001_MM,
  AM_01_MM,
  AM_1_MM,
  AM_5_MM,
  AM_10_MM,
  AM_1_PERCENT,
  AM_5_PERCENT,
  AM_10_PERCENT,
  AM_READY,
  AM_BUSY,
  AM_UNCONNECTED,
  AM_DISCONNECT_INFO,
  AM_LOADING,
  AM_POWER_FAILED,
  AM_CONTINUE,
  AM_CANNEL,
  AM_ADJUST_TITLE,
  AM_ADJUST_INFO,
  AM_ADJUST_OK,
  AM_ADJUST_FAILED,
  AM_WARNING,
  AM_STOP_PRINT,
  AM_CONFIRM,
  AM_TFTSD,
  AM_READ_TFTSD_ERROR,
  AM_TFTSD_INSERTED,
  AM_TFTSD_REMOVED,
  AM_U_DISK,
  AM_READ_U_DISK_ERROR,
  AM_U_DISK_INSERTED,
  AM_U_DISK_REMOVED,
  AM_ONBOARDSD,
  AM_READ_ONBOARDSD_ERROR,
  AM_FILAMENT_RUNOUT,
  AM_PREHEAT,
  AM_PREHEAT_BOTH,
  AM_PREHEAT_PLA,
  AM_PREHEAT_PETG,
  AM_PREHEAT_ABS,
  AM_PREHEAT_CUSTOM1,
  AM_PREHEAT_CUSTOM2,
  AM_IS_PAUSE,
  AM_AUTO_SHUT_DOWN,
  AM_MANUAL_SHUT_DOWN,
  AM_UNIFIEDMOVE,
  AM_UNIFIEDHEAT,
  AM_COOLDOWN,
  AM_EMERGENCYSTOP,
  AM_TOUCH_TO_EXIT,
  AM_MAINMENU,
  AM_WAIT_TEMP_SHUT_DOWN,
  AM_FORCE_SHUT_DOWN,
  AM_SHUTTING_DOWN,
  AM_PARAMETER_SETTING,
  AM_ON,
  AM_OFF,
  AM_AUTO,
  AM_SMART,
  AM_TERMINAL_ACK,
  AM_INVERT_YAXIS,
  AM_INVERT_ZAXIS,
  AM_MOVE_SPEED,
  AM_KNOB_LED,
  AM_M0_PAUSE,
};

const char *const de_pack[LABEL_NUM]={
  DE_LANGUAGE,
  DE_HEAT,
  DE_MOVE,
  DE_HOME,
  DE_PRINT,
  DE_EXTRUDE,
  DE_FAN,
  DE_SETTINGS,
  DE_SCREEN_SETTINGS,
  DE_MACHINE_SETTINGS,
  DE_FEATURE_SETTINGS,
  DE_SILENT_ON,
  DE_SILENT_OFF,
  DE_SHUT_DOWN,
  DE_RGB_SETTINGS,
  DE_RGB_OFF,
  DE_GCODE,
  DE_CUSTOM,
  DE_LEVELING,
  DE_POINT_1,
  DE_POINT_2,
  DE_POINT_3,
  DE_POINT_4,
  DE_ABL,
  DE_BLTOUCH,
  DE_BLTOUCH_TEST,
  DE_BLTOUCH_DEPLOY,
  DE_BLTOUCH_STOW,
  DE_BLTOUCH_REPEAT,
  DE_PROBE_OFFSET,
  DE_EEPROM_SAVE,
  DE_INC,
  DE_DEC,
  DE_NOZZLE,
  DE_BED,
  DE_STOP,
  DE_BACK,
  DE_PAGE_UP,
  DE_PAGE_DOWN,
  DE_PAUSE,
  DE_RESUME,
  DE_LOAD,
  DE_UNLOAD,
  DE_SLOW_SPEED,
  DE_NORMAL_SPEED,
  DE_FAST_SPEED,
  DE_FAN_FULL_SPEED,
  DE_FAN_HALF_SPEED,
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
  DE_PERCENTAGE_FLOW,
  DE_VALUE_ZERO,
  DE_1_DEGREE,
  DE_5_DEGREE,
  DE_10_DEGREE,
  DE_X_INC,
  DE_Y_INC,
  DE_Z_INC,
  DE_X_DEC,
  DE_Y_DEC,
  DE_Z_DEC,
  DE_X_HOME,
  DE_Y_HOME,
  DE_Z_HOME,
  DE_001_MM,
  DE_01_MM,
  DE_1_MM,
  DE_5_MM,
  DE_10_MM,
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
  DE_CANNEL,
  DE_ADJUST_TITLE,
  DE_ADJUST_INFO,
  DE_ADJUST_OK,
  DE_ADJUST_FAILED,
  DE_WARNING,
  DE_STOP_PRINT,
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
  DE_PREHEAT,
  DE_PREHEAT_BOTH,
  DE_PREHEAT_PLA,
  DE_PREHEAT_PETG,
  DE_PREHEAT_ABS,
  DE_PREHEAT_CUSTOM1,
  DE_PREHEAT_CUSTOM2,
  DE_IS_PAUSE,
  DE_AUTO_SHUT_DOWN,
  DE_MANUAL_SHUT_DOWN,
  DE_UNIFIEDMOVE,
  DE_UNIFIEDHEAT,
  DE_COOLDOWN,
  DE_EMERGENCYSTOP,
  DE_TOUCH_TO_EXIT,
  DE_MAINMENU,
  DE_WAIT_TEMP_SHUT_DOWN,
  DE_FORCE_SHUT_DOWN,
  DE_SHUTTING_DOWN,
  DE_PARAMETER_SETTING,
  DE_ON,
  DE_OFF,
  DE_AUTO,
  DE_SMART,
  DE_TERMINAL_ACK,
  DE_INVERT_YAXIS,
  DE_INVERT_ZAXIS,
  DE_MOVE_SPEED,
  DE_KNOB_LED,
  DE_M0_PAUSE,
};

const char *const cz_pack[LABEL_NUM]={
  CZ_LANGUAGE,
  CZ_HEAT,
  CZ_MOVE,
  CZ_HOME,
  CZ_PRINT,
  CZ_EXTRUDE,
  CZ_FAN,
  CZ_SETTINGS,
  CZ_SCREEN_SETTINGS,
  CZ_MACHINE_SETTINGS,
  CZ_FEATURE_SETTINGS,
  CZ_SILENT_ON,
  CZ_SILENT_OFF,
  CZ_SHUT_DOWN,
  CZ_RGB_SETTINGS,
  CZ_RGB_OFF,
  CZ_GCODE,
  CZ_CUSTOM,
  CZ_LEVELING,
  CZ_POINT_1,
  CZ_POINT_2,
  CZ_POINT_3,
  CZ_POINT_4,
  CZ_ABL,
  CZ_BLTOUCH,
  CZ_BLTOUCH_TEST,
  CZ_BLTOUCH_DEPLOY,
  CZ_BLTOUCH_STOW,
  CZ_BLTOUCH_REPEAT,
  CZ_PROBE_OFFSET,
  CZ_EEPROM_SAVE,
  CZ_INC,
  CZ_DEC,
  CZ_NOZZLE,
  CZ_BED,
  CZ_STOP,
  CZ_BACK,
  CZ_PAGE_UP,
  CZ_PAGE_DOWN,
  CZ_PAUSE,
  CZ_RESUME,
  CZ_LOAD,
  CZ_UNLOAD,
  CZ_SLOW_SPEED,
  CZ_NORMAL_SPEED,
  CZ_FAST_SPEED,
  CZ_FAN_FULL_SPEED,
  CZ_FAN_HALF_SPEED,
  CZ_ROTATE_UI,
  CZ_TOUCHSCREEN_ADJUST,
  CZ_MORE,
  CZ_SCREEN_INFO,
  CZ_WHITE,
  CZ_BLACK,
  CZ_BLUE,
  CZ_RED,
  CZ_GREEN,
  CZ_CYAN,
  CZ_YELLOW,
  CZ_BROWN,
  CZ_GRAY,
  CZ_ORANGE,
  CZ_INDIGO,
  CZ_VIOLET,
  CZ_DISCONNECT,
  CZ_BAUDRATE_115200,
  CZ_BAUDRATE_250000,
  CZ_RUNOUT_OFF,
  CZ_RUNOUT_ON,
  CZ_SMART_RUNOUT_ON,
  CZ_PERCENTAGE,
  CZ_BABYSTEP,
  CZ_PERCENTAGE_SPEED,
  CZ_PERCENTAGE_FLOW,
  CZ_VALUE_ZERO,
  CZ_1_DEGREE,
  CZ_5_DEGREE,
  CZ_10_DEGREE,
  CZ_X_INC,
  CZ_Y_INC,
  CZ_Z_INC,
  CZ_X_DEC,
  CZ_Y_DEC,
  CZ_Z_DEC,
  CZ_X_HOME,
  CZ_Y_HOME,
  CZ_Z_HOME,
  CZ_001_MM,
  CZ_01_MM,
  CZ_1_MM,
  CZ_5_MM,
  CZ_10_MM,
  CZ_1_PERCENT,
  CZ_5_PERCENT,
  CZ_10_PERCENT,
  CZ_READY,
  CZ_BUSY,
  CZ_UNCONNECTED,
  CZ_DISCONNECT_INFO,
  CZ_LOADING,
  CZ_POWER_FAILED,
  CZ_CONTINUE,
  CZ_CANNEL,
  CZ_ADJUST_TITLE,
  CZ_ADJUST_INFO,
  CZ_ADJUST_OK,
  CZ_ADJUST_FAILED,
  CZ_WARNING,
  CZ_STOP_PRINT,
  CZ_CONFIRM,
  CZ_TFTSD,
  CZ_READ_TFTSD_ERROR,
  CZ_TFTSD_INSERTED,
  CZ_TFTSD_REMOVED,
  CZ_U_DISK,
  CZ_READ_U_DISK_ERROR,
  CZ_U_DISK_INSERTED,
  CZ_U_DISK_REMOVED,
  CZ_ONBOARDSD,
  CZ_READ_ONBOARDSD_ERROR,
  CZ_FILAMENT_RUNOUT,
  CZ_PREHEAT,
  CZ_PREHEAT_BOTH,
  CZ_PREHEAT_PLA,
  CZ_PREHEAT_PETG,
  CZ_PREHEAT_ABS,
  CZ_PREHEAT_CUSTOM1,
  CZ_PREHEAT_CUSTOM2,
  CZ_IS_PAUSE,
  CZ_AUTO_SHUT_DOWN,
  CZ_MANUAL_SHUT_DOWN,
  CZ_UNIFIEDMOVE,
  CZ_UNIFIEDHEAT,
  CZ_COOLDOWN,
  CZ_EMERGENCYSTOP,
  CZ_TOUCH_TO_EXIT,
  CZ_MAINMENU,
  CZ_WAIT_TEMP_SHUT_DOWN,
  CZ_FORCE_SHUT_DOWN,
  CZ_SHUTTING_DOWN,
  CZ_PARAMETER_SETTING,
  CZ_ON,
  CZ_OFF,
  CZ_AUTO,
  CZ_SMART,
  CZ_TERMINAL_ACK,
  CZ_INVERT_YAXIS,
  CZ_INVERT_ZAXIS,
  CZ_MOVE_SPEED,
  CZ_KNOB_LED,
  CZ_M0_PAUSE,
};

const char *const es_pack[LABEL_NUM]={
  ES_LANGUAGE,
  ES_HEAT,
  ES_MOVE,
  ES_HOME,
  ES_PRINT,
  ES_EXTRUDE,
  ES_FAN,
  ES_SETTINGS,
  ES_SCREEN_SETTINGS,
  ES_MACHINE_SETTINGS,
  ES_FEATURE_SETTINGS,
  ES_SILENT_ON,
  ES_SILENT_OFF,
  ES_SHUT_DOWN,
  ES_RGB_SETTINGS,
  ES_RGB_OFF,
  ES_GCODE,
  ES_CUSTOM,
  ES_LEVELING,
  ES_POINT_1,
  ES_POINT_2,
  ES_POINT_3,
  ES_POINT_4,
  ES_ABL,
  ES_BLTOUCH,
  ES_BLTOUCH_TEST,
  ES_BLTOUCH_DEPLOY,
  ES_BLTOUCH_STOW,
  ES_BLTOUCH_REPEAT,
  ES_PROBE_OFFSET,
  ES_EEPROM_SAVE,
  ES_INC,
  ES_DEC,
  ES_NOZZLE,
  ES_BED,
  ES_STOP,
  ES_BACK,
  ES_PAGE_UP,
  ES_PAGE_DOWN,
  ES_PAUSE,
  ES_RESUME,
  ES_LOAD,
  ES_UNLOAD,
  ES_SLOW_SPEED,
  ES_NORMAL_SPEED,
  ES_FAST_SPEED,
  ES_FAN_FULL_SPEED,
  ES_FAN_HALF_SPEED,
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
  ES_PERCENTAGE_FLOW,
  ES_VALUE_ZERO,
  ES_1_DEGREE,
  ES_5_DEGREE,
  ES_10_DEGREE,
  ES_X_INC,
  ES_Y_INC,
  ES_Z_INC,
  ES_X_DEC,
  ES_Y_DEC,
  ES_Z_DEC,
  ES_X_HOME,
  ES_Y_HOME,
  ES_Z_HOME,
  ES_001_MM,
  ES_01_MM,
  ES_1_MM,
  ES_5_MM,
  ES_10_MM,
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
  ES_CANNEL,
  ES_ADJUST_TITLE,
  ES_ADJUST_INFO,
  ES_ADJUST_OK,
  ES_ADJUST_FAILED,
  ES_WARNING,
  ES_STOP_PRINT,
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
  ES_PREHEAT,
  ES_PREHEAT_BOTH,
  ES_PREHEAT_PLA,
  ES_PREHEAT_PETG,
  ES_PREHEAT_ABS,
  ES_PREHEAT_CUSTOM1,
  ES_PREHEAT_CUSTOM2,
  ES_IS_PAUSE,
  ES_AUTO_SHUT_DOWN,
  ES_MANUAL_SHUT_DOWN,
  ES_UNIFIEDMOVE,
  ES_UNIFIEDHEAT,
  ES_COOLDOWN,
  ES_EMERGENCYSTOP,
  ES_TOUCH_TO_EXIT,
  ES_MAINMENU,
  ES_WAIT_TEMP_SHUT_DOWN,
  ES_FORCE_SHUT_DOWN,
  ES_SHUTTING_DOWN,
  ES_PARAMETER_SETTING,
  ES_ON,
  ES_OFF,
  ES_AUTO,
  ES_SMART,
  ES_TERMINAL_ACK,
  ES_INVERT_YAXIS,
  ES_INVERT_ZAXIS,
  ES_MOVE_SPEED,
  ES_KNOB_LED,
  ES_M0_PAUSE,
};

const char *const fr_pack[LABEL_NUM]={
  FR_LANGUAGE,
  FR_HEAT,
  FR_MOVE,
  FR_HOME,
  FR_PRINT,
  FR_EXTRUDE,
  FR_FAN,
  FR_SETTINGS,
  FR_SCREEN_SETTINGS,
  FR_MACHINE_SETTINGS,
  FR_FEATURE_SETTINGS,
  FR_SILENT_ON,
  FR_SILENT_OFF,
  FR_SHUT_DOWN,
  FR_RGB_SETTINGS,
  FR_RGB_OFF,
  FR_GCODE,
  FR_CUSTOM,
  FR_LEVELING,
  FR_POINT_1,
  FR_POINT_2,
  FR_POINT_3,
  FR_POINT_4,
  FR_ABL,
  FR_BLTOUCH,
  FR_BLTOUCH_TEST,
  FR_BLTOUCH_DEPLOY,
  FR_BLTOUCH_STOW,
  FR_BLTOUCH_REPEAT,
  FR_PROBE_OFFSET,
  FR_EEPROM_SAVE,
  FR_INC,
  FR_DEC,
  FR_NOZZLE,
  FR_BED,
  FR_STOP,
  FR_BACK,
  FR_PAGE_UP,
  FR_PAGE_DOWN,
  FR_PAUSE,
  FR_RESUME,
  FR_LOAD,
  FR_UNLOAD,
  FR_SLOW_SPEED,
  FR_NORMAL_SPEED,
  FR_FAST_SPEED,
  FR_FAN_FULL_SPEED,
  FR_FAN_HALF_SPEED,
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
  FR_PERCENTAGE_FLOW,
  FR_VALUE_ZERO,
  FR_1_DEGREE,
  FR_5_DEGREE,
  FR_10_DEGREE,
  FR_X_INC,
  FR_Y_INC,
  FR_Z_INC,
  FR_X_DEC,
  FR_Y_DEC,
  FR_Z_DEC,
  FR_X_HOME,
  FR_Y_HOME,
  FR_Z_HOME,
  FR_001_MM,
  FR_01_MM,
  FR_1_MM,
  FR_5_MM,
  FR_10_MM,
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
  FR_CANNEL,
  FR_ADJUST_TITLE,
  FR_ADJUST_INFO,
  FR_ADJUST_OK,
  FR_ADJUST_FAILED,
  FR_WARNING,
  FR_STOP_PRINT,
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
  FR_PREHEAT,
  FR_PREHEAT_BOTH,
  FR_PREHEAT_PLA,
  FR_PREHEAT_PETG,
  FR_PREHEAT_ABS,
  FR_PREHEAT_CUSTOM1,
  FR_PREHEAT_CUSTOM2,
  FR_IS_PAUSE,
  FR_AUTO_SHUT_DOWN,
  FR_MANUAL_SHUT_DOWN,
  FR_UNIFIEDMOVE,
  FR_UNIFIEDHEAT,
  FR_COOLDOWN,
  FR_EMERGENCYSTOP,
  FR_TOUCH_TO_EXIT,
  FR_MAINMENU,
  FR_WAIT_TEMP_SHUT_DOWN,
  FR_FORCE_SHUT_DOWN,
  FR_SHUTTING_DOWN,
  FR_PARAMETER_SETTING,
  FR_ON,
  FR_OFF,
  FR_AUTO,
  FR_SMART,
  FR_TERMINAL_ACK,
  FR_INVERT_YAXIS,
  FR_INVERT_ZAXIS,
  FR_MOVE_SPEED,
  FR_KNOB_LED,
  FR_M0_PAUSE,
};

const char *const pt_pack[LABEL_NUM]={
  PT_LANGUAGE,
  PT_HEAT,
  PT_MOVE,
  PT_HOME,
  PT_PRINT,
  PT_EXTRUDE,
  PT_FAN,
  PT_SETTINGS,
  PT_SCREPT_SETTINGS,
  PT_MACHINE_SETTINGS,
  PT_FEATURE_SETTINGS,
  PT_SILENT_ON,
  PT_SILENT_OFF,
  PT_SHUT_DOWN,
  PT_RGB_SETTINGS,
  PT_RGB_OFF,
  PT_GCODE,
  PT_CUSTOM,
  PT_LEVELING,
  PT_POINT_1,
  PT_POINT_2,
  PT_POINT_3,
  PT_POINT_4,
  PT_ABL,
  PT_BLTOUCH,
  PT_BLTOUCH_TEST,
  PT_BLTOUCH_DEPLOY,
  PT_BLTOUCH_STOW,
  PT_BLTOUCH_REPEAT,
  PT_PROBE_OFFSET,
  PT_EEPROM_SAVE,
  PT_INC,
  PT_DEC,
  PT_NOZZLE,
  PT_BED,
  PT_STOP,
  PT_BACK,
  PT_PAGE_UP,
  PT_PAGE_DOWN,
  PT_PAUSE,
  PT_RESUME,
  PT_LOAD,
  PT_UNLOAD,
  PT_SLOW_SPEED,
  PT_NORMAL_SPEED,
  PT_FAST_SPEED,
  PT_FAN_FULL_SPEED,
  PT_FAN_HALF_SPEED,
  PT_ROTATE_UI,
  PT_TOUCHSCREPT_ADJUST,
  PT_MORE,
  PT_SCREPT_INFO,
  PT_WHITE,
  PT_BLACK,
  PT_BLUE,
  PT_RED,
  PT_GREEN,
  PT_CYAN,
  PT_YELLOW,
  PT_BROWN,
  PT_GRAY,
  PT_ORANGE,
  PT_INDIGO,
  PT_VIOLET,
  PT_DISCONNECT,
  PT_BAUDRATE_115200,
  PT_BAUDRATE_250000,
  PT_RUNOUT_OFF,
  PT_RUNOUT_ON,
  PT_SMART_RUNOUT_ON,
  PT_PERCENTAGE,
  PT_BABYSTEP,
  PT_PERCENTAGE_SPEED,
  PT_PERCENTAGE_FLOW,
  PT_VALUE_ZERO,
  PT_1_DEGREE,
  PT_5_DEGREE,
  PT_10_DEGREE,
  PT_X_INC,
  PT_Y_INC,
  PT_Z_INC,
  PT_X_DEC,
  PT_Y_DEC,
  PT_Z_DEC,
  PT_X_HOME,
  PT_Y_HOME,
  PT_Z_HOME,
  PT_001_MM,
  PT_01_MM,
  PT_1_MM,
  PT_5_MM,
  PT_10_MM,
  PT_1_PERCENT,
  PT_5_PERCENT,
  PT_10_PERCENT,
  PT_READY,
  PT_BUSY,
  PT_UNCONNECTED,
  PT_DISCONNECT_INFO,
  PT_LOADING,
  PT_POWER_FAILED,
  PT_CONTINUE,
  PT_CANNEL,
  PT_ADJUST_TITLE,
  PT_ADJUST_INFO,
  PT_ADJUST_OK,
  PT_ADJUST_FAILED,
  PT_WARNING,
  PT_STOP_PRINT,
  PT_CONFIRM,
  PT_TFTSD,
  PT_READ_TFTSD_ERROR,
  PT_TFTSD_INSERTED,
  PT_TFTSD_REMOVED,
  PT_U_DISK,
  PT_READ_U_DISK_ERROR,
  PT_U_DISK_INSERTED,
  PT_U_DISK_REMOVED,
  PT_ONBOARDSD,
  PT_READ_ONBOARDSD_ERROR,
  PT_FILAMENT_RUNOUT,
  PT_PREHEAT,
  PT_PREHEAT_BOTH,
  PT_PREHEAT_PLA,
  PT_PREHEAT_PETG,
  PT_PREHEAT_ABS,
  PT_PREHEAT_CUSTOM1,
  PT_PREHEAT_CUSTOM2,
  PT_IS_PAUSE,
  PT_AUTO_SHUT_DOWN,
  PT_MANUAL_SHUT_DOWN,
  PT_UNIFIEDMOVE,
  PT_UNIFIEDHEAT,
  PT_COOLDOWN,
  PT_EMERGENCYSTOP,
  PT_TOUCH_TO_EXIT,
  PT_MAINMENU,
  PT_WAIT_TEMP_SHUT_DOWN,
  PT_FORCE_SHUT_DOWN,
  PT_SHUTTING_DOWN,
  PT_PARAMETER_SETTING,
  PT_ON,
  PT_OFF,
  PT_AUTO,
  PT_SMART,
  PT_TERMINAL_ACK,
  PT_INVERT_YAXIS,
  PT_INVERT_ZAXIS,
  PT_MOVE_SPEED,
  PT_KNOB_LED,
  PT_M0_PAUSE,
};

const char *const it_pack[LABEL_NUM]={
  IT_LANGUAGE,
  IT_HEAT,
  IT_MOVE,
  IT_HOME,
  IT_PRINT,
  IT_EXTRUDE,
  IT_FAN,
  IT_SETTINGS,
  IT_SCREEN_SETTINGS,
  IT_MACHINE_SETTINGS,
  IT_FEATURE_SETTINGS,
  IT_SILENT_ON,
  IT_SILENT_OFF,
  IT_SHUT_DOWN,
  IT_RGB_SETTINGS,
  IT_RGB_OFF,
  IT_GCODE,
  IT_CUSTOM,
  IT_LEVELING,
  IT_POINT_1,
  IT_POINT_2,
  IT_POINT_3,
  IT_POINT_4,
  IT_ABL,
  IT_BLTOUCH,
  IT_BLTOUCH_TEST,
  IT_BLTOUCH_DEPLOY,
  IT_BLTOUCH_STOW,
  IT_BLTOUCH_REPEAT,
  IT_PROBE_OFFSET,
  IT_EEPROM_SAVE,
  IT_INC,
  IT_DEC,
  IT_NOZZLE,
  IT_BED,
  IT_STOP,
  IT_BACK,
  IT_PAGE_UP,
  IT_PAGE_DOWN,
  IT_PAUSE,
  IT_RESUME,
  IT_LOAD,
  IT_UNLOAD,
  IT_SLOW_SPEED,
  IT_NORMAL_SPEED,
  IT_FAST_SPEED,
  IT_FAN_FULL_SPEED,
  IT_FAN_HALF_SPEED,
  IT_ROTATE_UI,
  IT_TOUCHSCREEN_ADJUST,
  IT_MORE,
  IT_SCREEN_INFO,
  IT_WHITE,
  IT_BLACK,
  IT_BLUE,
  IT_RED,
  IT_GREEN,
  IT_CYAN,
  IT_YELLOW,
  IT_BROWN,
  IT_GRAY,
  IT_ORANGE,
  IT_INDIGO,
  IT_VIOLET,
  IT_DISCONNECT,
  IT_BAUDRATE_115200,
  IT_BAUDRATE_250000,
  IT_RUNOUT_OFF,
  IT_RUNOUT_ON,
  IT_SMART_RUNOUT_ON,
  IT_PERCENTAGE,
  IT_BABYSTEP,
  IT_PERCENTAGE_SPEED,
  IT_PERCENTAGE_FLOW,
  IT_VALUE_ZERO,
  IT_1_DEGREE,
  IT_5_DEGREE,
  IT_10_DEGREE,
  IT_X_INC,
  IT_Y_INC,
  IT_Z_INC,
  IT_X_DEC,
  IT_Y_DEC,
  IT_Z_DEC,
  IT_X_HOME,
  IT_Y_HOME,
  IT_Z_HOME,
  IT_001_MM,
  IT_01_MM,
  IT_1_MM,
  IT_5_MM,
  IT_10_MM,
  IT_1_PERCENT,
  IT_5_PERCENT,
  IT_10_PERCENT,
  IT_READY,
  IT_BUSY,
  IT_UNCONNECTED,
  IT_DISCONNECT_INFO,
  IT_LOADING,
  IT_POWER_FAILED,
  IT_CONTINUE,
  IT_CANNEL,
  IT_ADJUST_TITLE,
  IT_ADJUST_INFO,
  IT_ADJUST_OK,
  IT_ADJUST_FAILED,
  IT_WARNING,
  IT_STOP_PRINT,
  IT_CONFIRM,
  IT_TFTSD,
  IT_READ_TFTSD_ERROR,
  IT_TFTSD_INSERTED,
  IT_TFTSD_REMOVED,
  IT_U_DISK,
  IT_READ_U_DISK_ERROR,
  IT_U_DISK_INSERTED,
  IT_U_DISK_REMOVED,
  IT_ONBOARDSD,
  IT_READ_ONBOARDSD_ERROR,
  IT_FILAMENT_RUNOUT,
  IT_PREHEAT,
  IT_PREHEAT_BOTH,
  IT_PREHEAT_PLA,
  IT_PREHEAT_PETG,
  IT_PREHEAT_ABS,
  IT_PREHEAT_CUSTOM1,
  IT_PREHEAT_CUSTOM2,
  IT_IS_PAUSE,
  IT_AUTO_SHUT_DOWN,
  IT_MANUAL_SHUT_DOWN,
  IT_UNIFIEDMOVE,
  IT_UNIFIEDHEAT,
  IT_COOLDOWN,
  IT_EMERGENCYSTOP,
  IT_TOUCH_TO_EXIT,
  IT_MAINMENU,
  IT_WAIT_TEMP_SHUT_DOWN,
  IT_FORCE_SHUT_DOWN,
  IT_SHUTTING_DOWN,
  IT_PARAMETER_SETTING,
  IT_ON,
  IT_OFF,
  IT_AUTO,
  IT_SMART,
  IT_TERMINAL_ACK,
  IT_INVERT_YAXIS,
  IT_INVERT_ZAXIS,
  IT_MOVE_SPEED,
  IT_KNOB_LED,
  IT_M0_PAUSE,
};

const char *const pl_pack[LABEL_NUM]={
  PL_LANGUAGE,
  PL_HEAT,
  PL_MOVE,
  PL_HOME,
  PL_PRINT,
  PL_EXTRUDE,
  PL_FAN,
  PL_SETTINGS,
  PL_SCREEN_SETTINGS,
  PL_MACHINE_SETTINGS,
  PL_FEATURE_SETTINGS,
  PL_SILENT_ON,
  PL_SILENT_OFF,
  PL_SHUT_DOWN,
  PL_RGB_SETTINGS,
  PL_RGB_OFF,
  PL_GCODE,
  PL_CUSTOM,
  PL_LEVELING,
  PL_POINT_1,
  PL_POINT_2,
  PL_POINT_3,
  PL_POINT_4,
  PL_ABL,
  PL_BLTOUCH,
  PL_BLTOUCH_TEST,
  PL_BLTOUCH_DEPLOY,
  PL_BLTOUCH_STOW,
  PL_BLTOUCH_REPEAT,
  PL_PROBE_OFFSET,
  PL_EEPROM_SAVE,
  PL_INC,
  PL_DEC,
  PL_NOZZLE,
  PL_BED,
  PL_STOP,
  PL_BACK,
  PL_PAGE_UP,
  PL_PAGE_DOWN,
  PL_PAUSE,
  PL_RESUME,
  PL_LOAD,
  PL_UNLOAD,
  PL_SLOW_SPEED,
  PL_NORMAL_SPEED,
  PL_FAST_SPEED,
  PL_FAN_FULL_SPEED,
  PL_FAN_HALF_SPEED,
  PL_ROTATE_UI,
  PL_TOUCHSCREEN_ADJUST,
  PL_MORE,
  PL_SCREEN_INFO,
  PL_WHITE,
  PL_BLACK,
  PL_BLUE,
  PL_RED,
  PL_GREEN,
  PL_CYAN,
  PL_YELLOW,
  PL_BROWN,
  PL_GRAY,
  PL_ORANGE,
  PL_INDIGO,
  PL_VIOLET,
  PL_DISCONNECT,
  PL_BAUDRATE_115200,
  PL_BAUDRATE_250000,
  PL_RUNOUT_OFF,
  PL_RUNOUT_ON,
  PL_SMART_RUNOUT_ON,
  PL_PERCENTAGE,
  PL_BABYSTEP,
  PL_PERCENTAGE_SPEED,
  PL_PERCENTAGE_FLOW,
  PL_VALUE_ZERO,
  PL_1_DEGREE,
  PL_5_DEGREE,
  PL_10_DEGREE,
  PL_X_INC,
  PL_Y_INC,
  PL_Z_INC,
  PL_X_DEC,
  PL_Y_DEC,
  PL_Z_DEC,
  PL_X_HOME,
  PL_Y_HOME,
  PL_Z_HOME,
  PL_001_MM,
  PL_01_MM,
  PL_1_MM,
  PL_5_MM,
  PL_10_MM,
  PL_1_PERCENT,
  PL_5_PERCENT,
  PL_10_PERCENT,
  PL_READY,
  PL_BUSY,
  PL_UNCONNECTED,
  PL_DISCONNECT_INFO,
  PL_LOADING,
  PL_POWER_FAILED,
  PL_CONTINUE,
  PL_CANNEL,
  PL_ADJUST_TITLE,
  PL_ADJUST_INFO,
  PL_ADJUST_OK,
  PL_ADJUST_FAILED,
  PL_WARNING,
  PL_STOP_PRINT,
  PL_CONFIRM,
  PL_TFTSD,
  PL_READ_TFTSD_ERROR,
  PL_TFTSD_INSERTED,
  PL_TFTSD_REMOVED,
  PL_U_DISK,
  PL_READ_U_DISK_ERROR,
  PL_U_DISK_INSERTED,
  PL_U_DISK_REMOVED,
  PL_ONBOARDSD,
  PL_READ_ONBOARDSD_ERROR,
  PL_FILAMENT_RUNOUT,
  PL_PREHEAT,
  PL_PREHEAT_BOTH,
  PL_PREHEAT_PLA,
  PL_PREHEAT_PETG,
  PL_PREHEAT_ABS,
  PL_PREHEAT_CUSTOM1,
  PL_PREHEAT_CUSTOM2,
  PL_IS_PAUSE,
  PL_AUTO_SHUT_DOWN,
  PL_MANUAL_SHUT_DOWN,
  PL_UNIFIEDMOVE,
  PL_UNIFIEDHEAT,
  PL_COOLDOWN,
  PL_EMERGENCYSTOP,
  PL_TOUCH_TO_EXIT,
  PL_MAINMENU,
  PL_WAIT_TEMP_SHUT_DOWN,
  PL_FORCE_SHUT_DOWN,
  PL_SHUTTING_DOWN,
  PL_PARAMETER_SETTING,
  PL_ON,
  PL_OFF,
  PL_AUTO,
  PL_SMART,
  PL_TERMINAL_ACK,
  PL_INVERT_YAXIS,
  PL_INVERT_ZAXIS,
  PL_MOVE_SPEED,
  PL_KNOB_LED,
  PL_M0_PAUSE,
};

const char *const sk_pack[LABEL_NUM]={
  SK_LANGUAGE,
  SK_HEAT,
  SK_MOVE,
  SK_HOME,
  SK_PRINT,
  SK_EXTRUDE,
  SK_FAN,
  SK_SETTINGS,
  SK_SCREEN_SETTINGS,
  SK_MACHINE_SETTINGS,
  SK_FEATURE_SETTINGS,
  SK_SILENT_ON,
  SK_SILENT_OFF,
  SK_SHUT_DOWN,
  SK_RGB_SETTINGS,
  SK_RGB_OFF,
  SK_GCODE,
  SK_CUSTOM,
  SK_LEVELING,
  SK_POINT_1,
  SK_POINT_2,
  SK_POINT_3,
  SK_POINT_4,
  SK_ABL,
  SK_BLTOUCH,
  SK_BLTOUCH_TEST,
  SK_BLTOUCH_DEPLOY,
  SK_BLTOUCH_STOW,
  SK_BLTOUCH_REPEAT,
  SK_PROBE_OFFSET,
  SK_EEPROM_SAVE,
  SK_INC,
  SK_DEC,
  SK_NOZZLE,
  SK_BED,
  SK_STOP,
  SK_BACK,
  SK_PAGE_UP,
  SK_PAGE_DOWN,
  SK_PAUSE,
  SK_RESUME,
  SK_LOAD,
  SK_UNLOAD,
  SK_SLOW_SPEED,
  SK_NORMAL_SPEED,
  SK_FAST_SPEED,
  SK_FAN_FULL_SPEED,
  SK_FAN_HALF_SPEED,
  SK_ROTATE_UI,
  SK_TOUCHSCREEN_ADJUST,
  SK_MORE,
  SK_SCREEN_INFO,
  SK_WHITE,
  SK_BLACK,
  SK_BLUE,
  SK_RED,
  SK_GREEN,
  SK_CYAN,
  SK_YELLOW,
  SK_BROWN,
  SK_GRAY,
  SK_ORANGE,
  SK_INDIGO,
  SK_VIOLET,
  SK_DISCONNECT,
  SK_BAUDRATE_115200,
  SK_BAUDRATE_250000,
  SK_RUNOUT_OFF,
  SK_RUNOUT_ON,
  SK_SMART_RUNOUT_ON,
  SK_PERCENTAGE,
  SK_BABYSTEP,
  SK_PERCENTAGE_SPEED,
  SK_PERCENTAGE_FLOW,
  SK_VALUE_ZERO,
  SK_1_DEGREE,
  SK_5_DEGREE,
  SK_10_DEGREE,
  SK_X_INC,
  SK_Y_INC,
  SK_Z_INC,
  SK_X_DEC,
  SK_Y_DEC,
  SK_Z_DEC,
  SK_X_HOME,
  SK_Y_HOME,
  SK_Z_HOME,
  SK_001_MM,
  SK_01_MM,
  SK_1_MM,
  SK_5_MM,
  SK_10_MM,
  SK_1_PERCENT,
  SK_5_PERCENT,
  SK_10_PERCENT,
  SK_READY,
  SK_BUSY,
  SK_UNCONNECTED,
  SK_DISCONNECT_INFO,
  SK_LOADING,
  SK_POWER_FAILED,
  SK_CONTINUE,
  SK_CANNEL,
  SK_ADJUST_TITLE,
  SK_ADJUST_INFO,
  SK_ADJUST_OK,
  SK_ADJUST_FAILED,
  SK_WARNING,
  SK_STOP_PRINT,
  SK_CONFIRM,
  SK_TFTSD,
  SK_READ_TFTSD_ERROR,
  SK_TFTSD_INSERTED,
  SK_TFTSD_REMOVED,
  SK_U_DISK,
  SK_READ_U_DISK_ERROR,
  SK_U_DISK_INSERTED,
  SK_U_DISK_REMOVED,
  SK_ONBOARDSD,
  SK_READ_ONBOARDSD_ERROR,
  SK_FILAMENT_RUNOUT,
  SK_PREHEAT,
  SK_PREHEAT_BOTH,
  SK_PREHEAT_PLA,
  SK_PREHEAT_PETG,
  SK_PREHEAT_ABS,
  SK_PREHEAT_CUSTOM1,
  SK_PREHEAT_CUSTOM2,
  SK_IS_PAUSE,
  SK_AUTO_SHUT_DOWN,
  SK_MANUAL_SHUT_DOWN,
  SK_UNIFIEDMOVE,
  SK_UNIFIEDHEAT,
  SK_COOLDOWN,
  SK_EMERGENCYSTOP,
  SK_TOUCH_TO_EXIT,
  SK_MAINMENU,
  SK_WAIT_TEMP_SHUT_DOWN,
  SK_FORCE_SHUT_DOWN,
  SK_SHUTTING_DOWN,
  SK_PARAMETER_SETTING,
  SK_ON,
  SK_OFF,
  SK_AUTO,
  SK_SMART,
  SK_TERMINAL_ACK,
  SK_INVERT_YAXIS,
  SK_INVERT_ZAXIS,
  SK_MOVE_SPEED,
  SK_KNOB_LED,
  SK_M0_PAUSE,
};

const char *const du_pack[LABEL_NUM]={
  DU_LANGUAGE,
  DU_HEAT,
  DU_MOVE,
  DU_HOME,
  DU_PRINT,
  DU_EXTRUDE,
  DU_FAN,
  DU_SETTINGS,
  DU_SCREEN_SETTINGS,
  DU_MACHINE_SETTINGS,
  DU_FEATURE_SETTINGS,
  DU_SILENT_ON,
  DU_SILENT_OFF,
  DU_SHUT_DOWN,
  DU_RGB_SETTINGS,
  DU_RGB_OFF,
  DU_GCODE,
  DU_CUSTOM,
  DU_LEVELING,
  DU_POINT_1,
  DU_POINT_2,
  DU_POINT_3,
  DU_POINT_4,
  DU_ABL,
  DU_BLTOUCH,
  DU_BLTOUCH_TEST,
  DU_BLTOUCH_DEPLOY,
  DU_BLTOUCH_STOW,
  DU_BLTOUCH_REPEAT,
  DU_PROBE_OFFSET,
  DU_EEPROM_SAVE,
  DU_INC,
  DU_DEC,
  DU_NOZZLE,
  DU_BED,
  DU_STOP,
  DU_BACK,
  DU_PAGE_UP,
  DU_PAGE_DOWN,
  DU_PAUSE,
  DU_RESUME,
  DU_LOAD,
  DU_UNLOAD,
  DU_SLOW_SPEED,
  DU_NORMAL_SPEED,
  DU_FAST_SPEED,
  DU_FAN_FULL_SPEED,
  DU_FAN_HALF_SPEED,
  DU_ROTATE_UI,
  DU_TOUCHSCREEN_ADJUST,
  DU_MORE,
  DU_SCREEN_INFO,
  DU_WHITE,
  DU_BLACK,
  DU_BLUE,
  DU_RED,
  DU_GREEN,
  DU_CYAN,
  DU_YELLOW,
  DU_BROWN,
  DU_GRAY,
  DU_ORANGE,
  DU_INDIGO,
  DU_VIOLET,
  DU_DISCONNECT,
  DU_BAUDRATE_115200,
  DU_BAUDRATE_250000,
  DU_RUNOUT_OFF,
  DU_RUNOUT_ON,
  DU_SMART_RUNOUT_ON,
  DU_PERCENTAGE,
  DU_BABYSTEP,
  DU_PERCENTAGE_SPEED,
  DU_PERCENTAGE_FLOW,
  DU_VALUE_ZERO,
  DU_1_DEGREE,
  DU_5_DEGREE,
  DU_10_DEGREE,
  DU_X_INC,
  DU_Y_INC,
  DU_Z_INC,
  DU_X_DEC,
  DU_Y_DEC,
  DU_Z_DEC,
  DU_X_HOME,
  DU_Y_HOME,
  DU_Z_HOME,
  DU_001_MM,
  DU_01_MM,
  DU_1_MM,
  DU_5_MM,
  DU_10_MM,
  DU_1_PERCENT,
  DU_5_PERCENT,
  DU_10_PERCENT,
  DU_READY,
  DU_BUSY,
  DU_UNCONNECTED,
  DU_DISCONNECT_INFO,
  DU_LOADING,
  DU_POWER_FAILED,
  DU_CONTINUE,
  DU_CANNEL,
  DU_ADJUST_TITLE,
  DU_ADJUST_INFO,
  DU_ADJUST_OK,
  DU_ADJUST_FAILED,
  DU_WARNING,
  DU_STOP_PRINT,
  DU_CONFIRM,
  DU_TFTSD,
  DU_READ_TFTSD_ERROR,
  DU_TFTSD_INSERTED,
  DU_TFTSD_REMOVED,
  DU_U_DISK,
  DU_READ_U_DISK_ERROR,
  DU_U_DISK_INSERTED,
  DU_U_DISK_REMOVED,
  DU_ONBOARDSD,
  DU_READ_ONBOARDSD_ERROR,
  DU_FILAMENT_RUNOUT,
  DU_PREHEAT,
  DU_PREHEAT_BOTH,
  DU_PREHEAT_PLA,
  DU_PREHEAT_PETG,
  DU_PREHEAT_ABS,
  DU_PREHEAT_CUSTOM1,
  DU_PREHEAT_CUSTOM2,
  DU_IS_PAUSE,
  DU_AUTO_SHUT_DOWN,
  DU_MANUAL_SHUT_DOWN,
  DU_UNIFIEDMOVE,
  DU_UNIFIEDHEAT,
  DU_COOLDOWN,
  DU_EMERGENCYSTOP,
  DU_TOUCH_TO_EXIT,
  DU_MAINMENU,
  DU_WAIT_TEMP_SHUT_DOWN,
  DU_FORCE_SHUT_DOWN,
  DU_SHUTTING_DOWN,
  DU_PARAMETER_SETTING,
  DU_ON,
  DU_OFF,
  DU_AUTO,
  DU_SMART,
  DU_TERMINAL_ACK,
  DU_INVERT_YAXIS,
  DU_INVERT_ZAXIS,
  DU_MOVE_SPEED,
  DU_KNOB_LED,
  DU_M0_PAUSE,
};

u8 * textSelect(u8 sel)
{
  switch(infoSettings.language)
  {
    case ENGLISH:     return (u8 *)en_pack[sel];
    case CHINESE:     return (u8 *)cn_pack[sel];
    case RUSSIAN:     return (u8 *)ru_pack[sel];
    case JAPANESE:    return (u8 *)jp_pack[sel];
    case ARMENIAN:    return (u8 *)am_pack[sel];
    case GERMAN:      return (u8 *)de_pack[sel];
    case CZECH:       return (u8 *)cz_pack[sel];
    case SPAIN:       return (u8 *)es_pack[sel];
    case FRENCH:      return (u8 *)fr_pack[sel];
    case PORTUGUESE:  return (u8 *)pt_pack[sel];
    case ITALIAN:     return (u8 *)it_pack[sel];
    case POLISH:      return (u8 *)pl_pack[sel];
    case SLOVAK:      return (u8 *)sk_pack[sel];
    case DUTCH:       return (u8 *)du_pack[sel];

    default:        return NULL;
  }
}
