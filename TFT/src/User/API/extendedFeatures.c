// #include "extend.h"
#include "GPIO_Init.h"
// #include "System/variants.h"
#include "includes.h"

// Power Supply
#ifdef PS_ON_PIN
// Power Supply Control pins Initialization
void PS_ON_Init(void) {
  GPIO_InitSet(PS_ON_PIN, MGPIO_MODE_OUT_PP, 0);
  GPIO_SetLevel(PS_ON_PIN, PS_ON_ACTIVE_HIGH);
}

// Power Supply Control turn on, M80
void PS_ON_On(void) {
  GPIO_SetLevel(PS_ON_PIN, PS_ON_ACTIVE_HIGH);
}

// Power Supply Control turn off, M81
void PS_ON_Off(void) {
  GPIO_SetLevel(PS_ON_PIN, !PS_ON_ACTIVE_HIGH);
}
#endif

// Filament runout detect
#ifdef FIL_RUNOUT_PIN

static bool update_waiting = false;
/* Set whether we need to query the current position */
void positionSetUpdateWaiting(bool isWaiting) {
  update_waiting = isWaiting;
}

void FIL_Runout_Init(void) {
  GPIO_InitSet(FIL_RUNOUT_PIN, FIL_RUNOUT_INVERTING ? MGPIO_MODE_IPD : MGPIO_MODE_IPU, 0);
}

bool FIL_RunoutPinFilteredLevel(void) {
  static bool rst       = false;
  static u32 nowTime    = 0;
  static u32 trueTimes  = 0;
  static u32 falseTimes = 0;

  if (OS_GetTime() > nowTime + FIL_NOISE_THRESHOLD) {
    rst        = trueTimes > falseTimes ? true : false;
    nowTime    = OS_GetTime();
    trueTimes  = 0;
    falseTimes = 0;
  } else {
    if (GPIO_GetLevel(FIL_RUNOUT_PIN)) {
      trueTimes++;
    } else {
      falseTimes++;
    }
  }
  return rst;
}

// static u32 update_time = 200;
// Use an encoder disc to toggles the runout
// Suitable for BigTreeTech Smart filament detecter
bool FIL_SmartRunoutDetect(void) {
  static uint8_t lastRunoutPinLevel = 0;
  static uint8_t isAlive            = false;

  bool pinLevel = FIL_RunoutPinFilteredLevel();

  if (isAlive == false) {
    if (lastRunoutPinLevel != pinLevel) {
      isAlive = true;
    }
  }
  return false;
}

bool FIL_IsRunout(void) {
  switch (infoSettings.runout) {
    case FILAMENT_RUNOUT_ON:
      // Detect HIGH/LOW level, Suitable for general mechanical / photoelectric switches
      return (FIL_RunoutPinFilteredLevel() == FIL_RUNOUT_INVERTING);

    case FILAMENT_SMART_RUNOUT_ON:
      return FIL_SmartRunoutDetect();

    default:
      return false;
  }
}

void loopFILRunoutDetect(void) {
  if (infoSettings.runout == FILAMENT_RUNOUT_OFF) return;   // Filament runout turn off
  if (!FIL_IsRunout()) return;                              // Filament not runout yet, need constant scanning to filter interference
  if (!isPrinting() || isPause()) return;                   // No printing or printing paused

  if (setPrintPause(true)) {
    popupReminder(textSelect(LABEL_WARNING), textSelect(LABEL_FILAMENT_RUNOUT));
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
  u16 delayTime    = OS_GetTime() + 500;
  char buffer[512] = "";
  my_sprintf(buffer, "CRITICAL ERROR!\n\n\nProblem in file:\n%s\n\nOn line: %d", file, line);
  GUI_Clear(BLACK);
  GUI_SetColor(MAT_RED);
  GUI_DispStringInRectEOL(50, BYTE_HEIGHT * 1, LCD_WIDTH - 50, LCD_HEIGHT - BYTE_HEIGHT * 2, (u8*)buffer);
  GUI_SetColor(MAT_LOWWHITE);
  while (OS_GetTime() < delayTime) {
    my_sprintf(buffer, "Continuing in: %d", (delayTime - OS_GetTime()) / 100);
    GUI_DispStringInRect(50, LCD_HEIGHT - BYTE_HEIGHT, LCD_WIDTH - 50, LCD_HEIGHT, (u8*)buffer);
  }
}
#endif
