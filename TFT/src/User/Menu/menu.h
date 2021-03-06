#ifndef _MENU_H_
#define _MENU_H_

#include "GUI.h"
#include "stdbool.h"
#include "stdint.h"

#define IDLE_TOUCH 0xFFFF
typedef enum {
  KEY_ICON_0 = 0,
  KEY_ICON_1,
  KEY_ICON_2,
  KEY_ICON_3,
  KEY_ICON_4,
  KEY_ICON_5,
  KEY_ICON_6,
  KEY_ICON_7,
  KEY_LABEL_0,
  KEY_LABEL_1,
  KEY_LABEL_2,
  KEY_LABEL_3,
  KEY_LABEL_4,
  KEY_LABEL_5,
  KEY_LABEL_6,
  KEY_LABEL_7,
  KEY_IDLE = IDLE_TOUCH,
} KEY_VALUES;

#define ITEM_PER_PAGE 8
#define LISTITEM_PER_PAGE 5

typedef union {
  uint32_t index;   // language index, address = textSelect(index);
  uint8_t *address;
} LABEL;

typedef struct {
  uint16_t icon;
  LABEL label;
} ITEM;

typedef struct {
  LABEL title;
  ITEM items[ITEM_PER_PAGE];
} MENUITEMS;

typedef enum {
  TIMED_STATUS = 0,
  TIMED_INFO,
  TIMED_WARNNG,
  TIMED_ERROR,
  TIMED_CRITICAL
} MESSAGE_TYPE;

typedef struct {
  GUI_RECT rect;
  uint32_t time;
  uint8_t status;
  int16_t inf;
} REMINDER;

typedef enum {
  LIST_LABEL = 0,
  LIST_TOGGLE,
  LIST_RADIO,
  LIST_MOREBUTTON,
  LIST_CUSTOMVALUE,
} LISTITEM_TYPE;

typedef struct {
  uint16_t icon;
  LISTITEM_TYPE itemType;
  LABEL titlelabel;
  LABEL valueLabel;
} LISTITEM;

typedef struct {
  LABEL title;
  LISTITEM items[ITEM_PER_PAGE];
} LISTITEMS;

typedef struct {
  char *message;
  uint16_t timeout;
  MESSAGE_TYPE type;
} TIMEDMESSAGE;

extern const GUI_RECT exhibitRect;

#define CENTER_Y ((exhibitRect.y1 - exhibitRect.y0) / 2 + exhibitRect.y0)
#define CENTER_X ((exhibitRect.x1 - exhibitRect.x0 - BYTE_WIDTH) / 2 + exhibitRect.x0)
#define LISTITEM_WIDTH (LCD_WIDTH - (3 * START_X) - LIST_ICON_WIDTH)
#define LISTITEM_HEIGHT ((LCD_HEIGHT - ICON_START_Y - START_X) / 5)
#define LISTICON_SPACE_Y ((LCD_HEIGHT - ICON_START_Y - START_X - (3 * LIST_ICON_HEIGHT)) / 2)

void GUI_RestoreColorDefault(void);
void menuDrawItem(const ITEM *menuItem, uint8_t positon);
void menuDrawIconOnly(const ITEM *item, uint8_t positon);
void menuDrawListItem(const LISTITEM *item, uint8_t positon);
void menuRefreshListPage(void);
void menuDrawTitle(const uint8_t *content);   //(const MENUITEMS * menuItems);
void menuDrawPage(const MENUITEMS *menuItems);
void menuDrawListPage(const LISTITEMS *listItems);
void itemDrawIconPress(uint8_t positon, uint8_t is_press);
KEY_VALUES menuKeyGetValue(void);
GUI_POINT getIconStartPoint(int index);

void timedMessage(uint8_t delay_secs, MESSAGE_TYPE type, char *string, ...);
void timedMessageExpire(void);

void processGcode(void);

void gcodeQueueStatus(void);

void drawXYZ(void);

void updateScreen(void);
void runUpdateLoop(void);

#endif
