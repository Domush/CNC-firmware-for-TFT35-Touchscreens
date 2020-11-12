#include "gantry.h"
#include "includes.h"

// LCD init functions
#include "lcd.h"
#include "GUI.h"

// Chip specific includes
#include "Serial.h"

// File handling
#include "list_item.h"

// Gcode processing
#include "Gcode/gcodeSender.h"

// Base API functions
#include "API/coordinate.h"

// Menus
#include "includesMenus.h"  // All menu headers

SCROLL msgScroll;

static float xaxis;
static float yaxis;
static float zaxis;
static bool gantryCmdWait = false;

int current_router     = 0;
int current_speedID    = 0;
const char *SpeedID[2] = SPEED_ID;
// text position rectangles for Live icons
//icon 0

void storegantry(int n, float val) {
  //float* px = &val;
  switch (n) {
    case 0:
      xaxis = val;
      break;
    case 1:
      yaxis = val;
      break;
    case 2:
      zaxis = val;
      break;
    default:
      break;
  }
  gantryCmdWait = false;
}

void gantry_inc(int n, float val) {
  //float* px = &val;
  switch (n) {
    case 0:
      xaxis += val;
      if (xaxis > X_MAX_POS) {
        xaxis = X_MAX_POS;
      }
      break;
    case 1:
      yaxis += val;
      if (yaxis > Y_MAX_POS) {
        yaxis = Y_MAX_POS;
      }
      break;
    case 2:
      zaxis += val;
      if (zaxis > Z_MAX_POS) {
        zaxis = Z_MAX_POS;
      }
      break;
    default:
      break;
  }
}
void gantry_dec(int n, float val) {
  //float* px = &val;
  switch (n) {
    case 0:
      xaxis -= val;
      if (xaxis < X_MIN_POS) {
        xaxis = X_MIN_POS;
      }
      break;
    case 1:
      yaxis -= val;
      if (yaxis < Y_MIN_POS) {
        yaxis = Y_MIN_POS;
      }
      break;
    case 2:
      zaxis -= val;
      if (zaxis < Z_MIN_POS) {
        zaxis = Z_MIN_POS;
      }
      break;
    default:
      break;
  }
}

float getAxisLocation(u8 n) {
  switch (n) {
    case 0:
      return xaxis;
    case 1:
      return yaxis;
    case 2:
      return zaxis;
    default:
      return xaxis;
  }
}

void drawXYZ(void) {
  if (infoHost.connected) {
    COORDINATE curGantryCoords;
    coordinateGetAll(&curGantryCoords);
    // *X location [X_MIN_POS - X_MAX_POS] (set in Configuration.c)
    GUI_SetColor(MAT_RED);
    GUI_DispString((LCD_WIDTH / 3) * 0 + 6 * BYTE_WIDTH, BYTE_HEIGHT * 2, (u8 *)"X:");
    GUI_SetColor(MAT_YELLOW);
    GUI_DispFloat((LCD_WIDTH / 3) * 0 + 8 * BYTE_WIDTH, BYTE_HEIGHT * 2, curGantryCoords.axis[X_AXIS], 4, 1, RIGHT);
    // *Y location [Y_MIN_POS - Y_MAX_POS] (set in Configuration.c)
    GUI_SetColor(MAT_BLUE);
    GUI_DispString((LCD_WIDTH / 3) * 1 + 3 * BYTE_WIDTH, BYTE_HEIGHT * 2, (u8 *)"Y:");
    GUI_SetColor(MAT_YELLOW);
    GUI_DispFloat((LCD_WIDTH / 3) * 1 + 5 * BYTE_WIDTH, BYTE_HEIGHT * 2, curGantryCoords.axis[Y_AXIS], 4, 1, RIGHT);
    // *Z location [Z_MIN_POS - Z_MAX_POS] (set in Configuration.c)
    GUI_SetColor(MAT_GREEN);
    GUI_DispString((LCD_WIDTH / 3) * 2 + 0 * BYTE_WIDTH, BYTE_HEIGHT * 2, (u8 *)"Z:");
    GUI_SetColor(MAT_YELLOW);
    GUI_DispFloat((LCD_WIDTH / 3) * 2 + 2 * BYTE_WIDTH, BYTE_HEIGHT * 2, curGantryCoords.axis[Z_AXIS], 3, 1, RIGHT);
    // *CNC Coordinate space [53-59] (Defaults to G53 - Machine space)
    if (infoJobStatus.coordSpace < 53) infoJobStatus.coordSpace = 53;
    GUI_SetColor(WHITE);
    GUI_DispString(LCD_WIDTH - 4 * BYTE_WIDTH, BYTE_HEIGHT * 2, (u8 *)"C:");
    GUI_SetColor(MAT_YELLOW);
    GUI_DispDec(LCD_WIDTH - 2 * BYTE_WIDTH, BYTE_HEIGHT * 2, infoJobStatus.coordSpace - 52, 1, RIGHT);

    GUI_RestoreColorDefault();
  }
}
