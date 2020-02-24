#include "Fan.h"
#include "Move.h"
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon+label)
const MENUITEMS routerItems = {
    //   title
    LABEL_ROUTER,
    // icon                       label
    {
        {ICON_DEC, LABEL_DEC},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_INC, LABEL_INC},
        {ICON_ROUTER_OFF, LABEL_ROUTER_OFF},
        {ICON_CHANGE_BIT, LABEL_CHANGE_BIT},
        {ICON_ROUTER_ON, LABEL_ROUTER_ON},
        {ICON_BACK, LABEL_BACK},
    }};

const char* routerID[] = ROUTER_ID;
const char* routerCmd[] = ROUTER_CMD;
const u8 routerMaxPWM[] = ROUTER_MAX_PWM;
static u8 curIndex = 0;

static u8 routerSpeed[ROUTER_NUM] = {0};
static bool send_waiting[ROUTER_NUM] = {false};

void routerSetSpeed(u8 i, u8 speed) {
  routerSpeed[i] = speed;
}

u8 routerGetSpeed(u8 i) {
  return routerSpeed[i];
}

void routerSetCurIndex(u8 i) {
  if (i >= ROUTER_NUM) return;
  curIndex = i;
}

u8 routerGetCurIndex(u8 i) {
  return curIndex;
}

void routerSetSendWaiting(u8 i, bool isWaiting) {
  send_waiting[i] = isWaiting;
}

void routerControl(u8 speed) {
  if (speed > 255) {
    speed = 255;
  }
  switch (infoSettings.router_power) {
    case 1:
      if (!speed || speed < 1) {
        storeCmd("M5\n");
      } else {
        storeCmd("M3 S%d\n", speed);
      }
      break;

    case 2:
      if (!speed || speed < 1) {
        storeCmd("M107\n");
      } else {
        storeCmd("M106 S%d\n", speed);
      }
      break;

    default:
      break;
  }
}
void routerChangeBit(void) {
  u8 speed = routerGetSpeed(routerGetCurIndex(0));
  if (speed > 0) {
    routerControl(0);
  }
  mustStoreCmd("G53 G0 X20 Y200 Z100 F%d\n", SPEED_MOVE_FAST);
  mustStoreCmd("M0 Replace the bit and press Confirm when finished\n");
  storeCmd("G0 X0 Y0\n");
  if (speed > 0) {
    routerControl(speed);
  }
}

void showRouterSpeed(void) {
  const GUI_RECT rect = {exhibitRect.x0, CENTER_Y - BYTE_HEIGHT, exhibitRect.x1, CENTER_Y};
  u8 fs;
#ifdef SHOW_ROUTER_PERCENTAGE
  fs = (routerSpeed[curIndex] * 100) / 255;
#else
  fs = routerSpeed[curIndex]
#endif
  GUI_ClearRect(rect.x0, rect.y0, rect.x1, rect.y1);
  GUI_DispStringInPrect(&rect, (u8*)routerID[curIndex]);
#ifdef SHOW_ROUTER_PERCENTAGE
  char router_s[5];
  sprintf(router_s, "%3d%%", fs);
  GUI_DispString(CENTER_X - BYTE_WIDTH, CENTER_Y, (u8*)router_s);
#else
  GUI_DispDec(CENTER_X - BYTE_WIDTH, CENTER_Y, fs, 3, LEFT);
#endif
}

void routerSpeedReDraw(void) {
#ifdef SHOW_ROUTER_PERCENTAGE
  char router_s[5] = "";
  sprintf(router_s, "%3d%%", (routerSpeed[curIndex] * 100) / 255);
  GUI_DispString(CENTER_X - BYTE_WIDTH, CENTER_Y, (u8*)router_s);
#else
  GUI_DispDec(CENTER_X - BYTE_WIDTH, CENTER_Y, routerSpeed[curIndex];, 3, LEFT);
#endif
}

void menuRouter(void) {
  u8 nowIndex = curIndex;
  u8 nowRouterSpeed[ROUTER_NUM];
  memcpy(nowRouterSpeed, routerSpeed, sizeof(routerSpeed));
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&routerItems);
  showRouterSpeed();
  while (infoMenu.menu[infoMenu.cur] == menuRouter) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:  // Router speed --
        if (routerSpeed[curIndex] > 0) {
#ifdef SHOW_ROUTER_PERCENTAGE
          if ((routerSpeed[curIndex] - 2) > 0) {
            routerSpeed[curIndex] -= 2;  //2.55 is 1 percent, rounding down
          } else {
            routerSpeed[curIndex] = 0;
          }
#else
          routerSpeed[curIndex]--;
#endif
        }
        break;

      case KEY_ICON_3:  // Router speed ++
        if (routerSpeed[curIndex] < routerMaxPWM[curIndex]) {
#ifdef SHOW_ROUTER_PERCENTAGE
          if (routerSpeed[curIndex] + 2 <= routerMaxPWM[curIndex]) {
            routerSpeed[curIndex] += 2;  //2.55 is 1 percent, rounding down
          } else {
            routerSpeed[curIndex] = routerMaxPWM[curIndex];
          }
#else
          routerSpeed[curIndex]++;
#endif
        }
        break;

      case KEY_ICON_4:  // Router off
        routerControl(0);
        break;

      case KEY_ICON_5:  // Swap bits
        routerChangeBit();
        break;

      case KEY_ICON_6:  // Router on
        routerControl(routerMaxPWM[curIndex]);
        break;

      case KEY_ICON_7:  // Back
        infoMenu.cur--;
        break;

      default:
        break;
    }

    if (nowIndex != curIndex) {
      nowIndex = curIndex;
      showRouterSpeed();
    }
    if (nowRouterSpeed[curIndex] != routerSpeed[curIndex]) {
      nowRouterSpeed[curIndex] = routerSpeed[curIndex];
      routerSpeedReDraw();
      if (send_waiting[curIndex] != true) {
        send_waiting[curIndex] = true;
        storeCmd("%s ", routerCmd[curIndex]);
      }
    }
    loopProcess();
  }
}
