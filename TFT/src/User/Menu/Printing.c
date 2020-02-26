#include "Printing.h"
#include "Router.h"
#include "includes.h"

//1title, ITEM_PER_PAGE item(icon + label)
MENUITEMS printingItems = {
    //  title
    LABEL_BACKGROUND,
    // icon                       label
    {
        {ICON_PAUSE, LABEL_PAUSE},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_STOP, LABEL_STOP},
        {ICON_ROUTER, LABEL_ROUTER},
        {ICON_PERCENTAGE, LABEL_PERCENTAGE_SPEED},
        {ICON_BABYSTEP, LABEL_BABYSTEP},
        {ICON_MOVE, LABEL_MOVE},
    }};

const ITEM itemIsPause[2] = {
    // icon                       label
    {ICON_PAUSE, LABEL_PAUSE},
    {ICON_RESUME, LABEL_RESUME},
};

const ITEM itemIsFinished[2] = {
    // icon                       label
    {ICON_STOP, LABEL_STOP},
    {ICON_BACK, LABEL_BACK},
};

#ifndef M27_WATCH_OTHER_SOURCES
#define M27_WATCH_OTHER_SOURCES false
#endif

#ifndef M27_REFRESH
#define M27_REFRESH 3
#endif

static PRINTING infoPrinting;
static u16 update_delay = M27_REFRESH * 100;

#ifdef ONBOARD_SD_SUPPORT
static bool update_waiting = M27_WATCH_OTHER_SOURCES;
#else
static bool update_waiting = false;
#endif

static u8 curIndex = 0;

//
bool isPrinting(void) {
  return infoPrinting.printing;
}

//
bool isPause(void) {
  return infoPrinting.pause;
}

bool isM0_Pause(void) {
  return infoPrinting.m0_pause;
}

//
void setPrintingTime(u32 RTtime) {
  if (RTtime % 100 == 0) {
    if (isPrinting() && !isPause()) {
      infoPrinting.time++;
    }
  }
}

//
void setPrintSize(u32 size) {
  infoPrinting.size = size;
}

//
void setPrintCur(u32 cur) {
  infoPrinting.cur = cur;
}

u8 getPrintProgress(void) {
  return infoPrinting.progress;
}
u32 getPrintTime(void) {
  return infoPrinting.time;
}

void printSetUpdateWaiting(bool isWaiting) {
  update_waiting = isWaiting;
}

void startGcodeExecute(void) {
  routerControl(routerMaxPWM[curIndex]);
}

void endGcodeExecute(void) {
  mustStoreCmd("G90\n");
  routerControl(0);
  mustStoreCmd(PRINT_END_GCODE);
}

//only return gcode file name except path
//for example:"SD:/test/123.gcode"
//only return "123.gcode"
u8* getCurGcodeName(char* path) {
  int i = strlen(path);
  for (; path[i] != '/' && i > 0; i--) {
  }
  return (u8*)(&path[i + 1]);
}

void menuBeforePrinting(void) {
  long size = 0;
  switch (infoFile.source) {
    case BOARD_SD:  // GCode from file on ONBOARD SD
      size = request_M23(infoFile.title + 5);

      //  if( powerFailedCreate(infoFile.title)==false)
      //  {
      //
      //  }	  // FIXME: Powerfail resume is not yet supported for ONBOARD_SD. Need more work.

      if (size == 0) {
        ExitDir();
        infoMenu.cur--;
        return;
      }

      infoPrinting.size = size;

      //    if(powerFailedExist())
      //    {
      request_M24(0);
      //    }
      //    else
      //    {
      //      request_M24(infoBreakPoint.offset);
      //    }
      printSetUpdateWaiting(true);

#ifdef M27_AUTOREPORT
      request_M27(M27_REFRESH);
#else
      request_M27(0);
#endif

      infoHost.printing = true;  // Global lock info on printer is busy in printing.
      break;

    case TFT_UDISK:
    case TFT_SD:  // GCode from file on TFT SD
      if (f_open(&infoPrinting.file, infoFile.title, FA_OPEN_EXISTING | FA_READ) != FR_OK) {
        ExitDir();
        infoMenu.cur--;
        return;
      }
      if (powerFailedCreate(infoFile.title) == false) {
      }
      powerFailedlSeek(&infoPrinting.file);

      infoPrinting.size = f_size(&infoPrinting.file);
      infoPrinting.cur = infoPrinting.file.fptr;
      if (infoSettings.send_start_gcode == 1) {
        startGcodeExecute();
      }
      break;
  }
  infoPrinting.printing = true;
  infoMenu.menu[infoMenu.cur] = menuPrinting;
  printingItems.title.address = getCurGcodeName(infoFile.title);
}

