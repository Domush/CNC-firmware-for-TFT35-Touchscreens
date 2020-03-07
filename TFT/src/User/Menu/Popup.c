#include "Popup.h"
#include "my_misc.h"
#include "includes.h"

#define BUTTON_NUM 1

BUTTON bottomSingleBtn = {
    //button location                       color before pressed   color after pressed
    POPUP_RECT_SINGLE_CONFIRM, NULL, 5, 1, COLOR_BLUE, BLACK, WHITE, COLOR_BLUE, WHITE, COLOR_NAVY};

BUTTON bottomDoubleBtn[] = {
    {POPUP_RECT_DOUBLE_CONFIRM, NULL, 5, 1, COLOR_BLUE, BLACK, WHITE, COLOR_BLUE, WHITE, COLOR_NAVY},
    {POPUP_RECT_DOUBLE_CANCEL, NULL, 5, 1, RED, BLACK, WHITE, RED, WHITE, RED},
};

GUI_RECT doubleBtnRect[] = {POPUP_RECT_DOUBLE_CONFIRM, POPUP_RECT_DOUBLE_CANCEL};

WINDOW window = {
    POPUP_RECT_WINDOW,                         //Area of ??popup
    13,                                        //Four-corner arc radius
    5,                                         //Outer line width
    COLOR_NAVY,                                //Outer and title bar background color
    {WHITE, COLOR_NAVY, POPUP_TITLE_HEIGHT},   //Title bar font color /background color /height
    {BLACK, WHITE, POPUP_TEXT_HEIGHT},         //Text bar font color /background color /height
    {WHITE, GRAY, POPUP_BOTTOM_HEIGHT},        //Bottom (font color) /background color /(height)
};

static BUTTON *windowButton = NULL;
static u16 buttonNum        = 0;

void windowReDrawButton(u8 positon, u8 pressed) {
  if (positon >= buttonNum) return;
  if (pressed >= 2) return;
  if (windowButton == NULL) return;
  if (windowButton->context == NULL) return;

  GUI_DrawButton(windowButton + positon, pressed);
}

void popupDrawPage(BUTTON *btn, const u8 *title, const u8 *text, const u8 *choice1, const u8 *choice2) {
  buttonNum    = 0;
  windowButton = btn;
  if (choice1) {
    windowButton[buttonNum++].context = choice1;
  }
  if (choice2) {
    windowButton[buttonNum++].context = choice2;
  }

  TSC_ReDrawIcon = windowReDrawButton;
  GUI_DrawWindow(&window, title, text);

  for (u8 i = 0; i < buttonNum; i++)
    GUI_DrawButton(&windowButton[i], 0);
}

static const GUI_RECT popupMenuRect = POPUP_RECT_SINGLE_CONFIRM;

void menuPopup(void) {
  u16 key_num = IDLE_TOUCH;

  while (infoMenu.menu[infoMenu.active] == menuPopup) {
    key_num = KEY_GetValue(BUTTON_NUM, &popupMenuRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        infoMenu.active--;
        break;

      default:
        break;
    }
    runUpdateLoop();
  }
}

void popupReminder(u8 *title, u8 *text) {
  popupDrawPage(&bottomSingleBtn, title, text, textSelect(LABEL_CONFIRM), NULL);
  if (infoMenu.menu[infoMenu.active] != menuPopup) {
    infoMenu.menu[++infoMenu.active] = menuPopup;
  }
}
