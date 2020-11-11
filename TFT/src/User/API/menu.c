#include "menu.h"
#include "includes.h"

// LCD init functions
#include "lcd.h"
#include "GUI.h"

// Multi-language support
#include "Language/Language.h"

// Chip specific includes
#include "Serial.h"
#include "usart.h"

// UI handling
#include "ui_draw.h"
#include "touch_process.h"

// File handling
#include "Vfs/vfs.h"
#include "list_item.h"

// Gcode processing
#include "Gcode/gcodeSender.h"
#include "Gcode/gcodeResponseHandler.h"

// Timing functions
#include "System/os_timer.h"
#include "System/boot.h"

// Menus
#include "includesMenus.h" // All menu headers

// exhibitRect is 2 ICON Space in the Upper Row and 2 Center Coloum.
const GUI_RECT exhibitRect = {
    1 * ICON_WIDTH + 1 * SPACE_X + START_X, 0 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 3 * ICON_WIDTH + 2 * SPACE_X + START_X, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y};

const GUI_RECT rect_of_key[ITEM_PER_PAGE * 2] = {
    // 8 icons area
    {0 * ICON_WIDTH + 0 * SPACE_X + START_X, 0 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 1 * ICON_WIDTH + 0 * SPACE_X + START_X, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y},
    {1 * ICON_WIDTH + 1 * SPACE_X + START_X, 0 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 2 * ICON_WIDTH + 1 * SPACE_X + START_X, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y},
    {2 * ICON_WIDTH + 2 * SPACE_X + START_X, 0 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 3 * ICON_WIDTH + 2 * SPACE_X + START_X, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y},
    {3 * ICON_WIDTH + 3 * SPACE_X + START_X, 0 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 4 * ICON_WIDTH + 3 * SPACE_X + START_X, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y},
    {0 * ICON_WIDTH + 0 * SPACE_X + START_X, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y, 1 * ICON_WIDTH + 0 * SPACE_X + START_X, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y},
    {1 * ICON_WIDTH + 1 * SPACE_X + START_X, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y, 2 * ICON_WIDTH + 1 * SPACE_X + START_X, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y},
    {2 * ICON_WIDTH + 2 * SPACE_X + START_X, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y, 3 * ICON_WIDTH + 2 * SPACE_X + START_X, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y},
    {3 * ICON_WIDTH + 3 * SPACE_X + START_X, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y, 4 * ICON_WIDTH + 3 * SPACE_X + START_X, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y},

    // 8 labels area
    {0 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 1 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y},
    {1 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 2 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y},
    {2 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 3 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y},
    {3 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, 4 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y},
    {0 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y, 1 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 2 * SPACE_Y + ICON_START_Y},
    {1 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y, 2 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 2 * SPACE_Y + ICON_START_Y},
    {2 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y, 3 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 2 * SPACE_Y + ICON_START_Y},
    {3 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y, 4 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 2 * SPACE_Y + ICON_START_Y},
};

const GUI_RECT rect_of_keyListView[ITEM_PER_PAGE] = {
    //6 icons area
    {START_X, 0 * LISTITEM_HEIGHT + ICON_START_Y, LISTITEM_WIDTH + START_X, 1 * LISTITEM_HEIGHT + ICON_START_Y},
    {START_X, 1 * LISTITEM_HEIGHT + ICON_START_Y, LISTITEM_WIDTH + START_X, 2 * LISTITEM_HEIGHT + ICON_START_Y},
    {START_X, 2 * LISTITEM_HEIGHT + ICON_START_Y, LISTITEM_WIDTH + START_X, 3 * LISTITEM_HEIGHT + ICON_START_Y},
    {START_X, 3 * LISTITEM_HEIGHT + ICON_START_Y, LISTITEM_WIDTH + START_X, 4 * LISTITEM_HEIGHT + ICON_START_Y},
    {START_X, 4 * LISTITEM_HEIGHT + ICON_START_Y, LISTITEM_WIDTH + START_X, 5 * LISTITEM_HEIGHT + ICON_START_Y},

    {2 * START_X + LISTITEM_WIDTH, 0 * LIST_ICON_HEIGHT + 0 * LISTICON_SPACE_Y + ICON_START_Y, 2 * START_X + LISTITEM_WIDTH + 1 * LIST_ICON_WIDTH, 1 * LIST_ICON_HEIGHT + 0 * LISTICON_SPACE_Y + ICON_START_Y},
    {2 * START_X + LISTITEM_WIDTH, 1 * LIST_ICON_HEIGHT + 1 * LISTICON_SPACE_Y + ICON_START_Y, 2 * START_X + LISTITEM_WIDTH + 1 * LIST_ICON_WIDTH, 2 * LIST_ICON_HEIGHT + 1 * LISTICON_SPACE_Y + ICON_START_Y},
    {2 * START_X + LISTITEM_WIDTH, 2 * LIST_ICON_HEIGHT + 2 * LISTICON_SPACE_Y + ICON_START_Y, 2 * START_X + LISTITEM_WIDTH + 1 * LIST_ICON_WIDTH, 3 * LIST_ICON_HEIGHT + 2 * LISTICON_SPACE_Y + ICON_START_Y},
};