void resumeToPause(bool is_pause) {
  if (infoMenu.menu[infoMenu.cur] != menuPrinting) return;  // todo: make sure this runs during popups, too
  printingItems.items[KEY_ICON_0] = itemIsPause[is_pause];
  menuDrawItem(&itemIsPause[is_pause], 0);
}

void setM0Pause(bool m0_pause) {
  infoPrinting.m0_pause = m0_pause;
}

bool setPrintPause(bool is_pause, bool is_m0pause) {
  static bool pauseLock = false;
  if (pauseLock) return false;
  //if (!isPrinting()) return false;
  if (infoPrinting.pause == is_pause) return false;

  pauseLock = true;
  switch (infoFile.source) {
    case BOARD_SD:
      infoPrinting.pause = is_pause;
      if (is_pause) {
        infoPrinting.routerSpeed = routerGetSpeed(curIndex);
        routerControl(0);
        request_M25();
      } else {
        routerControl(infoPrinting.routerSpeed);
        request_M24(0);
      }
      break;

    case TFT_UDISK:
    case TFT_SD:
      infoPrinting.pause = is_pause;
      if (infoPrinting.pause == true && is_m0pause == false) {
        while (infoCmd.count != 0) {
          loopProcess();
        }
      }

      bool isCoorRelative = coorGetRelative();
      static COORDINATE tmp;

      if (infoPrinting.pause) {
        // *Restore status before pause
        // *if pause was triggered through M0/M1 then break
        if (is_m0pause == true) {
          u8 curRouterSpeed = routerGetSpeed(routerGetCurIndex(0));
          if (curRouterSpeed > 0) {
            routerControl(0);
          }
          mustStoreCmd("G53 G0 X20 Y200 Z100 F%d\n", SPEED_MOVE_FAST);
          mustStoreCmd("M0 Replace the bit and press Confirm when finished\n");
          storeCmd("G0 X0 Y0\n");
          if (curRouterSpeed > 0) {
            routerControl(curRouterSpeed);
          }

          setM0Pause(is_m0pause);
          popupReminder(textSelect(LABEL_PAUSE), textSelect(LABEL_M0_PAUSE));
          break;
        }

        coordinateGetAll(&tmp);
        if (isCoorRelative == true) mustStoreCmd("G90\n");
        if (coordinateIsClear()) {
          mustStoreCmd("G1 Z%.3f F%d\n", tmp.axis[Z_AXIS] + SPINDLE_PAUSE_Z_RAISE, SPINDLE_PAUSE_Z_GANTRYSPEED);
          mustStoreCmd("G1 X%d Y%d F%d\n", SPINDLE_PAUSE_X_POSITION, SPINDLE_PAUSE_Y_POSITION, SPINDLE_PAUSE_XY_GANTRYSPEED);
        }
        infoPrinting.routerSpeed = routerGetSpeed(curIndex);
        routerControl(infoPrinting.routerSpeed);

        if (isCoorRelative == true) mustStoreCmd("G91\n");
      } else {
        if (isM0_Pause() == true) {
          setM0Pause(is_m0pause);
          Serial_Puts(SERIAL_PORT, "M108\n");
          break;
        }
        if (isCoorRelative == true) mustStoreCmd("G90\n");

        if (coordinateIsClear()) {
          mustStoreCmd("G1 X%.3f Y%.3f F%d\n", tmp.axis[X_AXIS], tmp.axis[Y_AXIS], SPINDLE_PAUSE_XY_GANTRYSPEED);
          mustStoreCmd("G1 Z%.3f F%d\n", tmp.axis[Z_AXIS], SPINDLE_PAUSE_Z_GANTRYSPEED);
        }
        routerControl(infoPrinting.routerSpeed);
        mustStoreCmd("G1 F%d\n", tmp.gantryspeed);

        if (isCoorRelative == true) mustStoreCmd("G91\n");
      }
      break;
  }
  resumeToPause(is_pause);
  pauseLock = false;
  return true;
}

