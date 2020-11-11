#include "includes.h"

// LCD init functions
#include "Hal/LCD_Init.h"

// Chip specific includes
#include "Serial.h"
#include "spi_slave.h"

// SD card support
#include "Hal/sd.h"
#include "Hal/w25qxx.h"
#include "Hal/xpt2046.h"

// UI handling
#include "ST7920_Simulator.h"
#include "touch_process.h"

// File handling
#include "list_item.h"
#include "System/flashStorage.h"

// Gcode processing
#include "Gcode/gcodeSender.h"
#include "Gcode/gcodeResponseHandler.h"

// Base API functions
#include "API/extendedFeatures.h"

// Timing functions
#include "System/os_timer.h"
#include "System/boot.h"

void Hardware_GenericInit(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_init(F_CPUM);
  OS_TimerInit(9999, F_CPUM - 1);  // System clock timer, cycle 10ms

#ifdef DISABLE_DEBUG
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  //disable JTAG & SWD
#endif

#ifdef DISABLE_JTAG
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
#endif

  XPT2046_Init();
  W25Qxx_Init();
  LCD_Init();              // Initialize the LCD
  readStoredPara();        // Fetch settings from EEPROM
  LCD_RefreshDirection();  // Update display orientation using saved settings
  scanUpdates();
  SD_DeInit();

#if LCD_ENCODER_SUPPORT
  LCD_EncoderInit();
#endif

#ifdef PS_ON_PIN
  PS_ON_Init();
#endif

  if (readStoredPara() == false) {
    TSC_Calibration();
    storePara();
  }
  GUI_RestoreColorDefault();
  infoMenuSelect();
}

int main(void) {
  // Zero out serial queues
  gcodeResponse->count      = 0;
  gcodeResponse->queueIndex = 0;
  gcodeOutgoing.count       = 0;
  gcodeOutgoing.queueIndex  = 0;
  SCB->VTOR                 = VECT_TAB_FLASH;

  // Initialize the hardware
  Hardware_GenericInit();

  // Main loop
  do {
    (*infoMenu.menu[infoMenu.active])();
  } while (1);
}
