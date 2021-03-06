#include "BabyStep.h"
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon+label)
MENUITEMS babyStepItems = {
    //title
    LABEL_BABYSTEP,
    //icon                        label
    {
        {ICON_DEC, LABEL_DEC},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_INC, LABEL_INC},
        {ICON_JOBSETUP, LABEL_JOBSETUP},
        {ICON_1_MM, LABEL_1_MM},
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

#define ITEM_BABYSTEP_UNIT_NUM 2
const ITEM itemBabyStepUnit[ITEM_BABYSTEP_UNIT_NUM] = {
    // icon                       label
    {ICON_01_MM, LABEL_01_MM},
    {ICON_1_MM, LABEL_1_MM},
};
const float item_babystep_unit[ITEM_BABYSTEP_UNIT_NUM] = {0.1f, 1};

static ELEMENTS elementsUnit;
extern PRINTING infoPrinting;

static void initElements(u8 position) {
  elementsUnit.totaled = ITEM_BABYSTEP_UNIT_NUM;
  elementsUnit.list    = itemBabyStepUnit;
  elementsUnit.ele     = item_babystep_unit;

  for (u8 i = 0; i < elementsUnit.totaled; i++) {
    if (memcmp(&elementsUnit.list[i], &babyStepItems.items[position], sizeof(ITEM)) == 0) {
      elementsUnit.cur = i;
      break;
    }
  }
}

#define BABYSTEP_MAX_VALUE 9.0f
#define BABYSTEP_MIN_VALUE -9.0f

void showBabyStep(void) {
  GUI_DispFloat(CENTER_X - 5 * BYTE_WIDTH / 2, CENTER_Y, infoPrinting.babyStep, 3, 2, RIGHT);
}
void babyStepReDraw(void) {
  GUI_DispFloat(CENTER_X - 5 * BYTE_WIDTH / 2, CENTER_Y, infoPrinting.babyStep, 3, 2, RIGHT);
}

void menuBabyStep(void) {
  KEY_VALUES key_num = KEY_IDLE;
  float curBabyStep  = infoPrinting.babyStep;

  initElements(KEY_ICON_5);
  menuDrawPage(&babyStepItems);
  showBabyStep();

#if LCD_ENCODER_SUPPORT
  encoderPosition = 0;
#endif

  while (infoMenu.menu[infoMenu.active] == menuBabyStep) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        if (infoPrinting.babyStep - elementsUnit.ele[elementsUnit.cur] > BABYSTEP_MIN_VALUE) {
          if (storeCmd("M290 Z-%.2f\n", elementsUnit.ele[elementsUnit.cur])) {
            infoPrinting.babyStep -= elementsUnit.ele[elementsUnit.cur];
            timedMessage(2, TIMED_INFO, "Decreasing Z height by %fmm", elementsUnit.ele[elementsUnit.cur]);
          }
        }
        break;
      case KEY_ICON_3:
        if (infoPrinting.babyStep + elementsUnit.ele[elementsUnit.cur] < BABYSTEP_MAX_VALUE) {
          if (storeCmd("M290 Z%.2f\n", elementsUnit.ele[elementsUnit.cur])) {
            infoPrinting.babyStep += elementsUnit.ele[elementsUnit.cur];
            timedMessage(2, TIMED_INFO, "Increasing Z height by %fmm", elementsUnit.ele[elementsUnit.cur]);
          }
        }
        break;
      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.active] = menuJobSetup;
        // storeCmd("M500\n");
        break;
      case KEY_ICON_5:
        elementsUnit.cur             = (elementsUnit.cur + 1) % elementsUnit.totaled;
        babyStepItems.items[key_num] = elementsUnit.list[elementsUnit.cur];
        menuDrawItem(&babyStepItems.items[key_num], key_num);
        break;
      case KEY_ICON_6:
        if (storeCmd("M290 Z%.2f\n", -infoPrinting.babyStep)) {
          infoPrinting.babyStep = 0.0f;
          timedMessage(2, TIMED_INFO, "Resetting Z height");
        }
        break;
      case KEY_ICON_7:
        infoMenu.active--;
        break;
      default:
#if LCD_ENCODER_SUPPORT
        if (encoderPosition) {
          infoPrinting.babyStep += elementsUnit.ele[elementsUnit.cur] * encoderPosition;
          encoderPosition = 0;
        }
        LCD_LoopEncoder();
#endif
        break;
    }
    if (curBabyStep != infoPrinting.babyStep) {
      curBabyStep = infoPrinting.babyStep;
      babyStepReDraw();
    }
    runUpdateLoop();
  }
}
