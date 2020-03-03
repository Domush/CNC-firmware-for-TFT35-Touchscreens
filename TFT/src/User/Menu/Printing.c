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

// *Return gCode filename with NO path
// Example: "SD:/CNC/Clocks/Bird/Profile 1.gcode"
// Will return: "Profile 1.gcode"
u8* getGcodeFilename(char* path) {
  int i = strlen(path);
  for (; path[i] != '/' && i > 0; i--) {
  }
  return (u8*)(&path[i + 1]);
}

// *Return gCode filename with LAST path
// Example: "SD:/CNC/Clocks/Bird/Profile 1.gcode"
// Will return: "Bird/Profile 1.gcode"
u8* getGcodePathFilename(char* fullFilename) {
  int i = strlen(fullFilename) - 6;  // -6 ignores the .gcode extension
  int pathsFound = 0;
  for (; fullFilename[i] != '/' && i > 0 && pathsFound < 2; i--) {
    if (fullFilename[i] == '/') pathsFound++;
  }
  return (u8*)(&fullFilename[i + 1]);
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
  printingItems.title.address = getGcodePathFilename(infoFile.title);
}

void resumeToPause(bool pauseCalled) {
  if (infoMenu.menu[infoMenu.active] != menuPrinting) return;
  printingItems.items[KEY_ICON_0] = itemIsPause[pauseCalled];
  menuDrawItem(&itemIsPause[pauseCalled], 0);
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
          timedMessage(1, TIMED_WARNNG, "Finishing gCode queue");
          runUpdateLoop();
        }
      }

      bool isCoorRelative = coorGetRelative();
      COORDINATE pauseCoords;

      if (pauseCalled) {
        timedMessage(3, TIMED_INFO, "Pausing CNC");
        curRouterSpeed = infoPrinting.routerSpeed;  // *Save current router speed
        routerControl(0);                           // *turn off the router
        coordinateGetAll(&pauseCoords);             // *save the current gantry position
        if (isCoorRelative == true) mustStoreCmd("G90\n");
        if (coordinateIsClear()) {  // *move the gantry into paused position
          mustStoreCmd("G1 Z%.3f F%d\n", pauseCoords.axis[Z_AXIS] + SPINDLE_PAUSE_Z_RAISE, SPINDLE_PAUSE_Z_GANTRYSPEED);
          mustStoreCmd("G1 X%d Y%d F%d\n", SPINDLE_PAUSE_X_POSITION, SPINDLE_PAUSE_Y_POSITION, SPINDLE_PAUSE_XY_GANTRYSPEED);
        }

        if (isCoorRelative == true) mustStoreCmd("G91\n");
      } else {
        timedMessage(3, TIMED_INFO, "Resuming CNC");
        if (infoPrinting.m0_pause == true) {
          Serial_Puts(SERIAL_PORT, "M108\n");
          infoPrinting.m0_pause = false;
        }
        if (isCoorRelative == true) mustStoreCmd("G90\n");

        routerControl(curRouterSpeed);  // *resume previous router speed
        if (coordinateIsClear()) {      // *restore previous gantry position
          mustStoreCmd("G1 X%.3f Y%.3f F%d\n", pauseCoords.axis[X_AXIS], pauseCoords.axis[Y_AXIS], SPINDLE_PAUSE_XY_GANTRYSPEED);
          mustStoreCmd("G1 Z%.3f F%d\n", pauseCoords.axis[Z_AXIS], SPINDLE_PAUSE_Z_GANTRYSPEED);
        }
        mustStoreCmd("G1 F%d\n", pauseCoords.gantryspeed);

        if (isCoorRelative == true) mustStoreCmd("G91\n");
      }
      break;
  }
  resumeToPause(pauseCalled);  // *change the pause/resume icon
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
  if (hour > 0) {
    GUI_RestoreColorDefault();
    GUI_DispString(PRINT_STATUS_ROUTER_X + 3 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, (u8*)"h");
    GUI_SetColor(YELLOW);
    GUI_DispDec(PRINT_STATUS_ROUTER_X + 1 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, hour, 2, RIGHT);
  }
  if (min > 0) {
    GUI_RestoreColorDefault();
    GUI_DispString(PRINT_STATUS_ROUTER_X + 6 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, (u8*)"m");
    GUI_SetColor(YELLOW);
    GUI_DispDec(PRINT_STATUS_ROUTER_X + 4 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, min, 2, RIGHT);
  }
  GUI_RestoreColorDefault();
  GUI_DispString(PRINT_STATUS_ROUTER_X + 9 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, (u8*)"s");
  GUI_SetColor(YELLOW);
  GUI_DispDec(PRINT_STATUS_ROUTER_X + 7 * BYTE_WIDTH, PRINT_STATUS_TIME_Y, sec, 2, RIGHT);
  GUI_RestoreColorDefault();
}

