#include "Gantry.h"
// #include "GUI.h"
#include "includes.h"
//1 title, ITEM_PER_PAGE items (icon + label)

static u32 nowTime = 0;
static u32 update_time = 200;  // 1 seconds is 100
SCROLL msgScroll;

static float xaxis;
static float yaxis;
static float zaxis;
static bool gantryCmdWait = false;

int current_router = 0;
int current_speedID = 0;
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