const GUI_RECT progressRect = {1 * SPACE_X_PER_ICON, 0 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + ICON_HEIGHT / 4,
                               3 * SPACE_X_PER_ICON, 0 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + ICON_HEIGHT * 3 / 4};

#define PRINT_STATUS_ROUTER_X (progressRect.x1 - 10 * BYTE_WIDTH)
#define PRINT_STATUS_SPEED_Y (progressRect.y1 + 3)
#define PRINT_STATUS_TIME_Y (PRINT_STATUS_SPEED_Y + 1 * BYTE_HEIGHT + 3)

void showPrintTime(void) {
  u8 hour = infoPrinting.time / 3600,
     min = infoPrinting.time % 3600 / 60,
     sec = infoPrinting.time % 60;
  GUI_SetNumMode(GUI_NUMMODE_ZERO);
  GUI_DispDec(progressRect.x0 + 2 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, hour, 2, RIGHT);
  GUI_DispDec(progressRect.x0 + 5 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, min, 2, RIGHT);
  GUI_DispDec(progressRect.x0 + 8 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, sec, 2, RIGHT);
  GUI_SetNumMode(GUI_NUMMODE_SPACE);
}

void showRouterSpeed(void) {
  //ROUTER - now only F0
  u8 fs;
#ifdef SHOW_ROUTER_PERCENTAGE
  fs = (routerGetSpeed(0) * 100) / 255;
#else
  fs = routerGetSpeed(0);
#endif
#ifdef SHOW_ROUTER_PERCENTAGE
  char router_s[15];
  sprintf(router_s, "Bit:%d%%", fs);
  GUI_DispString(PRINT_STATUS_ROUTER_X + 3 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y, (u8*)router_s);
#else
  GUI_DispDec(PRINT_STATUS_ROUTER_X + BYTE_WIDTH, PRINT_STATUS_SPEED_Y, fs, 3, LEFT);
#endif
}

void showPrintProgress(u8 progress) {
  char buf[5];
  const GUI_RECT percentageRect = {PRINT_STATUS_ROUTER_X, PRINT_STATUS_SPEED_Y - 3 * BYTE_HEIGHT, PRINT_STATUS_ROUTER_X + 10 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - 2 * BYTE_HEIGHT};
  u16 progressX = map(progress, 0, 100, percentageRect.x0, percentageRect.x1);
  GUI_FillRectColor(percentageRect.x0, percentageRect.y0, progressX, percentageRect.y1, BLUE);
  GUI_FillRectColor(progressX, percentageRect.y0, percentageRect.x1, percentageRect.y1, GRAY);
  my_sprintf(buf, "%d%%", progress);
  // GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_DispStringInPrect(&percentageRect, (u8*)buf);
  // GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
}

extern SCROLL titleScroll;
extern GUI_RECT titleRect;

void printingDrawPage(void) {
  // int16_t i;
  //	Scroll_CreatePara(&titleScroll, infoFile.title,&titleRect);  //
  // *Display print timer framework
  GUI_DispString(progressRect.x0, PRINT_STATUS_TIME_Y, (u8*)"T:");
  GUI_DispString(progressRect.x0 + 4 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, (u8*)":");
  GUI_DispString(progressRect.x0 + 7 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, (u8*)":");
  // *Display router speed
  // GUI_DispString(PRINT_STATUS_ROUTER_X + BYTE_WIDTH, PRINT_STATUS_SPEED_Y, (u8*)"Rtr:");
  showPrintProgress(infoPrinting.progress);
  showRouterSpeed();
  showPrintTime();
  // z_axis coordinate
  // GUI_DispString(PRINT_STATUS_ROUTER_X + BYTE_WIDTH, PRINT_STATUS_TIME_Y, (u8*)"Z:");

  if (get_Pre_Icon() == true) {
    //printingItems.items[key_pause - 1] = itemBlank;
    printingItems.items[KEY_ICON_1].icon = ICON_PREVIEW;
    printingItems.items[KEY_ICON_1].label.index = LABEL_BACKGROUND;
  }
  // else{
  //   printingItems.items[KEY_ICON_1] = itemBabyStep;
  // }
  menuDrawPage(&printingItems);

  // i = get_Pre_Icon((char *)getCurGcodeName(infoFile.title));
  // if(i != ICON_BACKGROUND)
  //   lcd_frame_display(1*ICON_WIDTH + 1 * SPACE_X + START_X, 0 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y, ICON_WIDTH, ICON_HEIGHT, ICON_ADDR(i));
}

