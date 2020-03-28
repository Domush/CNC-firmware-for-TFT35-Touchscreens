#ifndef _PRINTING_H_
#define _PRINTING_H_
#include "includes.h"
// #include "ff.h"
// #include "stdbool.h"
// #include "System/variants.h"

typedef struct
{
  FIL file;

  u32 time;          // Printed time in sec
  u32 size;          // Gcode file total size
  u32 currentLine;   // Current Gcode line
  u8 coordSpace;     // CNC coodinate space (53-59) [53 is the default machine coords]
  u8 progress;       // Print progress (0-100)
  bool printing;     // true = printing, false = idle
  bool pause;        // true = paused
  bool m0_pause;     // true = M0/M1 gcode triggered pause
  u8 routerSpeed;    // Current router speed
  float babyStep;    // Current Z-height adjustment
} PRINTING;

PRINTING infoPrinting;

void exitPrinting(void);
void endPrinting(void);
void completePrinting(void);
void abortPrinting(void);

bool setPrintPause(bool pauseCalled);

bool isPrinting(void);
bool isPause(void);
bool isM0_Pause(void);
void setPrintingTime(u32 RTtime);

void setPrintSize(u32 size);
void setPrintCur(u32 currentLine);

u8 getPrintProgress(void);
u32 getPrintTime(void);

void printSetUpdateWaiting(bool isWaiting);

void startGcodeExecute(void);
void endGcodeExecute(void);

void getGcodeFromFile(void);
u8* getGcodeFilename(char* path);

void menuBeforePrinting(void);
void menuPrinting(void);
void menuStopPrinting(void);
void menuShutDown(void);

void printingDrawPage(void);
void fetchPreviewIcon(bool showPreviewBMP);
void showPrintProgress(u8 progress);
void showPrintTime(void);
void showPrintTimeUpper(void);
void showBabyStepValue(void);
void showRouterSpeed(void);

void checkJobStatus(void);

#endif
