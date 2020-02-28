#include "Move.h"
#include "includes.h"

//const GUI_RECT RecXYZ = {START_X + 1*ICON_WIDTH,        STATUS_GANTRY_YOFFSET,
//                         4*ICON_WIDTH+3*SPACE_X+START_X,ICON_START_Y-STATUS_GANTRY_YOFFSET};

#define YGCODE_INC "G1 Y%.1f\n"
#define YGCODE_DEC "G1 Y-%.1f\n"
#define ZGCODE_INC "G1 Z%.1f\n"
#define ZGCODE_DEC "G1 Z-%.1f\n"

// AXIS a = X_AXIS;

//1 title, ITEM_PER_PAGE item
MENUITEMS moveItems = {
    //  title
    LABEL_MOVE,
    // icon                       label
    {
        {ICON_Z_DEC, LABEL_Z_DEC},
        {ICON_Y_INC, LABEL_Y_INC},
        {ICON_Z_INC, LABEL_Z_INC},
        {ICON_100_MM, LABEL_100_MM},
        {ICON_X_DEC, LABEL_X_DEC},
        {ICON_Y_DEC, LABEL_Y_DEC},
        {ICON_X_INC, LABEL_X_INC},
        {ICON_BACK, LABEL_BACK},
    }};

#define ITEM_MOVE_DISTANCE 3
const ITEM itemMoveDistance[ITEM_MOVE_DISTANCE] = {
    // icon                       label
    {ICON_10_MM, LABEL_10_MM},
    {ICON_1_MM, LABEL_1_MM},
    {ICON_100_MM, LABEL_100_MM},
};

const float moveDistance[ITEM_MOVE_DISTANCE] = {10, 1, 100};
static u8 moveDistance_index = 2;

static u32 nowTime = 0;
static u32 update_time = 50;  // 1 seconds is 100

void menuMove(void) {
  KEY_VALUES key_num = KEY_IDLE;
  char *y_axis_up;
  char *y_axis_down;
  char *z_axis_up;
  char *z_axis_down;

  if (infoSettings.invert_yaxis == 1) {
    moveItems.items[1].label.index = LABEL_Y_DEC;
    moveItems.items[5].label.index = LABEL_Y_INC;
    y_axis_up = YGCODE_DEC;
    y_axis_down = YGCODE_INC;
  } else {
    moveItems.items[1].label.index = LABEL_Y_INC;
    moveItems.items[5].label.index = LABEL_Y_DEC;
    y_axis_up = YGCODE_INC;
    y_axis_down = YGCODE_DEC;
  }

  if (infoSettings.invert_zaxis == 1) {
    moveItems.items[0].label.index = LABEL_Z_INC;
    moveItems.items[2].label.index = LABEL_Z_DEC;
    z_axis_up = ZGCODE_INC;
    z_axis_down = ZGCODE_DEC;
  } else {
    moveItems.items[0].label.index = LABEL_Z_DEC;
    moveItems.items[2].label.index = LABEL_Z_INC;
    z_axis_up = ZGCODE_DEC;
    z_axis_down = ZGCODE_INC;
  }

  menuDrawPage(&moveItems);
  mustStoreCmd("G91\n");

  switch (infoSettings.move_speed) {
    case 1:
      mustStoreCmd("G1 F%d\n", SPEED_MOVE_SLOW);
      break;
    case 2:
      mustStoreCmd("G1 F%d\n", SPEED_MOVE_FAST);
      break;
    default:
      mustStoreCmd("G1 F%d\n", DEFAULT_SPEED_MOVE);
      break;
  }
  mustStoreCmd("M114\n");
  drawXYZ();

  while (infoMenu.menu[infoMenu.active] == menuMove) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        storeCmd(z_axis_up, moveDistance[moveDistance_index]);
        break;

      case KEY_ICON_1:
        storeCmd(y_axis_up, moveDistance[moveDistance_index]);
        break;

      case KEY_ICON_2:
        storeCmd(z_axis_down, moveDistance[moveDistance_index]);
        break;

      case KEY_ICON_3:
        moveDistance_index = (moveDistance_index + 1) % ITEM_MOVE_DISTANCE;
        moveItems.items[key_num] = itemMoveDistance[moveDistance_index];
        menuDrawItem(&moveItems.items[key_num], key_num);
        break;

      case KEY_ICON_4:
        storeCmd("G1 X-%.1f\n", moveDistance[moveDistance_index]);
        break;

      case KEY_ICON_5:
        storeCmd(y_axis_down, moveDistance[moveDistance_index]);
        break;

      case KEY_ICON_6:
        storeCmd("G1 X%.1f\n", moveDistance[moveDistance_index]);
        break;
      case KEY_ICON_7:
        infoMenu.active--;
        break;
      default:
        break;
    }
    loopProcess();
  }
  mustStoreCmd("G90\n");
}

void update_gantry(void) {
  if (OS_GetTime() > nowTime + update_time) {
    if (infoHost.connected == true && infoHost.waiting == false) {
      storeCmd("M114\n");
    }
    drawXYZ();
    nowTime = OS_GetTime();
  }
}
void drawXYZ(void) {
  if (infoHost.connected) {
    char tempstr[10];
    GUI_SetColor(YELLOW);
    //GUI_FillPrect(&RecXYZ);
    my_sprintf(tempstr, "X:%.1f", getAxisLocation(0));
    GUI_DispLenString((LCD_WIDTH / 3) * 0 + 3 * BYTE_WIDTH, BYTE_HEIGHT * 2, (u8 *)tempstr, LCD_WIDTH / 3);
    my_sprintf(tempstr, "Y:%.1f", getAxisLocation(1));
    GUI_DispLenString((LCD_WIDTH / 3) * 1 + 3 * BYTE_WIDTH, BYTE_HEIGHT * 2, (u8 *)tempstr, LCD_WIDTH / 3);
    my_sprintf(tempstr, "Z:%.1f", getAxisLocation(2));
    GUI_DispLenString((LCD_WIDTH / 3) * 2 + 3 * BYTE_WIDTH, BYTE_HEIGHT * 2, (u8 *)tempstr, LCD_WIDTH / 3);

    //GUI_SetBkColor(BACKGROUND_COLOR);
    GUI_SetColor(FONT_COLOR);
  }
}
