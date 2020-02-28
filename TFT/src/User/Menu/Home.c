#include "Home.h"
#include "includes.h"

const MENUITEMS homeItems = {
    //   title
    LABEL_HOME,
    // icon                       label
    {
        {ICON_HOME, LABEL_HOME},
        {ICON_X_HOME, LABEL_X_HOME},
        {ICON_Y_HOME, LABEL_Y_HOME},
        {ICON_Z_HOME, LABEL_Z_HOME},
        {ICON_HOME, LABEL_COORD_1},
        {ICON_HOME, LABEL_COORD_2},
        {ICON_HOME, LABEL_COORD_3},
        {ICON_BACK, LABEL_BACK},
    }};

void menuHome(void) {
  extern PRINTING infoPrinting;
  if (infoPrinting.coordSpace < 53) infoPrinting.coordSpace = 53;

  KEY_VALUES key_num = KEY_IDLE;
  menuDrawPage(&homeItems);
  while (infoMenu.menu[infoMenu.active] == menuHome) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        storeCmd("G28\n");
        break;
      case KEY_ICON_1:
        storeCmd("G28 X\n");
        break;
      case KEY_ICON_2:
        storeCmd("G28 Y\n");
        break;
      case KEY_ICON_3:
        storeCmd("G28 Z\n");
        break;
      case KEY_ICON_4:  // Change to Machine Coord Space (G53)
        infoPrinting.coordSpace = 53;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        // routerControl(0);
        break;
      case KEY_ICON_5:  // Change to Coord Space 2 (G54)
        infoPrinting.coordSpace = 54;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        // routerChangeBit();
        break;
      case KEY_ICON_6:  // Change to Coord Space 3 (G55)
        infoPrinting.coordSpace = 55;
        storeCmd("G%d\n", infoPrinting.coordSpace);
        // routerControl(255);
        break;
      case KEY_ICON_7:
        infoMenu.active--;
        break;
      default:
        break;
    }
    loopProcess();
  }
}
