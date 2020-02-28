#include "Speed.h"
#include "includes.h"

//1 title copy (copy title copy), ITEM_PER_PAGE copy title item (picture copy copy + signature copy)
MENUITEMS speedOverrideItems = {
    //   title
    LABEL_PERCENTAGE_SPEED,
    // icon                       label
    {
        {ICON_DEC, LABEL_DEC},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_INC, LABEL_INC},
        {ICON_MOVE, LABEL_PERCENTAGE_SPEED},
        {ICON_E_5_MM, LABEL_5_PERCENT},
        {ICON_NORMAL_SPEED, LABEL_NORMAL_SPEED},
        {ICON_BACK, LABEL_BACK},
    }};

#define ITEM_SPEEDOVERRIDE_NUM 2
const ITEM itemSpeedOverrideIcons[ITEM_SPEEDOVERRIDE_NUM] = {
    // icon                       label
    {ICON_MOVE, LABEL_PERCENTAGE_SPEED},
};
static int16_t itemSpeedOverrideTitle[ITEM_SPEEDOVERRIDE_NUM] = {
    LABEL_PERCENTAGE_SPEED};

static u16 percentage = 100;  // Speed

#define ITEM_PERCENT_INCREMENT 3
const ITEM itemIncrementIcons[ITEM_PERCENT_INCREMENT] = {
    // icon                       label
    {ICON_E_1_MM, LABEL_1_PERCENT},
    {ICON_E_5_MM, LABEL_5_PERCENT},
    {ICON_E_10_MM, LABEL_10_PERCENT},
};
const u8 itemIncrementValues[ITEM_PERCENT_INCREMENT] = {1, 5, 10};
static u8 itemIncrementIndex = 1;

/**
 * Override global CNC speed
 *
 * @version	v1.0.0	Thursday, February 27th, 2020.
 * @version	v1.0.1	Thursday, February 27th, 2020.
 * @global
 * @param	u16	percent
 * @return	void
 */
void setCNCSpeedOverride(u16 percent) {
  percentage = limitValue(10, percent, 999);
}

/**
 * Get global CNC speed override value (in percent)
 *
 * @version	v1.0.0	Thursday, February 27th, 2020.
 * @global
 * @param	u8	tool
 * @return	mixed
 */
u16 getCNCSpeedOverride(void) {
  return percentage;
}

void showCNCSpeedOverride(void) {
  GUI_DispDec(CENTER_X - 3 * BYTE_WIDTH / 2, CENTER_Y, percentage, 3, LEFT);
  GUI_DispString(CENTER_X - 3 * BYTE_WIDTH / 2 + 3 * BYTE_WIDTH, CENTER_Y, (u8 *)"%");
}
void redrawCNCSpeedOverride(void) {
  GUI_DispDec(CENTER_X - 3 * BYTE_WIDTH / 2, CENTER_Y, percentage, 3, LEFT);
}

void menuSpeed(void) {
  KEY_VALUES key_num = KEY_IDLE;
  u16 nowSpeed;
  nowSpeed = percentage;

  menuDrawPage(&speedOverrideItems);
  showCNCSpeedOverride();

#if LCD_ENCODER_SUPPORT
  encoderPosition = 0;
#endif

  while (infoMenu.menu[infoMenu.active] == menuSpeed) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        if (percentage > 10) {
          percentage =
              limitValue(10,
                         percentage - itemIncrementValues[itemIncrementIndex],
                         999);
        }
        break;

      case KEY_ICON_3:
        if (percentage < 999) {
          percentage =
              limitValue(10,
                         percentage + itemIncrementValues[itemIncrementIndex],
                         999);
        }
        break;

      case KEY_ICON_4:
        itemIncrementIndex = (itemIncrementIndex + 1) % ITEM_SPEEDOVERRIDE_NUM;
        speedOverrideItems.items[key_num] = itemSpeedOverrideIcons[itemIncrementIndex];
        menuDrawItem(&speedOverrideItems.items[key_num], key_num);
        speedOverrideItems.title.index = itemSpeedOverrideTitle[itemIncrementIndex];
        menuDrawTitle(textSelect(speedOverrideItems.title.index));
        showCNCSpeedOverride();
        break;

      case KEY_ICON_5:
        itemIncrementIndex = (itemIncrementIndex + 1) % ITEM_PERCENT_INCREMENT;
        speedOverrideItems.items[key_num] = itemIncrementIcons[itemIncrementIndex];
        menuDrawItem(&speedOverrideItems.items[key_num], key_num);
        break;
      case KEY_ICON_6:
        percentage = 100;
        break;
      case KEY_ICON_7:
        infoMenu.active--;
        break;
      default:
#if LCD_ENCODER_SUPPORT
        if (encoderPosition) {
          if (percentage < 999 && encoderPosition > 0) {
            percentage =
                limitValue(10,
                           percentage + itemIncrementValues[itemIncrementIndex],
                           999);
          }
          if (percentage > 10 && encoderPosition < 0) {
            percentage =
                limitValue(10,
                           percentage - itemIncrementValues[itemIncrementIndex],
                           999);
          }
          encoderPosition = 0;
        }
        LCD_LoopEncoder();
#endif
        break;
    }

    if (nowSpeed != percentage) {
      nowSpeed = percentage;
      storeCmd("M220 S%d\n", percentage);
      redrawCNCSpeedOverride();
    }
    loopProcess();
  }
}