void showPrintTimeUpper(void) {
  u8 hour = infoPrinting.time / 3600,
     min = infoPrinting.time % 3600 / 60,
     sec = infoPrinting.time % 60;
  if (hour > 0) {
    GUI_RestoreColorDefault();
    GUI_DispString(LCD_WIDTH - 8 * BYTE_WIDTH, 0, (u8*)"h");
    GUI_SetColor(YELLOW);
    GUI_DispDec(LCD_WIDTH - 10 * BYTE_WIDTH, 0, hour, 2, RIGHT);
  }
  if (min > 0) {
    GUI_RestoreColorDefault();
    GUI_DispString(LCD_WIDTH - 5 * BYTE_WIDTH, 0, (u8*)"m");
    GUI_SetColor(YELLOW);
    GUI_DispDec(LCD_WIDTH - 7 * BYTE_WIDTH, 0, min, 2, RIGHT);
  }
  GUI_RestoreColorDefault();
  GUI_DispString(LCD_WIDTH - 2 * BYTE_WIDTH, 0, (u8*)"s");
  GUI_SetColor(YELLOW);
  GUI_DispDec(LCD_WIDTH - 4 * BYTE_WIDTH, 0, sec, 2, RIGHT);
  GUI_RestoreColorDefault();
}

void showBabyStepValue(void) {
  if (infoPrinting.babyStep != 0) {
    GUI_SetColor(MAT_LOWWHITE);
    GUI_DispString(PRINT_STATUS_ROUTER_X + 0 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - BYTE_HEIGHT * 2, (u8*)"Adj:");
    GUI_RestoreColorDefault();
    GUI_DispFloat(PRINT_STATUS_ROUTER_X + 5 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - BYTE_HEIGHT * 2, infoPrinting.babyStep, 2, 1, RIGHT);
    // GUI_SetColor(MAT_LOWWHITE);
    // GUI_DispString(PRINT_STATUS_ROUTER_X + 8 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - BYTE_HEIGHT * 2, (u8*)"mm");
    GUI_RestoreColorDefault();
  }
}

void showCNCSpeed(void) {
  u8 cncSpeed = getCNCSpeedOverride();
  GUI_SetColor(MAT_LOWWHITE);
  GUI_DispString(PRINT_STATUS_ROUTER_X + 0 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - BYTE_HEIGHT, (u8*)"CNC:");
  GUI_RestoreColorDefault();
  GUI_DispDec(PRINT_STATUS_ROUTER_X + 5 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - BYTE_HEIGHT, cncSpeed, 3, RIGHT);
  GUI_SetColor(MAT_LOWWHITE);
  GUI_DispString(PRINT_STATUS_ROUTER_X + 9 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - BYTE_HEIGHT, (u8*)"%");
  GUI_RestoreColorDefault();
}

void showRouterSpeed(void) {
  u8 routerSpeedPercent;
  routerSpeedPercent = (infoPrinting.routerSpeed * 100) / 255;
  GUI_SetColor(MAT_LOWWHITE);
  GUI_DispString(PRINT_STATUS_ROUTER_X + 0 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y, (u8*)"Bit:");
  GUI_RestoreColorDefault();
  GUI_DispDec(PRINT_STATUS_ROUTER_X + 5 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y, routerSpeedPercent, 3, RIGHT);
  GUI_SetColor(MAT_LOWWHITE);
  GUI_DispString(PRINT_STATUS_ROUTER_X + 9 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y, (u8*)"%");
  GUI_RestoreColorDefault();
}

void showPrintProgress(u8 progress) {
  static u8 lastProgress = 100;
  // *Show graphical percent complete
  if (lastProgress != progress) {
    char buf[5];
    const GUI_RECT percentageRect = {PRINT_STATUS_ROUTER_X, PRINT_STATUS_SPEED_Y - 3 * BYTE_HEIGHT, PRINT_STATUS_ROUTER_X + 10 * BYTE_WIDTH, PRINT_STATUS_SPEED_Y - 2 * BYTE_HEIGHT};
    u16 progressX = map(progress, 0, 100, percentageRect.x0, percentageRect.x1);
    GUI_FillRectColor(percentageRect.x0, percentageRect.y0, progressX, percentageRect.y1, BLUE);
    GUI_FillRectColor(progressX, percentageRect.y0, percentageRect.x1, percentageRect.y1, GRAY);
    // *Show numeric percent complete
    my_sprintf(buf, "%d%%", progress);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetColor(YELLOW);
    GUI_DispStringInPrect(&percentageRect, (u8*)buf);
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
    GUI_RestoreColorDefault();
    lastProgress = progress;
  }
}

void showIconPrintProgress(u8 progress) {
  static u8 lastIconProgress = 100;
  // *Show graphical percent complete
  if (lastIconProgress != progress) {
    char buf[5];
    GUI_POINT progressIcon;
    progressIcon = getIconStartPoint(1);
    const GUI_RECT percentageRect = {
        progressIcon.x,
        progressIcon.y,
        progressIcon.x + 95,
        progressIcon.y + 95};
    u16 progressX = map(progress, 0, 100, percentageRect.x0, percentageRect.x1);
    GUI_FillRectColor(percentageRect.x0, percentageRect.y0, progressX, percentageRect.y1, MAT_BLUE);
    GUI_FillRectColor(progressX, percentageRect.y0, percentageRect.x1, percentageRect.y1, MAT_DARKGRAY);
    // *Show numeric percent complete
    my_sprintf(buf, "%d%%", progress);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetColor(YELLOW);
    GUI_DispStringInPrect(&percentageRect, (u8*)buf);
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
    GUI_RestoreColorDefault();
    lastIconProgress = progress;
  }
}

