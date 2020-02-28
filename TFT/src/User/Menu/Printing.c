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

PRINTING infoPrinting;
static u16 update_delay = M27_REFRESH * 100;

#ifdef ONBOARD_SD_SUPPORT
static bool update_waiting = M27_WATCH_OTHER_SOURCES;
#else
static bool update_waiting = false;
#endif

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
void setPrintCur(u32 line) {
  infoPrinting.currentLine = line;
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
  if (strlen(PRINT_START_GCODE) > 0)
    storeCmd(PRINT_START_GCODE);
  routerControl(ROUTER_MAX_PWM);
}

void endGcodeExecute(void) {
  mustStoreCmd("G90\n");
  routerControl(0);
  if (strlen(PRINT_END_GCODE) > 0)
    storeCmd(PRINT_END_GCODE);
}

//only return gcode file name except path
//for example:"SD:/test/123.gcode"
//only return "123.gcode"
u8* getGcodeFilename(char* path) {
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
        infoMenu.active--;
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
        infoMenu.active--;
        return;
      }
      if (powerFailedCreate(infoFile.title) == false) {
      }
      powerFailedlSeek(&infoPrinting.file);

      infoPrinting.size = f_size(&infoPrinting.file);
      infoPrinting.currentLine = infoPrinting.file.fptr;
      if (infoSettings.send_start_gcode == 1) {
        startGcodeExecute();
      }
      break;
  }
  infoPrinting.printing = true;
  infoMenu.menu[infoMenu.active] = menuPrinting;
  printingItems.title.address = getGcodeFilename(infoFile.title);
}

void resumeToPause(bool pauseCalled) {
  if (infoMenu.menu[infoMenu.active] != menuPrinting) return;
  printingItems.items[KEY_ICON_0] = itemIsPause[pauseCalled];
  menuDrawItem(&itemIsPause[pauseCalled], 0);
}

void setM0Pause(bool paused) {
  infoPrinting.m0_pause = paused;
}

bool setPrintPause(bool pauseCalled) {
  static bool pauseInProgress = false;
  extern u8 curRouterSpeed;
  if (pauseInProgress || infoPrinting.pause == pauseCalled) {
    return false;
  } else {
    pauseInProgress = true;
  }

  switch (infoFile.source) {
    case BOARD_SD:
      infoPrinting.pause = pauseCalled;
      if (pauseCalled) {
        curRouterSpeed = infoPrinting.routerSpeed;
        routerControl(0);
        request_M25();
      } else {
        routerControl(curRouterSpeed);
        request_M24(0);
      }
      break;

    case TFT_UDISK:
    case TFT_SD:
      infoPrinting.pause = pauseCalled;
      if (pauseCalled) {
        while (gcodeCommand.count != 0) {
          loopProcess();
        }
      }

      bool isCoorRelative = coorGetRelative();
      COORDINATE pauseCoords;

      if (pauseCalled) {
        // *Restore status before pause
        // *if pause was triggered through M0/M1 then break
        curRouterSpeed = infoPrinting.routerSpeed;
        routerControl(0);
        coordinateGetAll(&pauseCoords);
        if (isCoorRelative == true) mustStoreCmd("G90\n");
        if (coordinateIsClear()) {
          mustStoreCmd("G1 Z%.3f F%d\n", pauseCoords.axis[Z_AXIS] + SPINDLE_PAUSE_Z_RAISE, SPINDLE_PAUSE_Z_GANTRYSPEED);
          mustStoreCmd("G1 X%d Y%d F%d\n", SPINDLE_PAUSE_X_POSITION, SPINDLE_PAUSE_Y_POSITION, SPINDLE_PAUSE_XY_GANTRYSPEED);
        }

        if (isCoorRelative == true) mustStoreCmd("G91\n");
      } else {
        if (infoPrinting.m0_pause == true) {
          Serial_Puts(SERIAL_PORT, "M108\n");
          infoPrinting.m0_pause = false;
        }
        if (isCoorRelative == true) mustStoreCmd("G90\n");

        routerControl(curRouterSpeed);
        if (coordinateIsClear()) {
          mustStoreCmd("G1 X%.3f Y%.3f F%d\n", pauseCoords.axis[X_AXIS], pauseCoords.axis[Y_AXIS], SPINDLE_PAUSE_XY_GANTRYSPEED);
          mustStoreCmd("G1 Z%.3f F%d\n", pauseCoords.axis[Z_AXIS], SPINDLE_PAUSE_Z_GANTRYSPEED);
        }
        mustStoreCmd("G1 F%d\n", pauseCoords.gantryspeed);

        if (isCoorRelative == true) mustStoreCmd("G91\n");
      }
      break;
  }
  resumeToPause(pauseCalled);
  pauseInProgress = false;
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
  u8 Router_speed;
#ifdef SHOW_ROUTER_PERCENTAGE
  Router_speed = (infoPrinting.routerSpeed * 100) / 255;
#else
  Router_speed = infoPrinting.routerSpeed;
#endif
#ifdef SHOW_ROUTER_PERCENTAGE
  char text[15];
  sprintf(text, "Bit:%d%%", Router_speed);
  GUI_DispString(PRINT_STATUS_ROUTER_X, PRINT_STATUS_SPEED_Y, (u8*)text);
#else
  GUI_DispDec(PRINT_STATUS_ROUTER_X + BYTE_WIDTH, PRINT_STATUS_SPEED_Y, Router_speed, 3, LEFT);
#endif
}

