#include "extendedFeatures.h"
#include "includes.h"

// LCD init functions
#include "lcd.h"
#include "GUI.h"

// Chip specific includes
#include "Serial.h"
#include "usart.h"
#include "GPIO_Init.h"

// File handling
#include "list_item.h"

// Timing functions
#include "System/os_timer.h"

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

static bool updateJobStatus = true;

/* Set whether we need to query the current position */
void positionSetUpdateWaiting(bool needUpdate) {
  updateJobStatus = needUpdate;
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