void menuPrinting(void) {
  KEY_VALUES key_num = KEY_IDLE;
  u32 time = 0;

  printingItems.items[KEY_ICON_0] = itemIsPause[infoPrinting.pause];
  if (isPrinting())
    printingItems.items[KEY_ICON_3] = itemIsFinished[0];
  else
    printingItems.items[KEY_ICON_3] = itemIsFinished[1];

  printingDrawPage();
  // printingItems.items[key_pause] = itemIsPause[infoPrinting.pause];

  while (infoMenu.menu[infoMenu.cur] == menuPrinting) {
    //    Scroll_DispString(&titleScroll, LEFT); //Scroll display file name will take too many CPU cycles

    if (infoPrinting.size != 0) {
      if (infoPrinting.progress != limitValue(0, (uint64_t)infoPrinting.cur * 100 / infoPrinting.size, 100)) {
        infoPrinting.progress = limitValue(0, (uint64_t)infoPrinting.cur * 100 / infoPrinting.size, 100);
        showPrintProgress(infoPrinting.progress);
      }
    } else {
      if (infoPrinting.progress != 100) {
        infoPrinting.progress = 100;
        showPrintProgress(infoPrinting.progress);
      }
    }

    if (time != infoPrinting.time) {
      time = infoPrinting.time;
      showPrintTime();  // job timer
    }
    showRouterSpeed();  // router speed

    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        setPrintPause(!isPause(), false);
        break;

      case KEY_ICON_3:
        if (isPrinting())
          infoMenu.menu[++infoMenu.cur] = menuStopPrinting;
        else {
          exitPrinting();
          infoMenu.cur--;
        }
        break;

      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.cur] = menuRouter;
        break;

      case KEY_ICON_5:
        infoMenu.menu[++infoMenu.cur] = menuSpeed;
        break;

      case KEY_ICON_6:
        infoMenu.menu[++infoMenu.cur] = menuBabyStep;
        break;

      case KEY_ICON_7:
        infoMenu.menu[++infoMenu.cur] = menuMove;
        break;

      default:
        break;
    }
    loopProcess();
  }
}

void exitPrinting(void) {
  memset(&infoPrinting, 0, sizeof(PRINTING));
  ExitDir();
}

void endPrinting(void) {
  switch (infoFile.source) {
    case BOARD_SD:
      printSetUpdateWaiting(M27_WATCH_OTHER_SOURCES);
      break;

    case TFT_UDISK:
    case TFT_SD:
      f_close(&infoPrinting.file);
      break;
  }
  infoPrinting.printing = infoPrinting.pause = false;
  powerFailedClose();
  powerFailedDelete();
  if (infoSettings.send_end_gcode == 1) {
    endGcodeExecute();
  }
}

void completePrinting(void) {
  endPrinting();

  printingItems.items[KEY_ICON_3] = itemIsFinished[1];
  printingDrawPage();
  if (infoSettings.auto_off)  // Auto shut down after printing
  {
    infoMenu.menu[++infoMenu.cur] = menuShutDown;
  }
}

void abortPrinting(void) {
  switch (infoFile.source) {
    case BOARD_SD:
      request_M524();
      break;

    case TFT_UDISK:
    case TFT_SD:
      clearCmdQueue();
      break;
  }

  mustStoreCmd("G0 Z%d F3000\n", limitValue(0, (int)coordinateGetAxisTarget(Z_AXIS) + 10, Z_MAX_POS));
  if (strlen(CANCEL_CNC_GCODE) > 0)
    mustStoreCmd(CANCEL_CNC_GCODE);

  endPrinting();
  exitPrinting();
}