extern SCROLL titleScroll;
extern GUI_RECT titleRect;

void printingDrawPage(void) {
  menuDrawPage(&printingItems);
  // fetchPreviewIcon(false);
}

void fetchPreviewIcon(bool showPreviewBMP) {
  if (showPreviewBMP) {
    int filenameLength;
    int locationLength;
    char filename[128] = "";
    char sample[128] = "";
    char preview[128] = "";
    // if model preview bmp exists, display bmp directly without writing to flash
    filenameLength = strlen((const char*)infoFile.file[0]);  // -6 if you want to remove ".gcode"
    locationLength = strlen((const char*)infoFile.title);
    strncat(filename, (const char*)infoFile.title, locationLength - 6);
    strncat(sample, (const char*)infoFile.title, locationLength - filenameLength);
    strcat(sample, "sample.bmp");
    strncat(preview, (const char*)infoFile.title, locationLength - filenameLength);
    strcat(preview, "preview.bmp");

    // *If sample.bmp exists, display it as a preview icon
    if (bmp_DirectDisplay(getIconStartPoint(1), strcat(filename, ".bmp")) == true) {
    } else if (bmp_DirectDisplay(getIconStartPoint(1), sample) == true) {
    } else if (bmp_DirectDisplay(getIconStartPoint(1), preview) == true) {
    } else {
      ITEM curItem = {ICON_BACKGROUND, LABEL_BACKGROUND};
      curItem.icon = ICON_SD_SOURCE;
      menuDrawItem(&curItem, 1);
    }
  } else {
    ITEM curItem = {ICON_BACKGROUND, LABEL_BACKGROUND};
    curItem.icon = ICON_SD_SOURCE;
    menuDrawItem(&curItem, 1);
  }
  // model preview -- end
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
      }
    } else if (infoPrinting.progress != 100) {
      infoPrinting.progress = 100;
    }
    // showPrintProgress(infoPrinting.progress);
    showIconPrintProgress(infoPrinting.progress);

    if (time != infoPrinting.time) {
      time = infoPrinting.time;
      showPrintTimeUpper();  // job timer
    }
    showBabyStepValue();
    showCNCSpeed();
    showRouterSpeed();

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
    runUpdateLoop();
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
  timedMessage(3, TIMED_WARNNG, "Job complete!");
  endPrinting();

  printingItems.items[KEY_ICON_3] = itemIsFinished[1];
  printingDrawPage();
  // Auto shut down after printing
  if (infoSettings.auto_off) {
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
        timedMessage(3, TIMED_ERROR, "Aborting CNC job");
        abortPrinting();
        infoMenu.active -= 2;
        break;

      case KEY_POPUP_CANCEL:
        infoMenu.active--;
        break;
    }
    runUpdateLoop();
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
        timedMessage(3, TIMED_CRITICAL, "Shutting down..");
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
    runUpdateLoop();
  }
}

// get gcode command from sd card
void getGcodeFromFile(void) {
  bool sd_comment_mode = false;
  bool sd_comment_space = true;
  char sd_char;
  u8 sdCharIndex = 0;
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
      if (sdCharIndex != 0) {
        gcodeCommand.queue[gcodeCommand.writeIndex].gcode[sdCharIndex++] = '\n';
        gcodeCommand.queue[gcodeCommand.writeIndex].gcode[sdCharIndex] = 0;  //terminate string
        gcodeCommand.queue[gcodeCommand.writeIndex].src = SERIAL_PORT;
        sdCharIndex = 0;  //clear buffer
        gcodeCommand.writeIndex = (gcodeCommand.writeIndex + 1) % GCODE_QUEUE_MAX;
        gcodeCommand.count++;
        break;
      }
    } else if (sdCharIndex >= GCODE_MAX_CHARACTERS - 2) {
    }  //when the command length beyond the maximum, ignore the following bytes
    else {
      if (sd_char == ';')  //';' is comment out flag
        sd_comment_mode = true;
      else {
        if (sd_comment_space && (sd_char == 'G' || sd_char == 'M' || sd_char == 'T'))  //ignore ' ' space bytes
          sd_comment_space = false;
        if (!sd_comment_mode && !sd_comment_space && sd_char != '\r')  //normal gcode
          gcodeCommand.queue[gcodeCommand.writeIndex].gcode[sdCharIndex++] = sd_char;
      }
    }
  }

  // *end of .gcode file - Finish up
  if ((infoPrinting.currentLine >= infoPrinting.size) && isPrinting()) {
    completePrinting();
  }
}

void checkJobStatus(void) {
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
