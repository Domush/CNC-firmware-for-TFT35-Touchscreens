#include "Popup.h"
#include "my_misc.h"
#include "includes.h"

#define BUTTON_NUM 1

BUTTON bottomSingleBtn = {
    //button location                       color before pressed   color after pressed
    POPUP_RECT_SINGLE_CONFIRM, NULL, 5, 1, GREEN, BLACK, WHITE, GREEN, WHITE, BLACK};

BUTTON bottomDoubleBtn[] = {
    {POPUP_RECT_DOUBLE_CONFIRM, NULL, 5, 1, GREEN, BLACK, WHITE, GREEN, WHITE, BLACK},
    {POPUP_RECT_DOUBLE_CANCEL, NULL, 5, 1, GREEN, BLACK, WHITE, GREEN, WHITE, BLACK},
};

GUI_RECT doubleBtnRect[] = {POPUP_RECT_DOUBLE_CONFIRM, POPUP_RECT_DOUBLE_CANCEL};

WINDOW window = {
    POPUP_RECT_WINDOW,                            //Area of ??popup
    6,                                            //Four-corner arc radius
    3,                                            //Outer line width
    COLOR_DARKBLUE,                               //Outer and title bar background color
    {WHITE, COLOR_DARKBLUE, POPUP_TITLE_HEIGHT},  //Title bar font color /background color /height
    {BLACK, WHITE, POPUP_TEXT_HEIGHT},            //Text bar font color /background color /height
    {WHITE, GRAY, POPUP_BOTTOM_HEIGHT},           //Bottom (font color) /background color /(height)
};

static BUTTON *windowButton = NULL;
static u16 buttonNum = 0;

void windowReDrawButton(u8 positon, u8 pressed) {
  if (positon >= buttonNum) return;
  if (pressed >= 2) return;
  if (windowButton == NULL) return;
  if (windowButton->context == NULL) return;

  GUI_DrawButton(windowButton + positon, pressed);
}

void popupDrawPage(BUTTON *btn, const u8 *title, const u8 *context, const u8 *choice1, const u8 *choice2) {
  buttonNum = 0;
  windowButton = btn;
  if (choice1) {
    windowButton[buttonNum++].context = choice1;
  }
  if (choice2) {
    windowButton[buttonNum++].context = choice2;
  }

  TSC_ReDrawIcon = windowReDrawButton;
  GUI_DrawWindow(&window, title, context);

  for (u8 i = 0; i < buttonNum; i++)
    GUI_DrawButton(&windowButton[i], 0);
}

static const GUI_RECT popupMenuRect = POPUP_RECT_SINGLE_CONFIRM;

void menuPopup(void) {
  u16 key_num = IDLE_TOUCH;

  while (infoMenu.menu[infoMenu.cur] == menuPopup) {
    key_num = KEY_GetValue(BUTTON_NUM, &popupMenuRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        infoMenu.cur--;
        break;

      default:
        break;
    }
    loopProcess();
  }
}

void popupReminder(u8 *info, u8 *message) {
  if (strstr((char *)message, "//action:prompt_end")) {
    // char message[] = "Load V-Bit -  0.5\" Dia., then Pos@ 0:0:1mm\r\n//action:prompt_end\n//action:prompt_begin M0/1 Break Called\n//action:prompt_button Continue\n//action:prompt_show\n";
    char *title = substr((char *)message, (int)(strstr((char *)message, "//action:prompt_begin") + 21), (int)(strstr((char *)message, "//action:prompt_button") - 2));
    char *text = substr((char *)message, 0, (int)(strstr((char *)message, "//action:prompt_end") - 2));
    char *button = substr((char *)message, (int)(strstr((char *)message, "//action:prompt_button") + 22), (int)(strstr((char *)message, "//action:prompt_show") - 2));
    popupDrawPage(&bottomSingleBtn, (u8 *)title, (u8 *)text, (u8 *)button, NULL);
  } else {
    popupDrawPage(&bottomSingleBtn, info, message, textSelect(LABEL_CONFIRM), NULL);
  }

  if (infoMenu.menu[infoMenu.cur] != menuPopup) {
    infoMenu.menu[++infoMenu.cur] = menuPopup;
  }
}
