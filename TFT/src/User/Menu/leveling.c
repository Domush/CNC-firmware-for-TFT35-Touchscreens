#include "leveling.h"
#include "includes.h"

const MENUITEMS homingItems = {
    // title
    LABEL_ABL,
    // icon                        label
    {
        {ICON_ZERO_X, LABEL_ZERO_X},
        {ICON_ZERO_Y, LABEL_ZERO_Y},
        {ICON_ZERO_Z, LABEL_ZERO_Z},
        {ICON_ZERO_ALL, LABEL_ZERO_ALL},
        {ICON_ROUTER_OFF, LABEL_ROUTER_OFF},
        {ICON_CHANGE_BIT, LABEL_CHANGE_BIT},
        {ICON_ROUTER_ON, LABEL_ROUTER_ON},
        {ICON_BACK, LABEL_BACK},
    }};

void menuHoming(void) {
  KEY_VALUES key_num = KEY_IDLE;
  menuDrawPage(&homingItems);
  while (infoMenu.menu[infoMenu.cur] == menuHoming) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:  // Home X
        storeCmd("G92 X0\n");
        break;
      case KEY_ICON_1:  // Home Y
        storeCmd("G92 Y0\n");
        break;
      case KEY_ICON_2:  // Home Z
        storeCmd("G92 Z0\n");
        break;
      case KEY_ICON_3:  // Home all
        storeCmd("G92 X0 Y0 Z0\n");
        break;
      case KEY_ICON_4:  // Router off
        routerControl(0);
        break;
      case KEY_ICON_5:  // Router replace bit
        routerChangeBit();
        break;
      case KEY_ICON_6:  // Router on
        routerControl(255);
        break;
      case KEY_ICON_7:  // Back
        infoMenu.cur--;
        break;
      default:
        break;
    }
    loopProcess();
  }
}

const MENUITEMS manualLevelingItems = {
    // title
    LABEL_LEVELING,
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
  storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_MOVE_Z, LEVELING_POINT_Z_FEEDRATE);
  storeCmd("G0 X%d Y%d F%d\n", pointPosition[point][0], pointPosition[point][1], LEVELING_POINT_XY_FEEDRATE);
  storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_Z, LEVELING_POINT_Z_FEEDRATE);
}

void menuManualLeveling(void) {
  KEY_VALUES key_num = KEY_IDLE;
  menuDrawPage(&manualLevelingItems);
  while (infoMenu.menu[infoMenu.cur] == menuManualLeveling) {
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
        infoMenu.cur--;
        break;
      default:
        break;
    }
    loopProcess();
  }
}
