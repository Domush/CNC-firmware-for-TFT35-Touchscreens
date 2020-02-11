#include "StatusScreen.h"
#include "GUI.h"
#include "includes.h"
//1 title, ITEM_PER_PAGE items (icon + label)
MENUITEMS StatusItems = {
    // title
    LABEL_READY,
    // icon                       label
    {
        {ICON_MAINMENU, LABEL_MAINMENU},
        {ICON_BACKGROUND, LABEL_BACKGROUND},  //Reserved for gantry position to be added later
        {ICON_BACKGROUND, LABEL_BACKGROUND},  //Reserved for gantry position to be added later
        {ICON_CNC, LABEL_CNC},
    }};

static u32 nowTime = 0;
static u32 update_time = 200;  // 1 seconds is 100
SCROLL msgScroll;
// static int lastConnection_status = -1;

// static char msgtitle[20];
// static char msgbody[512];

//static char msgxyz[512];
static float xaxis;
static float yaxis;
static float zaxis;
static bool gantryCmdWait = false;

TOOL current_Ext = SPINDLE0;
int current_router = 0;
int current_speedID = 0;
const char *SpeedID[2] = SPEED_ID;
// text position rectangles for Live icons
//icon 0
const GUI_POINT pointID[4] = {
    {1 * ICON_WIDTH + 0 * SPACE_X + START_X - BYTE_WIDTH / 2, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
    {2 * ICON_WIDTH + 1 * SPACE_X + START_X - BYTE_WIDTH / 2, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
    {3 * ICON_WIDTH + 2 * SPACE_X + START_X - BYTE_WIDTH / 2, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
    {4 * ICON_WIDTH + 3 * SPACE_X + START_X - BYTE_WIDTH / 2, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
};
const GUI_RECT rectB[4] = {
    {START_X + 0 * ICON_WIDTH + 0 * SPACE_X, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
     START_X + 1 * ICON_WIDTH + 0 * SPACE_X, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},

    {START_X + 1 * ICON_WIDTH + 1 * SPACE_X, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
     START_X + 2 * ICON_WIDTH + 1 * SPACE_X, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},

    {START_X + 2 * ICON_WIDTH + 2 * SPACE_X, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
     START_X + 3 * ICON_WIDTH + 2 * SPACE_X, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},

    {START_X + 3 * ICON_WIDTH + 3 * SPACE_X, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
     START_X + 4 * ICON_WIDTH + 3 * SPACE_X, ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},
};

//info rectangle
const GUI_RECT RectInfo = {START_X + 1 * ICON_WIDTH + 1 * SPACE_X, ICON_START_Y + 1 * ICON_HEIGHT + 1 * SPACE_Y,
                           START_X + 3 * ICON_WIDTH + 2 * SPACE_X, ICON_START_Y + 2 * ICON_HEIGHT + 1 * SPACE_Y};

const GUI_RECT msgRect = {START_X + 1 * ICON_WIDTH + 1 * SPACE_X + 2, ICON_START_Y + 1 * ICON_HEIGHT + 1 * SPACE_Y + STATUS_MSG_BODY_YOFFSET,
                          START_X + 3 * ICON_WIDTH + 2 * SPACE_X - 2, ICON_START_Y + 2 * ICON_HEIGHT + 1 * SPACE_Y - STATUS_MSG_BODY_BOTTOM};

const GUI_RECT RecGantry = {START_X, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + STATUS_GANTRY_YOFFSET,
                            4 * ICON_WIDTH + 3 * SPACE_X + START_X, 1 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y - STATUS_GANTRY_YOFFSET};

/*set status icons */
/* void set_status_icon(void)
{
  StatusItems.items[0] = ToolItems[0];
  StatusItems.items[1] = ToolItems[1];
  StatusItems.items[2] = ToolItems[2];
  StatusItems.items[3] = SpeedItems[0];

} */

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

void toggleTool(void) {
  if (OS_GetTime() > nowTime + update_time) {
    if (EXTRUDER_NUM > 1) {
      current_Ext = (TOOL)((current_Ext + 1) % HEATER_NUM);
      if (current_Ext == 0) {
        current_Ext += 1;
      }
    }
    if (ROUTER_NUM > 1) {
      current_router = (current_router + 1) % ROUTER_NUM;
    }
    current_speedID = (current_speedID + 1) % 2;
    nowTime = OS_GetTime();
    // drawTemperature();

    if (infoHost.connected == true) {
      if (gantryCmdWait != true) {
        gantryCmdWait = true;
        storeCmd("M114\n");
      }
    } else {
      gantryCmdWait = false;
    }
  }
}
