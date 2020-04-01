// #include "MainPage.h"
#include "includes.h"
// #include "Move.h"

extern SETTINGS infoSettings;

//1 title, ITEM_PER_PAGE items(icon+label)
const MENUITEMS mainPageItems = {
    // title
    LABEL_READY,
    // icon                       label
    {
        {ICON_HOME, LABEL_HOME},
        {ICON_MOVE, LABEL_MOVE},
        {ICON_JOBSETUP, LABEL_JOBSETUP},
        {ICON_CNC, LABEL_CNC},
        {ICON_ROUTER, LABEL_ROUTER},
        {ICON_DISABLE_XY, LABEL_DISABLE_XY},
        {ICON_DISABLE_Z, LABEL_DISABLE_Z},
        {ICON_SETTINGS, LABEL_SETTINGS},
    }};

void menuMain(void) {
  KEY_VALUES key_num = KEY_IDLE;
  GUI_SetBkColor(ST7920_BKCOLOR);
  menuDrawPage(&mainPageItems);

  while (infoMenu.menu[infoMenu.active] == menuMain) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        infoMenu.menu[++infoMenu.active] = menuHome;
        break;
      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.active] = menuMove;
        break;
      case KEY_ICON_2:
        infoMenu.menu[++infoMenu.active] = menuJobSetup;
        break;
      case KEY_ICON_3:
        if (infoJobStatus.inProgress) {
          infoMenu.menu[++infoMenu.active] = menuPrinting;
        } else {
          infoMenu.menu[++infoMenu.active] = menuPrint;
        }
        break;
      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.active] = menuRouter;
        break;
      case KEY_ICON_5: {
        timedMessage(2, TIMED_INFO, "X,Y steppers disabled");
        queueCommand(false, "M18 X Y\n");   //disable X and Y motors
      } break;
      case KEY_ICON_6: {
        timedMessage(2, TIMED_INFO, "Z stepper disabled");
        queueCommand(false, "M18 Z\n");   //disable Z motors
      } break;
      case KEY_ICON_7:
        infoMenu.menu[++infoMenu.active] = menuSettings;
        break;
      default:
        break;
    }
    runUpdateLoop();
  }
}
