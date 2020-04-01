#ifndef _PRINTING_H_
#define _PRINTING_H_
#include "includes.h"
// #include "ff.h"
// #include "stdbool.h"
// #include "System/variants.h"

typedef struct
{
  FIL file;             // File details
  u32 timeElapsed;      // Job time in sec
  u32 size;             // Gcode file total size
  u32 currentLine;      // Current Gcode line
  u8 coordSpace;        // CNC coodinate space (53-59) [53 is the default machine coords]
  u8 percentComplete;   // Print progress (0-100)
  bool inProgress;      // true = job active, false = idle
  bool isPaused;        // true = paused
  bool isM0Paused;      // true = M0/M1 gcode triggered pause
  u8 routerSpeed;       // Current router speed
  float babyStep;       // Current Z-height adjustment
} JOBSTATUS;

JOBSTATUS infoJobStatus;

void jobExit(void);
void jobEnd(void);
void jobComplete(void);
void jobAbort(void);

bool setPrintPause(bool pauseCalled);

bool jobInProgress(void);
bool jobIsPaused(void);
bool jobIsM0Paused(void);
void jobSetTimeElapsed(u32 RTtime);

void jobSetSize(u32 size);
void jobSetCurrentLine(u32 currentLine);

u8 jobGetPercentComplete(void);
u32 jobGetTimeElapsed(void);

void jobNeedUpdate(bool isWaiting);

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
