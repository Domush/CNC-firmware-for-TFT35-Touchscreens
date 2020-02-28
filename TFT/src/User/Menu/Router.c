#include "Router.h"
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

const char* routerID = ROUTER_ID;
const u8 routerMaxPWM = ROUTER_MAX_PWM;

extern PRINTING infoPrinting;

/**
 * routerControl.
 *
 * @version	v1.0.0	Thursday, February 27th, 2020.
 * @global
 * @param	u8	speed	// 0-255
 * @return	void
 */
void routerControl(u8 speed) {
  static COORDINATE tmp;
  coordinateGetAll(&tmp);
  if ((tmp.axis[X_AXIS] < 0 || tmp.axis[Y_AXIS] < 0) && speed > 0) {  // *Don't allow power up while gantry is below X/Y zero
    popupReminder((u8*)"Caution: Unsafe gantry position!", (u8*)"Cannot power up router while X/Y axis is negative.\n\nZero X/Y axis to enable.");
    return;
  }

  if (speed > 255) {
    speed = 255;
  }
  switch (infoSettings.router_power) {
    case 1:
      if (!speed || speed < 1) {
        if (infoPrinting.m0_pause) {
          Serial_Puts(SERIAL_PORT, "M5\n");
        } else {
          storeCmd("M5\n");
        }
      } else {
        if (infoPrinting.m0_pause) {
          Serial_Puts(SERIAL_PORT, "M3 S%d\n", speed);
        } else {
          storeCmd("M3 S%d\n", speed);
        }
      }
      infoPrinting.routerSpeed = speed;
      break;

    case 2:
      if (!speed || speed < 1) {
        if (infoPrinting.m0_pause) {
          Serial_Puts(SERIAL_PORT, "M107\n");
        } else {
          storeCmd("M107\n");
        }
      } else {
        if (infoPrinting.m0_pause) {
          Serial_Puts(SERIAL_PORT, "M106 S%d\n", speed);
        } else {
          storeCmd("M106 S%d\n", speed);
        }
      }
      infoPrinting.routerSpeed = speed;
      break;

    default:
      break;
  }
}
void routerChangeBit(void) {
  u8 speed = infoPrinting.routerSpeed;
  if (speed > 0) {
    routerControl(0);
  }
  if (infoPrinting.coordSpace < 53) infoPrinting.coordSpace = 53;
  mustStoreCmd("G59\n");
  mustStoreCmd("G0 X20 Y200 Z150 F%d\n", SPEED_MOVE_FAST);
  mustStoreCmd("M0 Please replace the bit. Continue when finished.");
  infoPrinting.m0_pause = true;
  // storeCmd("G%d\n", infoPrinting.coordSpace);
  // storeCmd("G0 X0 Y0\n");
  // if (speed > 0) {
  //   routerControl(speed);
  // }
}

void routerShowSpeed(void) {
  const GUI_RECT rect = {exhibitRect.x0, CENTER_Y - BYTE_HEIGHT, exhibitRect.x1, CENTER_Y};
  u8 fs;
#ifdef SHOW_ROUTER_PERCENTAGE
  fs = (infoPrinting.routerSpeed * 100) / 255;
#else
  fs = infoPrinting.routerSpeed
#endif
  GUI_ClearRect(rect.x0, rect.y0, rect.x1, rect.y1);
  GUI_DispStringInPrect(&rect, (u8*)routerID);
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
  sprintf(router_s, "%3d%%", (infoPrinting.routerSpeed * 100) / 255);
  GUI_DispString(CENTER_X - BYTE_WIDTH, CENTER_Y, (u8*)router_s);
#else
  GUI_DispDec(CENTER_X - BYTE_WIDTH, CENTER_Y, infoPrinting.routerSpeed, 3, LEFT);
#endif
}

void menuRouter(void) {
  u8 nowRouterSpeed = infoPrinting.routerSpeed;
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&routerItems);
  routerShowSpeed();
  while (infoMenu.menu[infoMenu.active] == menuRouter) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:  // Router speed --
        if (infoPrinting.routerSpeed > 0) {
#ifdef SHOW_ROUTER_PERCENTAGE
          if ((infoPrinting.routerSpeed - 2) > 0) {
            infoPrinting.routerSpeed -= 2;  //2.55 is 1 percent, rounding down
          } else {
            infoPrinting.routerSpeed = 0;
          }
#else
          infoPrinting.routerSpeed--;
#endif
        }
        break;

      case KEY_ICON_3:  // Router speed ++
        if (infoPrinting.routerSpeed < routerMaxPWM) {
#ifdef SHOW_ROUTER_PERCENTAGE
          if (infoPrinting.routerSpeed + 2 <= routerMaxPWM) {
            infoPrinting.routerSpeed += 2;  //2.55 is 1 percent, rounding down
          } else {
            infoPrinting.routerSpeed = routerMaxPWM;
          }
#else
          infoPrinting.routerSpeed++;
#endif
        }
        break;

      case KEY_ICON_4:  // Router off
        if (isPrinting() && !isPause()) {
          popupReminder((u8*)"Not allowed", (u8*)"To avoid bit damage, stopping the router is not allowed during a print.\n\nPause the print first");
        } else {
          routerControl(0);
        }
        break;

      case KEY_ICON_5:  // Swap bits
        if (isPrinting() && !isPause()) {
          popupReminder((u8*)"Not allowed", (u8*)"Cannot change bits while print in progress.\n\nPause print to enable bit change");
        } else {
          // routerChangeBit();
          infoMenu.menu[infoMenu.active] = menuChangeBit;
          // menuChangeBit();
        }
        break;

      case KEY_ICON_6:  // Router on
        routerControl(routerMaxPWM);
        break;

      case KEY_ICON_7:  // Back
        infoMenu.active--;
        break;

      default:
        break;
    }
    if (nowRouterSpeed != infoPrinting.routerSpeed) {
      routerControl(infoPrinting.routerSpeed);
      nowRouterSpeed = infoPrinting.routerSpeed;
      routerSpeedReDraw();
    }
    loopProcess();
  }
}