//Clean up the gaps outside icons
void menuClearGaps(void) {
  return; //!Prevent removal of wrapping text
  const GUI_RECT gaps[] = {
      {0, 0, LCD_WIDTH, TITLE_END_Y},
      {0, TITLE_END_Y, LCD_WIDTH, ICON_START_Y},
      {0, ICON_START_Y, 0 * ICON_WIDTH + 0 * SPACE_X + START_X, LCD_HEIGHT},
      {1 * ICON_WIDTH + 0 * SPACE_X + START_X, ICON_START_Y, 1 * ICON_WIDTH + 1 * SPACE_X + START_X, LCD_HEIGHT},
      {2 * ICON_WIDTH + 1 * SPACE_X + START_X, ICON_START_Y, 2 * ICON_WIDTH + 2 * SPACE_X + START_X, LCD_HEIGHT},
      {3 * ICON_WIDTH + 2 * SPACE_X + START_X, ICON_START_Y, 3 * ICON_WIDTH + 3 * SPACE_X + START_X, LCD_HEIGHT},
      {4 * ICON_WIDTH + 3 * SPACE_X + START_X, ICON_START_Y, LCD_WIDTH, LCD_HEIGHT}};

  GUI_SetBkColor(BACKGROUND_COLOR);
  GUI_ClearPrect(gaps);
  GUI_SetBkColor(BACKGROUND_COLOR);
  for (uint8_t i = 1; i < COUNT(gaps); i++)
    GUI_ClearPrect(gaps + i);
}

void GUI_RestoreColorDefault(void) {
  GUI_SetBkColor(BACKGROUND_COLOR);
  GUI_SetColor(FONT_COLOR);
  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_SetNumMode(GUI_NUMMODE_SPACE);
}

static const MENUITEMS *curMenuItems = NULL;   //current menu
static const LISTITEMS *curListItems = NULL;   //current listmenu

static bool isListview;

uint8_t *labelGetAddress(const LABEL *label) {
  if (label->index == LABEL_BACKGROUND) return NULL;   // No content in label
  if (label->index < LABEL_NUM)                        // Index of language
    return textSelect(label->index);
  else   // Address of string
    return label->address;
}

void menuDrawItem(const ITEM *item, uint8_t positon) {
  uint8_t *content     = labelGetAddress(&item->label);
  const GUI_RECT *rect = rect_of_key + positon;
  if (item->icon != ICON_BACKGROUND)
    ICON_ReadDisplay(rect->x0, rect->y0, item->icon);
  else
    GUI_ClearPrect(rect);

  rect = rect_of_key + ITEM_PER_PAGE + positon;
  GUI_ClearPrect(rect);
  if (content)
    GUI_DispStringInPrect(rect, content);
}

void menuDrawIconOnly(const ITEM *item, uint8_t positon) {
  const GUI_RECT *rect = rect_of_key + positon;
  if (item->icon != ICON_BACKGROUND)
    ICON_ReadDisplay(rect->x0, rect->y0, item->icon);
  else
    GUI_ClearPrect(rect);
}

void menuDrawListItem(const LISTITEM *item, uint8_t position) {
  const GUI_RECT *rect = rect_of_keyListView + position;
  if (item->icon == ICONCHAR_BACKGROUND) {
    GUI_ClearPrect(rect);
  } else {
    ListItem_Display(rect, position, item, false);
  }
}
void menuRefreshListPage(void) {
  for (uint8_t i = 0; i < ITEM_PER_PAGE; i++) {
    menuDrawListItem(&curListItems->items[i], i);
  }
}

