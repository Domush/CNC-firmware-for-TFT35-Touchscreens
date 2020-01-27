#include "ProbeOffset.h"
#include "includes.h"

//1 titl, ITEM_PER_PAGE item
MENUITEMS zeroAxisItems = {
    // title
    LABEL_ZERO_ALL,
    // icon                        label
    {
        {ICON_DEC, LABEL_DEC},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_INC, LABEL_INC},
        {ICON_EEPROM_SAVE, LABEL_EEPROM_SAVE},
        {ICON_01_MM, LABEL_01_MM},
        {ICON_NORMAL_SPEED, LABEL_VALUE_ZERO},
        {ICON_BACK, LABEL_BACK},
    }};

typedef struct
{
  const ITEM *list;
  const float *ele;
  uint8_t cur;
  uint8_t totaled;
} ELEMENTS;

#define ITEM_ZERO_ALL_UNIT_NUM 3
const ITEM itemZeroAxisUnit[ITEM_ZERO_ALL_UNIT_NUM] = {
    // icon                       label
    {ICON_001_MM, LABEL_001_MM},
    {ICON_01_MM, LABEL_01_MM},
    {ICON_1_MM, LABEL_1_MM},
};
const float item_zeroAxis_unit[ITEM_ZERO_ALL_UNIT_NUM] = {0.01f, 0.1f, 1};

static ELEMENTS elementsUnit;

static void initElements(u8 position) {
  elementsUnit.totaled = ITEM_ZERO_ALL_UNIT_NUM;
  elementsUnit.list = itemZeroAxisUnit;
  elementsUnit.ele = item_zeroAxis_unit;

  for (u8 i = 0; i < elementsUnit.totaled; i++) {
    if (memcmp(&elementsUnit.list[i], &zeroAxisItems.items[position], sizeof(ITEM)) == 0) {
      elementsUnit.cur = i;
      break;
    }
  }
}

static float ZERO_ALL_value = 0.0;

#define ZERO_ALL_MAX_VALUE 20.0f
#define ZERO_ALL_MIN_VALUE -20.0f

void showZeroAxis(void) {
  GUI_DispFloat(CENTER_X - 5 * BYTE_WIDTH / 2, CENTER_Y, ZERO_ALL_value, 3, 2, RIGHT);
}
void zeroAxisReDraw(void) {
  GUI_DispFloat(CENTER_X - 5 * BYTE_WIDTH / 2, CENTER_Y, ZERO_ALL_value, 3, 2, RIGHT);
}

/* Set current offset */
void setCurrentOffset(float offset) {
  //ZERO_ALL_value = limitValue(ZERO_ALL_MIN_VALUE, offset, ZERO_ALL_MAX_VALUE);
  ZERO_ALL_value = offset;
}

void menuZeroAxis(void) {
  KEY_VALUES key_num = KEY_IDLE;
  float now = ZERO_ALL_value;
  initElements(KEY_ICON_5);
  menuDrawPage(&zeroAxisItems);
  showZeroAxis();
  while (infoMenu.menu[infoMenu.cur] == menuZeroAxis) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        if (ZERO_ALL_value - elementsUnit.ele[elementsUnit.cur] > ZERO_ALL_MIN_VALUE) {
          if (storeCmd("M851 Z%.2f\n", ZERO_ALL_value - elementsUnit.ele[elementsUnit.cur]))
            ZERO_ALL_value -= elementsUnit.ele[elementsUnit.cur];
        }
        break;
      case KEY_ICON_3:
        if (ZERO_ALL_value + elementsUnit.ele[elementsUnit.cur] < ZERO_ALL_MAX_VALUE) {
          if (storeCmd("M851 Z%.2f\n", ZERO_ALL_value + elementsUnit.ele[elementsUnit.cur]))
            ZERO_ALL_value += elementsUnit.ele[elementsUnit.cur];
        }
        break;
      case KEY_ICON_4:
        storeCmd("M500\n");
        break;
      case KEY_ICON_5:
        elementsUnit.cur = (elementsUnit.cur + 1) % elementsUnit.totaled;
        zeroAxisItems.items[key_num] = elementsUnit.list[elementsUnit.cur];
        menuDrawItem(&zeroAxisItems.items[key_num], key_num);
        break;
      case KEY_ICON_6:
        if (storeCmd("M851 Z%.2f\n", 0))
          ZERO_ALL_value = 0.0f;
        break;
      case KEY_ICON_7:
        infoMenu.cur--;
        break;
      default:
        break;
    }
    if (now != ZERO_ALL_value) {
      now = ZERO_ALL_value;
      zeroAxisReDraw();
    }
    loopProcess();
  }
}
