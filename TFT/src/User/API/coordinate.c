#include "coordinate.h"
#include "string.h"
#include "Configuration.h"

const char axis_id[TOTAL_AXIS] = {'X', 'Y', 'Z'};

COORDINATE targetPosition = {{0.0f, 0.0f, 0.0f}, 3000};

//
static bool relative_mode = false;
static bool relative_e = false;
// false means current position is not clear
// false after M18/M84 disable stepper or power up, true after G28
static bool position_cleared = false;

bool coorGetRelative(void) {
  return relative_mode;
}

void coorSetRelative(bool mode) {
  relative_mode = mode;
}

bool eGetRelative(void) {
  return relative_e;
}

void eSetRelative(bool mode) {
  relative_e = mode;
}

bool coordinateIsClear(void) {
  return position_cleared;
}

void coordinateSetClear(bool clear) {
  position_cleared = clear;
}

void coordinateSetAxisTarget(AXIS axis, float position) {
  bool r = (axis == E_AXIS)
               ? relative_e || relative_mode
               : relative_mode;

  if (r == false) {
    targetPosition.axis[axis] = position;
  } else {
    targetPosition.axis[axis] += position;
  }
}

void coordinateSetGantrySpeed(u32 gantryspeed) {
  targetPosition.gantryspeed = gantryspeed;
}

float coordinateGetAxisTarget(AXIS axis) {
  return targetPosition.axis[axis];
}

u32 coordinateGetGantrySpeed(void) {
  return targetPosition.gantryspeed;
}

void coordinateGetAll(COORDINATE *tmp) {
  memcpy(tmp, &targetPosition, sizeof(targetPosition));
}

COORDINATE curPosition = {{0.0f, 0.0f, 0.0f}, 3000};

void coordinateSetAxisActualSteps(AXIS axis, int steps) {
  curPosition.axis[axis] = steps / 80;  // TODO: NEED fix, M92 auto get
}

float coordinateGetAxisActual(AXIS axis) {
  return curPosition.axis[axis];
}