void menuDrawTitle(const uint8_t *content) {
  return;   //!Disabled for status message support
  uint16_t start_y = (TITLE_END_Y - BYTE_HEIGHT) / 2;
  GUI_FillRectColor(10, start_y, LCD_WIDTH - 10, start_y + BYTE_HEIGHT, BACKGROUND_COLOR);

  if (content) {
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_DispLenString(10, start_y, content, LCD_WIDTH - 20);
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  }
}

//Draw the icon-based interface
void menuDrawPage(const MENUITEMS *menuItems) {
  u8 i           = 0;
  isListview     = false;
  curMenuItems   = menuItems;
  TSC_ReDrawIcon = itemDrawIconPress;

  GUI_Clear(BLACK);
  // menuClearGaps();  //Use this function instead of GUI_Clear to eliminate the splash screen when clearing the screen.
  menuDrawTitle(labelGetAddress(&menuItems->title));
  for (i = 0; i < ITEM_PER_PAGE; i++) {
    menuDrawItem(&menuItems->items[i], i);
  }
}

//Draw the listing-based interface
void menuDrawListPage(const LISTITEMS *listItems) {
  u8 i           = 0;
  isListview     = true;
  curListItems   = listItems;
  TSC_ReDrawIcon = itemDrawIconPress;

  GUI_SetBkColor(BACKGROUND_COLOR);
  GUI_ClearRect(0, 0, LCD_WIDTH, TITLE_END_Y);
  GUI_SetBkColor(BACKGROUND_COLOR);
  GUI_ClearRect(0, TITLE_END_Y, LCD_WIDTH, LCD_HEIGHT);

  // menuClearGaps();  //Use this function instead of GUI_Clear to eliminate the splash screen when clearing the screen.
  menuDrawTitle(labelGetAddress(&listItems->title));

  for (i = 0; i < ITEM_PER_PAGE; i++) {
    //const GUI_RECT *rect = rect_of_keyListView + i;
    if (curListItems->items[i].icon != ICONCHAR_BACKGROUND)
      menuDrawListItem(&curListItems->items[i], i);
  }
}

// When there is a button value, the icon changes color and redraws
void itemDrawIconPress(u8 position, u8 is_press) {
  if (position > KEY_ICON_7) return;

  if (isListview == false) {
    if (curMenuItems == NULL) return;
    if (curMenuItems->items[position].icon == ICON_BACKGROUND) return;

    const GUI_RECT *rect = rect_of_key + position;

    if (is_press)   // Turn green when pressed
      ICON_PressedDisplay(rect->x0, rect->y0, curMenuItems->items[position].icon);
    else   // Redraw normal icon when released
      ICON_ReadDisplay(rect->x0, rect->y0, curMenuItems->items[position].icon);
  } else {   //draw rec over list item if pressed
    if (curListItems == NULL)
      return;

    const GUI_RECT *rect = rect_of_keyListView + position;

    if (curListItems->items[position].icon == ICONCHAR_BACKGROUND) {
      GUI_ClearPrect(rect);
      return;
    }
    if (is_press) {
      ListItem_Display(rect, position, &curListItems->items[position], true);
    } else {
      ListItem_Display(rect, position, &curListItems->items[position], false);
    }
  }
}

// Get button value
KEY_VALUES menuKeyGetValue(void) {
  if (isListview == false) {
    return (KEY_VALUES)KEY_GetValue(sizeof(rect_of_key) / sizeof(rect_of_key[0]), rect_of_key);   // for normal menu
  } else {
    return (KEY_VALUES)KEY_GetValue(sizeof(rect_of_keyListView) / sizeof(rect_of_keyListView[0]), rect_of_keyListView);   //for listview
  }
}

// Get the top-left point of the corresponding icon position)
GUI_POINT getIconStartPoint(int index) {
  GUI_POINT p = {rect_of_key[index].x0, rect_of_key[index].y0};
  return p;
}

static TIMEDMESSAGE lastMessage;

