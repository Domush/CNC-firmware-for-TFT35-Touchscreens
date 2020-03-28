#ifndef _POPUP_H_
#define _POPUP_H_
#include "includes.h"

// #include "UI/GUI.h"
// #include "API/helperFunctions.h"
// #include "System/variants.h"

// #define POPUP_RECT_WINDOW         {80, 50, 400, 270} // Original config
#define POPUP_RECT_WINDOW \
  { 40, BYTE_HEIGHT * 3 + 4, 440, LCD_HEIGHT - BYTE_HEIGHT }
// { 80, 50, 400, 270 }

#define POPUP_RECT_SINGLE_CONFIRM \
  { BYTE_WIDTH * 10, 300 - BYTE_HEIGHT * 2, LCD_WIDTH - BYTE_WIDTH * 10, 300 - 14 }
// { 180, 210, 300, 260 }
#define POPUP_RECT_DOUBLE_CONFIRM \
  { BYTE_WIDTH * 5, 300 - BYTE_HEIGHT * 2, LCD_WIDTH / 2 - BYTE_WIDTH, 300 - 14 }
// { 90, 210, 210, 260 }
#define POPUP_RECT_DOUBLE_CANCEL \
  { LCD_WIDTH / 2 + BYTE_WIDTH, 300 - BYTE_HEIGHT * 2, LCD_WIDTH - BYTE_WIDTH * 5, 300 - 14 }
// { 270, 210, 390, 260 }

#define POPUP_TITLE_HEIGHT  (BYTE_HEIGHT + 10)
#define POPUP_BOTTOM_HEIGHT (BYTE_HEIGHT * 2)
#define POPUP_TEXT_HEIGHT   (BYTE_HEIGHT * 5 + 16)

enum {
  KEY_POPUP_CONFIRM = 0,
  KEY_POPUP_CANCEL,
};

extern BUTTON bottomSingleBtn;
extern BUTTON bottomDoubleBtn[];
extern GUI_RECT doubleBtnRect[];
extern WINDOW window;

void windowSetButton(const BUTTON *btn);
void windowReDrawButton(uint8_t positon, uint8_t is_press);
void popupDrawPage(BUTTON *btn, const uint8_t *title, const uint8_t *text, const uint8_t *choice1, const uint8_t *choice2);
void popupReminder(u8 *title, u8 *text);

#endif