void showCNCSpeed(void) {
  u8 CNC_speed = getCNCSpeedOverride();
  char text[15];
  sprintf(text, "CNC:%d%%", CNC_speed);
  GUI_DispString(PRINT_STATUS_ROUTER_X, PRINT_STATUS_SPEED_Y - BYTE_HEIGHT, (u8*)text);
}

void showPrintProgress(u8 progress) {
  // *Show graphical percent complete
  char buf[5];
  const GUI_RECT percentageRect = {PRINT_STATUS_ROUTER_X, PRINT_STATUS_SPEED_Y - 3 * BYTE_HEIGHT, PRINT_STATUS_ROUTER_X + 10 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - 2 * BYTE_HEIGHT};
  u16 progressX = map(progress, 0, 100, percentageRect.x0, percentageRect.x1);
  GUI_FillRectColor(percentageRect.x0, percentageRect.y0, progressX, percentageRect.y1, BLUE);
  GUI_FillRectColor(progressX, percentageRect.y0, percentageRect.x1, percentageRect.y1, GRAY);
  my_sprintf(buf, "%d%%", progress);
  // GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_DispStringInPrect(&percentageRect, (u8*)buf);
  // GUI_SetTextMode(GUI_TEXTMODE_NORMAL);

  // *Show numeric percent complete
  char text[15];
  sprintf(text, "Job:%d%%", progress);
  GUI_DispString(PRINT_STATUS_ROUTER_X, PRINT_STATUS_SPEED_Y - 2 * BYTE_HEIGHT, (u8*)text);
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

  // i = get_Pre_Icon((char *)getGcodeFilename(infoFile.title));
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

  while (infoMenu.menu[infoMenu.active] == menuPrinting) {
    //    Scroll_DispString(&titleScroll, LEFT); //Scroll display file name will take too many CPU cycles

    if (infoPrinting.size != 0) {
      if (infoPrinting.progress != limitValue(0, (uint64_t)infoPrinting.currentLine * 100 / infoPrinting.size, 100)) {
        infoPrinting.progress = limitValue(0, (uint64_t)infoPrinting.currentLine * 100 / infoPrinting.size, 100);
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
        setPrintPause(!isPause());
        break;

      case KEY_ICON_3:
        if (isPrinting())
          infoMenu.menu[++infoMenu.active] = menuStopPrinting;
        else {
          exitPrinting();
          infoMenu.active--;
        }
        break;

      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.active] = menuRouter;
        break;

      case KEY_ICON_5:
        infoMenu.menu[++infoMenu.active] = menuSpeed;
        break;

      case KEY_ICON_6:
        infoMenu.menu[++infoMenu.active] = menuBabyStep;
        break;

      case KEY_ICON_7:
        infoMenu.menu[++infoMenu.active] = menuMove;
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
    infoMenu.menu[++infoMenu.active] = menuShutDown;
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

  while (infoMenu.menu[infoMenu.active] == menuStopPrinting) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        abortPrinting();
        infoMenu.active -= 2;
        break;

      case KEY_POPUP_CANCEL:
        infoMenu.active--;
        break;
    }
    loopProcess();
  }
}

// Shut down menu, when the hotend temperature is higher than "AUTO_SHUT_DOWN_MAXTEMP"
// wait for cool down, in the meantime, you can shut down by force
void menuShutDown(void) {
  u16 key_num = IDLE_TOUCH;

  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_SHUT_DOWN), textSelect(LABEL_WAIT_TEMP_SHUT_DOWN), textSelect(LABEL_FORCE_SHUT_DOWN), textSelect(LABEL_CANCEL));

  while (infoMenu.menu[infoMenu.active] == menuShutDown) {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch (key_num) {
      case KEY_POPUP_CONFIRM:
        goto shutdown;

      case KEY_POPUP_CANCEL:
        infoMenu.active--;
        break;
    }
  shutdown:
    routerControl(0);
    mustStoreCmd("M81\n");
    infoMenu.active--;
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

  if (gcodeCommand.count || infoPrinting.pause) return;

  if (moveCacheToCmd() == true) return;

  for (; infoPrinting.currentLine < infoPrinting.size;) {
    if (f_read(&infoPrinting.file, &sd_char, 1, &br) != FR_OK) break;

    infoPrinting.currentLine++;

    //Gcode
    if (sd_char == '\n')  //'\n' is end flag for per command
    {
      sd_comment_mode = false;  //for new command
      sd_comment_space = true;
      if (sd_count != 0) {
        gcodeCommand.queue[gcodeCommand.index_w].gcode[sd_count++] = '\n';
        gcodeCommand.queue[gcodeCommand.index_w].gcode[sd_count] = 0;  //terminate string
        gcodeCommand.queue[gcodeCommand.index_w].src = SERIAL_PORT;
        sd_count = 0;  //clear buffer
        gcodeCommand.index_w = (gcodeCommand.index_w + 1) % GCODE_QUEUE_MAX;
        gcodeCommand.count++;
        break;
      }
    } else if (sd_count >= GCODE_MAX_CHARACTERS - 2) {
    }  //when the command length beyond the maximum, ignore the following bytes
    else {
      if (sd_char == ';')  //';' is comment out flag
        sd_comment_mode = true;
      else {
        if (sd_comment_space && (sd_char == 'G' || sd_char == 'M' || sd_char == 'T'))  //ignore ' ' space bytes
          sd_comment_space = false;
        if (!sd_comment_mode && !sd_comment_space && sd_char != '\r')  //normal gcode
          gcodeCommand.queue[gcodeCommand.index_w].gcode[sd_count++] = sd_char;
      }
    }
  }

  if ((infoPrinting.currentLine >= infoPrinting.size) && isPrinting())  // end of .gcode file
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
