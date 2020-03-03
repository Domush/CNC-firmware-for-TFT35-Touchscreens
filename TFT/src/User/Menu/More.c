#include "includes.h"
#include "More.h"

void menuIsPause(void) {
  u16 key_num = IDLE_TOUCH;

  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_PAUSE), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while (infoMenu.menu[infoMenu.active] == menuIsPause) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        if (setPrintPause(true))
          infoMenu.menu[infoMenu.active] = menuMove;
        break;

      case KEY_POPUP_CANCEL:
        infoMenu.active--;
        break;
    }
    runUpdateLoop();
  }
}

MENUITEMS moreItems = {
    //  title
    LABEL_MORE,
    // icon                       label
    {
        {ICON_ROUTER, LABEL_ROUTER},
        {ICON_RGB_SETTINGS, LABEL_RGB_SETTINGS},
        {ICON_FEATURE_SETTINGS, LABEL_FEATURE_SETTINGS},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACK, LABEL_BACK},
    }};

void menuMore(void) {
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&moreItems);
  while (infoMenu.menu[infoMenu.active] == menuMore) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        infoMenu.menu[++infoMenu.active] = menuRouter;
        break;

      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.active] = menuRGBSettings;
        break;

      case KEY_ICON_2:
        infoMenu.menu[++infoMenu.active] = menuFeatureSettings;
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
