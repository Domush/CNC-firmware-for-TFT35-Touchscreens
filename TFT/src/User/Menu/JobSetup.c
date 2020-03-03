#include "JobSetup.h"
#include "Home.h"
#include "includes.h"

const MENUITEMS jobSetupItems = {
    // title
    LABEL_ABL,
    // icon                        label
    {
        {ICON_ZERO_X, LABEL_ZERO_X},
        {ICON_ZERO_Y, LABEL_ZERO_Y},
        {ICON_ZERO_Z, LABEL_ZERO_Z},
        {ICON_ZERO_ALL, LABEL_ZERO_ALL},
        {ICON_HOME, LABEL_COORD_1},
        {ICON_HOME, LABEL_COORD_2},
        {ICON_HOME, LABEL_X0Y0},
        {ICON_BACK, LABEL_BACK},
    }};

void menuJobSetup(void) {
  extern PRINTING infoPrinting;
  if (infoPrinting.coordSpace < 53) infoPrinting.coordSpace = 53;

  KEY_VALUES key_num = KEY_IDLE;
  menuDrawPage(&jobSetupItems);
  while (infoMenu.menu[infoMenu.active] == menuJobSetup) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:  // Zero X
        infoPrinting.coordSpace = 54;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        timedMessage(2, TIMED_INFO, "Zeroing X axis");
        storeCmd("G92 X0\n");
        break;
      case KEY_ICON_1:  // Zero Y
        infoPrinting.coordSpace = 54;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        timedMessage(2, TIMED_INFO, "Zeroing Y axis");
        storeCmd("G92 Y0\n");
        break;
      case KEY_ICON_2:  // Zero Z
        infoPrinting.coordSpace = 54;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        timedMessage(2, TIMED_INFO, "Zeroing Z axis");
        storeCmd("G92 Z0\n");
        break;
      case KEY_ICON_3:  // Zero all
        infoPrinting.coordSpace = 54;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        timedMessage(2, TIMED_INFO, "Zeroing all axes");
        storeCmd("G92 X0 Y0 Z0\n");
        break;
      case KEY_ICON_4:  // Change to Machine Coord Space (G53)
        infoPrinting.coordSpace = 53;
        timedMessage(2, TIMED_INFO, "Coord set 1");
        storeCmd("G%d\n", infoPrinting.coordSpace);
        break;
      case KEY_ICON_5:  // Change to Coord Space 2 (G54)
        timedMessage(2, TIMED_INFO, "Coord set 2");
        infoPrinting.coordSpace = 54;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        break;
      case KEY_ICON_6:  // Move to X0,Y0
        timedMessage(2, TIMED_INFO, "Moving to X0,Y0");
        storeCmd("G0 X0 Y0\n");
        break;
      case KEY_ICON_7:  // Back
        infoMenu.active--;
        break;
      default:
        break;
    }
    runUpdateLoop();
  }
}

const MENUITEMS manualLevelingItems = {
    // title
    LABEL_JOBSETUP,
    // icon                        label
    {
        {ICON_DISABLE_XY, LABEL_DISABLE_XY},
        {ICON_DISABLE_Z, LABEL_DISABLE_Z},
        {ICON_POINT_3, LABEL_POINT_3},
        {ICON_POINT_4, LABEL_POINT_4},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACK, LABEL_BACK},
    }};

void moveToLevelingPoint(u8 point) {
  static const s16 pointPosition[][2] = {
      {LEVELING_DISABLE_XY_X, LEVELING_DISABLE_XY_Y},
      {LEVELING_DISABLE_Z_X, LEVELING_DISABLE_Z_Y},
      {LEVELING_POINT_3_X, LEVELING_POINT_3_Y},
      {LEVELING_POINT_4_X, LEVELING_POINT_4_Y},
  };
  if (coordinateIsClear() == false) {
    storeCmd("G28\n");
  }
  storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_MOVE_Z, LEVELING_POINT_Z_GANTRYSPEED);
  storeCmd("G0 X%d Y%d F%d\n", pointPosition[point][0], pointPosition[point][1], LEVELING_POINT_XY_GANTRYSPEED);
  storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_Z, LEVELING_POINT_Z_GANTRYSPEED);
}

void menuManualLeveling(void) {
  KEY_VALUES key_num = KEY_IDLE;
  menuDrawPage(&manualLevelingItems);
  while (infoMenu.menu[infoMenu.active] == menuManualLeveling) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        moveToLevelingPoint(0);
        break;
      case KEY_ICON_1:
        moveToLevelingPoint(1);
        break;
      case KEY_ICON_2:
        moveToLevelingPoint(2);
        break;
      case KEY_ICON_3:
        moveToLevelingPoint(3);
        break;
      case KEY_ICON_7:
        infoMenu.active--;
        break;
      default:
        break;
    }
    runUpdateLoop();
  }
}
