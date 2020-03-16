#include "PowerFailed.h"
#include "includes.h"

BREAK_POINT infoBreakPoint;
char powerFailedFileName[256];

static bool powerFailedSave = false;
static bool create_ok       = false;

void powerFailedSetDriverSource(char* src) {
  strcpy(powerFailedFileName, src);
  strcat(powerFailedFileName, BREAK_POINT_FILE);
}

void powerFailedEnable(bool enable) {
  powerFailedSave = enable;
}

void clearPowerFailed(void) {
  memset(&infoBreakPoint, 0, sizeof(BREAK_POINT));
}

FIL fpPowerFailed;
bool powerFailedCreate(char* path) {
  UINT br;

  create_ok = false;

  if (infoFile.source != TFT_SD) return false;   //support SD Card only now

  if (f_open(&fpPowerFailed, powerFailedFileName, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) return false;

  f_write(&fpPowerFailed, path, MAX_PATH_LEN, &br);
  f_write(&fpPowerFailed, &infoBreakPoint, sizeof(BREAK_POINT), &br);
  f_sync(&fpPowerFailed);

  create_ok = true;
  return true;
}

void powerFailedCache(u32 offset) {
  UINT br;
  static u32 nowTime = 0;

  if (OS_GetTime() < nowTime + 100) return;
  if (create_ok == false) return;
  if (powerFailedSave == false) return;
  if (isPause() == true) return;

  // if (gcodeCommandQueue.count != 0) return;

  powerFailedSave = false;

  nowTime = OS_GetTime();

  infoBreakPoint.offset = offset;
  for (AXIS i = X_AXIS; i < TOTAL_AXIS; i++) {
    infoBreakPoint.axis[i] = coordinateGetAxisTarget(i);
  }
  infoBreakPoint.gantryspeed = coordinateGetGantrySpeed();
  infoBreakPoint.speed       = getCNCSpeedOverride();

  infoBreakPoint.routerSpeed = ROUTER_MAX_PWM;
  infoBreakPoint.relative    = coorGetRelative();

  f_lseek(&fpPowerFailed, MAX_PATH_LEN);
  f_write(&fpPowerFailed, &infoBreakPoint, sizeof(BREAK_POINT), &br);
  f_sync(&fpPowerFailed);
}

void powerFailedClose(void) {
  if (create_ok == false) return;

  f_close(&fpPowerFailed);
}

void powerFailedDelete(void) {
  if (create_ok == false) return;

  f_unlink(powerFailedFileName);
  clearPowerFailed();
}

static bool powerFailedExist(void) {
  FIL fp;
  UINT br;
  if (f_open(&fp, powerFailedFileName, FA_OPEN_EXISTING | FA_READ) != FR_OK) return false;
  if (f_read(&fp, infoFile.title, MAX_PATH_LEN, &br) != FR_OK) return false;
  if (f_close(&fp) != FR_OK) return false;

  create_ok = true;
  return true;
}

bool powerFailedlSeek(FIL* fp) {
  if (f_lseek(fp, infoBreakPoint.offset) != FR_OK) return false;

  return true;
}

bool powerOffGetData(void) {
  FIL fp;
  UINT br;

  if (f_open(&fp, powerFailedFileName, FA_OPEN_EXISTING | FA_READ) != FR_OK) return false;
  if (f_lseek(&fp, MAX_PATH_LEN) != FR_OK) return false;
  if (f_read(&fp, &infoBreakPoint, sizeof(infoBreakPoint), &br) != FR_OK) return false;

  if (infoBreakPoint.routerSpeed != 0) {
    routerControl(infoBreakPoint.routerSpeed);
  }

  if (infoBreakPoint.gantryspeed != 0) {
    storeCmd("G92 Z%.3f\n", infoBreakPoint.axis[Z_AXIS]
#ifdef BTT_MINI_UPS
                                + POWER_LOSS_ZRAISE
#endif
    );
    storeCmd("G1 Z%.3f\n", infoBreakPoint.axis[Z_AXIS] + POWER_LOSS_ZRAISE);
#ifdef HOME_BEFORE_PLR
    storeCmd("G28\n");
    storeCmd("G1 Z%.3f\n", infoBreakPoint.axis[Z_AXIS] + POWER_LOSS_ZRAISE);
#else
    storeCmd("G28 R0 XY\n");
#endif
    storeCmd("G1 X%.3f Y%.3f Z%.3f F3000\n",
             infoBreakPoint.axis[X_AXIS],
             infoBreakPoint.axis[Y_AXIS],
             infoBreakPoint.axis[Z_AXIS]);
    storeCmd("G1 F%d\n", infoBreakPoint.gantryspeed);

    if (infoBreakPoint.relative == true) {
      storeCmd("G91\n");
    }
  }

  f_close(&fp);
  return true;
}

void menuPowerOff(void) {
  u16 key_num = IDLE_TOUCH;
  clearPowerFailed();
  GUI_Clear(BACKGROUND_COLOR);
  GUI_DispString((LCD_WIDTH - GUI_StrPixelWidth(textSelect(LABEL_LOADING))) / 2, LCD_HEIGHT / 2 - BYTE_HEIGHT, textSelect(LABEL_LOADING));

  if (mountFS() == true && powerFailedExist()) {
    popupDrawPage(bottomDoubleBtn, textSelect(LABEL_POWER_FAILED), (u8*)infoFile.title, textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

    while (infoMenu.menu[infoMenu.active] == menuPowerOff) {
      key_num = KEY_GetValue(2, doubleBtnRect);
      switch (key_num) {
        case KEY_POPUP_CONFIRM:
          powerOffGetData();
          infoMenu.menu[1] = menuPrintFromSource;
          infoMenu.menu[2] = menuBeforePrinting;
          infoMenu.active  = 2;
          break;

        case KEY_POPUP_CANCEL:
          powerFailedDelete();
          ExitDir();
          infoMenu.active--;
          break;
      }

#ifdef SD_CD_PIN
      if (isVolumeExist(infoFile.source) != true) {
        resetInfoFile();
        clearPowerFailed();
        infoMenu.active--;
      }
#endif
      runUpdateLoop();
    }
  } else {
    infoMenu.active--;
  }
}