void timedMessageExpire(void) {
  char *menuTitle;
  menuTitle = " ";
  if (OS_GetTime() > lastMessage.timeout) {
    GUI_SetColor(MAT_LOWWHITE);
    if (curMenuItems == NULL && strcmp(lastMessage.message, "Ready") != 0 && infoHost.connected) {
      GUI_FillRectColor(BYTE_WIDTH * ((infoHost.connected) ? 2 : 0), 0, LCD_WIDTH - BYTE_WIDTH * ((infoJobStatus.inProgress) ? 9 : 0), BYTE_HEIGHT, BACKGROUND_COLOR);
      GUI_DispStringInRect(BYTE_WIDTH * 2, 0, LCD_WIDTH - BYTE_WIDTH * ((infoJobStatus.inProgress) ? 9 : 2), BYTE_HEIGHT, (u8 *)"Ready");
      lastMessage.message = "Ready";
    } else {
      menuTitle = (char *)textSelect(curMenuItems->title.index);
      if (curMenuItems != NULL) {
        if (lastMessage.message != menuTitle) {
          if (infoHost.connected) {
            GUI_FillRectColor(BYTE_WIDTH * ((infoHost.connected) ? 2 : 0), 0, LCD_WIDTH - BYTE_WIDTH * ((infoJobStatus.inProgress) ? 9 : 0), BYTE_HEIGHT, BACKGROUND_COLOR);
            GUI_DispStringInRect(BYTE_WIDTH * 2, 0, LCD_WIDTH - BYTE_WIDTH * ((infoJobStatus.inProgress) ? 9 : 2), BYTE_HEIGHT, (u8 *)textSelect(curMenuItems->title.index));
            lastMessage.message = (char *)textSelect(curMenuItems->title.index);
          }
        }
      }
    }
    GUI_RestoreColorDefault();
    lastMessage.type = 0;
  }
}

void timedMessage(u8 delay_secs, MESSAGE_TYPE type, char *string, ...) {
  if (type < lastMessage.type) {
    return;
  } else if (lastMessage.message == string) {
    lastMessage.timeout = OS_GetTime() + delay_secs * 100;
    return;
  }
  switch (type) {
    case TIMED_STATUS:
      GUI_SetColor(MAT_BLUE);
      break;
    case TIMED_INFO:
      GUI_SetColor(MAT_LOWWHITE);
      break;
    case TIMED_WARNNG:
      GUI_SetColor(MAT_YELLOW);
      break;
    case TIMED_ERROR:
      GUI_SetColor(MAT_ORANGE);
      break;
    case TIMED_CRITICAL:
      GUI_SetColor(MAT_RED);
      break;

    default:
      GUI_SetColor(MAT_LOWWHITE);
      break;
  }
  if (strlen(string) > 1) {
    char buffer[500] = "";
    my_va_list ap;
    my_va_start(ap, string);
    my_vsprintf(buffer, string, ap);
    my_va_end(ap);
    char displayText[LCD_WIDTH / BYTE_WIDTH - 10] = "";
    strncpy(displayText, buffer, LCD_WIDTH / BYTE_WIDTH - 11);
    if (lastMessage.message == &displayText[0] && lastMessage.type == type) {
      return;
    }
    lastMessage.type    = type;
    lastMessage.timeout = OS_GetTime() + delay_secs * 100;
    GUI_FillRectColor(BYTE_WIDTH * ((infoHost.connected) ? 2 : 0), 0, LCD_WIDTH - BYTE_WIDTH * ((infoJobStatus.inProgress) ? 9 : 0), BYTE_HEIGHT, BACKGROUND_COLOR);
    GUI_DispStringInRect(BYTE_WIDTH * 2, 0, LCD_WIDTH - BYTE_WIDTH * ((infoJobStatus.inProgress) ? 9 : 2), BYTE_HEIGHT, (u8 *)displayText);
    GUI_RestoreColorDefault();
    lastMessage.message = &displayText[0];
  }
}

void runUpdateLoop(void) {
  processGcode();   // Process all gcode
  updateScreen();   // Process screen updates / touch detection
}

void updateScreen(void) {
  timedMessageExpire();     // Remove expired status bar messages
  detectSDInsertion();      // Check if SD/USB has been inserted
  updateGantryLocation();   // Update gantry coordinate display
  gcodeQueueStatus();       // Update gCode queue icon
}

void processGcode(void) {
  parseGcodeResponse();   // Parse the received slave response information
  parseGcodeOutgoing();   // Parse and send Gcode commands in the queue
  parseSerialGcode();     // Parse the received Gcode from other UART, such as: ESP3D, etc...
  getGcodeFromFile();     // Get Gcode command from the file to be printed
#if defined ONBOARD_SD_SUPPORT && !defined M27_AUTOREPORT
  checkJobStatus();   // Check if there is a SD or USB print running.
#endif

#ifdef U_DISK_SUPPORT
  USBH_Process(&USB_OTG_Core, &USB_Host);
#endif

#if LCD_ENCODER_SUPPORT
  loopCheckMode();
#endif

}