void menuStopPrinting(void) {
  u16 key_num = IDLE_TOUCH;

  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_STOP_CNC), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while (infoMenu.menu[infoMenu.cur] == menuStopPrinting) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        abortPrinting();
        infoMenu.cur -= 2;
        break;

      case KEY_POPUP_CANCEL:
        infoMenu.cur--;
        break;
    }
    loopProcess();
  }
}

// Shut down menu, when the hotend temperature is higher than "AUTO_SHUT_DOWN_MAXTEMP"
// wait for cool down, in the meantime, you can shut down by force
void menuShutDown(void) {
  bool tempIsLower;
  u16 key_num = IDLE_TOUCH;

  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_SHUT_DOWN), textSelect(LABEL_WAIT_TEMP_SHUT_DOWN), textSelect(LABEL_FORCE_SHUT_DOWN), textSelect(LABEL_CANCEL));

  for (u8 i = 0; i < ROUTER_NUM; i++) {
    mustStoreCmd("%s S255\n", routerCmd[i]);
  }
  while (infoMenu.menu[infoMenu.cur] == menuShutDown) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        goto shutdown;

      case KEY_POPUP_CANCEL:
        infoMenu.cur--;
        break;
    }
  shutdown:
    for (u8 i = 0; i < ROUTER_NUM; i++) {
      mustStoreCmd("%s S0\n", routerCmd[i]);
    }
    mustStoreCmd("M81\n");
    infoMenu.cur--;
    popupReminder(textSelect(LABEL_SHUT_DOWN), textSelect(LABEL_SHUTTING_DOWN));
    loopProcess();
  }
}

// get gcode command from sd card
void getGcodeFromFile(void) {
  bool sd_comment_mode = false;
  bool sd_comment_space = true;
  char sd_char;
  u8 sd_count = 0;
  UINT br = 0;

  if (isPrinting() == false || infoFile.source == BOARD_SD) return;

  powerFailedCache(infoPrinting.file.fptr);

  if (infoCmd.count || infoPrinting.pause) return;

  if (moveCacheToCmd() == true) return;

  for (; infoPrinting.cur < infoPrinting.size;) {
    if (f_read(&infoPrinting.file, &sd_char, 1, &br) != FR_OK) break;

    infoPrinting.cur++;

    //Gcode
    if (sd_char == '\n')  //'\n' is end flag for per command
    {
      sd_comment_mode = false;  //for new command
      sd_comment_space = true;
      if (sd_count != 0) {
        infoCmd.queue[infoCmd.index_w].gcode[sd_count++] = '\n';
        infoCmd.queue[infoCmd.index_w].gcode[sd_count] = 0;  //terminate string
        infoCmd.queue[infoCmd.index_w].src = SERIAL_PORT;
        sd_count = 0;  //clear buffer
        infoCmd.index_w = (infoCmd.index_w + 1) % CMD_MAX_LIST;
        infoCmd.count++;
        break;
      }
    } else if (sd_count >= CMD_MAX_CHAR - 2) {
    }  //when the command length beyond the maximum, ignore the following bytes
    else {
      if (sd_char == ';')  //';' is comment out flag
        sd_comment_mode = true;
      else {
        if (sd_comment_space && (sd_char == 'G' || sd_char == 'M' || sd_char == 'T'))  //ignore ' ' space bytes
          sd_comment_space = false;
        if (!sd_comment_mode && !sd_comment_space && sd_char != '\r')  //normal gcode
          infoCmd.queue[infoCmd.index_w].gcode[sd_count++] = sd_char;
      }
    }
  }

  if ((infoPrinting.cur >= infoPrinting.size) && isPrinting())  // end of .gcode file
  {
    completePrinting();
  }
}

void loopCheckPrinting(void) {
  static u32 nowTime = 0;

  do { /* WAIT FOR M27	*/
    if (update_waiting == true) {
      nowTime = OS_GetTime();
      break;
    }
    if (OS_GetTime() < nowTime + update_delay) break;

    if (storeCmd("M27\n") == false) break;

    nowTime = OS_GetTime();
    update_waiting = true;
  } while (0);
}
