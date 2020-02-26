#ifndef _PRINTING_H_
#define _PRINTING_H_
#include "ff.h"
#include "stdbool.h"
#include "variants.h"

typedef struct
{
  FIL file;

  u32 time;        // Printed time in sec
  u32 size;        // Gcode file total size
  u32 cur;         // Current Gcode line
  u8 progress;     // Print progress (0-100)
  bool printing;   // true = printing, false = idle
  bool pause;      // true = paused
  bool m0_pause;   // true = M0/M1 gcode triggered pause
  u8 routerSpeed;  // Current router speed
} PRINTING;

void exitPrinting(void);
void endPrinting(void);
void completePrinting(void);
void abortPrinting(void);

void setM0Pause(bool m0_pause);
bool setPrintPause(bool is_pause, bool is_m0pause);

bool isPrinting(void);
bool isPause(void);
bool isM0_Pause(void);
void setPrintingTime(u32 RTtime);

void setPrintSize(u32 size);
void setPrintCur(u32 cur);

u8 getPrintProgress(void);
u32 getPrintTime(void);

void printSetUpdateWaiting(bool isWaiting);

void startGcodeExecute(void);
void endGcodeExecute(void);

void getGcodeFromFile(void);

void menuBeforePrinting(void);
void menuPrinting(void);
void menuStopPrinting(void);
void menuShutDown(void);

void printingDrawPage(void);
void showPrintProgress(u8 progress);
void showPrintTime(void);
void showRouterSpeed(void);

void loopCheckPrinting(void);

#endif
