#include "Print.h"
#include "includes.h"

LISTITEMS printListItems = {
    // title
    LABEL_BACKGROUND,
    // icon                 ItemType      Item Title        item value text(only for custom value)
    {
        {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
        {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
        {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
        {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
        {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
        {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
        {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
        {ICONCHAR_BACK, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
    }};

MENUITEMS printIconItems = {
    //  title
    LABEL_BACKGROUND,
    // icon                       label
    {
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_PAGE_UP, LABEL_PAGE_UP},
        {ICON_PAGE_DOWN, LABEL_PAGE_DOWN},
        {ICON_BACK, LABEL_BACK},
    }};

// File list number per page
#define NUM_PER_PAGE 5

SCROLL titleScroll;
const GUI_RECT titleRect = {10, (TITLE_END_Y - BYTE_HEIGHT) / 2, LCD_WIDTH - 10, (TITLE_END_Y - BYTE_HEIGHT) / 2 + BYTE_HEIGHT};

SCROLL gcodeScroll;
bool icon_pre = false;

const GUI_RECT gcodeRect[NUM_PER_PAGE] = {
    {BYTE_WIDTH / 2 + 0 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2,
     1 * SPACE_X_PER_ICON - BYTE_WIDTH / 2, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2 + BYTE_HEIGHT},

    {BYTE_WIDTH / 2 + 1 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2,
     2 * SPACE_X_PER_ICON - BYTE_WIDTH / 2, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2 + BYTE_HEIGHT},

    {BYTE_WIDTH / 2 + 2 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2,
     3 * SPACE_X_PER_ICON - BYTE_WIDTH / 2, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2 + BYTE_HEIGHT},

    {BYTE_WIDTH / 2 + 3 * SPACE_X_PER_ICON, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2,
     4 * SPACE_X_PER_ICON - BYTE_WIDTH / 2, 1 * ICON_HEIGHT + 0 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2 + BYTE_HEIGHT},

    {BYTE_WIDTH / 2 + 0 * SPACE_X_PER_ICON, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2,
     1 * SPACE_X_PER_ICON - BYTE_WIDTH / 2, 2 * ICON_HEIGHT + 1 * SPACE_Y + ICON_START_Y + (SPACE_Y - BYTE_HEIGHT) / 2 + BYTE_HEIGHT},
};

void scrollFileNameCreate(u8 i) {
  u8 num = infoFile.cur_page * NUM_PER_PAGE + i;

  if (infoFile.F_num + infoFile.f_num == 0) {
    memset(&gcodeScroll, 0, sizeof(SCROLL));
    return;
  }
  if (num < infoFile.F_num) {
    Scroll_CreatePara(&gcodeScroll, (u8*)infoFile.folder[num], &gcodeRect[i]);
  } else if (num < infoFile.F_num + infoFile.f_num) {
    Scroll_CreatePara(&gcodeScroll, (u8*)infoFile.file[num - infoFile.F_num], &gcodeRect[i]);
  }
}

void normalNameDisp(const GUI_RECT* rect, u8* name) {
  if (name == NULL) return;

  GUI_ClearPrect(rect);
  GUI_SetRange(rect->x0, rect->y0, rect->x1, rect->y1);
  GUI_DispString(rect->x0, rect->y0 + (rect->y1 - rect->y0 - BYTE_HEIGHT) / 2, name);
  GUI_CancelRange();
}

bool get_Pre_Icon(void) {
  return icon_pre;
}

void gcodeIconDraw(void) {
  u8 i = 0;   //k = 0;
  int gn;
  char* gnew;
  ITEM curItem = {ICON_BACKGROUND, LABEL_BACKGROUND};

  scrollFileNameCreate(0);
  Scroll_CreatePara(&titleScroll, (uint8_t*)infoFile.title, &titleRect);
  printIconItems.title.address = (uint8_t*)infoFile.title;
  GUI_SetBkColor(TITLE_BACKGROUND_COLOR);
  GUI_ClearPrect(&titleRect);
  GUI_SetBkColor(BACKGROUND_COLOR);

  //draw folders
  for (i = 0; (i + infoFile.cur_page * NUM_PER_PAGE < infoFile.F_num) && (i < NUM_PER_PAGE); i++)   // folder
  {
    curItem.icon = ICON_FOLDER;
    menuDrawItem(&curItem, i);
    normalNameDisp(&gcodeRect[i], (u8*)infoFile.folder[i + infoFile.cur_page * NUM_PER_PAGE]);
  }

  //draw files
  for (; (i + infoFile.cur_page * NUM_PER_PAGE < infoFile.f_num + infoFile.F_num) && (i < NUM_PER_PAGE); i++)   // gcode file
  {
    curItem.icon = ICON_FILE;
    // if model preview bmp exists, display bmp directly without writing to flash
    gn   = strlen(infoFile.file[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num]) - 6;   // -6 means ".gcode"
    gnew = malloc(gn + 10);
    strcpy(gnew, getCurFileSource());
    strncat(gnew, infoFile.file[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num], gn);

    // if (bmp_DirectDisplay(getIconStartPoint(i), strcat(gnew, "_" STRINGIFY(ICON_WIDTH) ".bmp")) != true) {
    // *If sample.bmp exists, display it as a preview icon
    if (bmp_DirectDisplay(getIconStartPoint(i), strcat(gnew, ".bmp")) == true) {
      // Exact name bmp image will be displayed instead of generic icon
    } else if (bmp_DirectDisplay(getIconStartPoint(i), "sample.bmp") == true) {
      // Exact name bmp image will be displayed instead of generic icon
    } else {
      menuDrawItem(&curItem, i);
    }
    free(gnew);
    // model preview -- end
    normalNameDisp(&gcodeRect[i], (u8*)infoFile.file[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num]);
  }

  //clear blank icons
  for (; (i < NUM_PER_PAGE); i++) {
    curItem.icon = ICON_BACKGROUND;
    menuDrawItem(&curItem, i);
  }
}

void gcodeListDraw(void) {
  u8 i = 0;

  Scroll_CreatePara(&titleScroll, (u8*)infoFile.title, &titleRect);
  printListItems.title.address = (u8*)infoFile.title;
  GUI_SetBkColor(TITLE_BACKGROUND_COLOR);
  GUI_ClearRect(0, 0, LCD_WIDTH, TITLE_END_Y);
  GUI_SetBkColor(BACKGROUND_COLOR);

  for (i = 0; (i + infoFile.cur_page * NUM_PER_PAGE < infoFile.F_num) && (i < NUM_PER_PAGE); i++)   // folder
  {
    printListItems.items[i].icon = ICONCHAR_FOLDER;
    setDynamicLabel(i, infoFile.folder[i + infoFile.cur_page * NUM_PER_PAGE]);
    printListItems.items[i].titlelabel.index = LABEL_DYNAMIC;
    menuDrawListItem(&printListItems.items[i], i);
  }
  for (; (i + infoFile.cur_page * NUM_PER_PAGE < infoFile.f_num + infoFile.F_num) && (i < NUM_PER_PAGE); i++)   // gcode file
  {
    printListItems.items[i].icon = ICONCHAR_FILE;
    setDynamicLabel(i, (infoFile.source == BOARD_SD) ? infoFile.Longfile[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num] : infoFile.file[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num]);
    //dynamic_label[i] = (infoFile.source == BOARD_SD) ? infoFile.Longfile[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num] : infoFile.file[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num];
    printListItems.items[i].titlelabel.index = LABEL_DYNAMIC;
    menuDrawListItem(&printListItems.items[i], i);
  }

  for (; (i < NUM_PER_PAGE); i++)   //background
  {
    printListItems.items[i].icon             = ICONCHAR_BACKGROUND;
    printListItems.items[i].titlelabel.index = LABEL_BACKGROUND;
    menuDrawListItem(&printListItems.items[i], i);
  }
  // set page up down button according to page count and current page
  int t_pagenum = (infoFile.F_num + infoFile.f_num + (LISTITEM_PER_PAGE - 1)) / LISTITEM_PER_PAGE;
  if ((infoFile.F_num + infoFile.f_num) <= LISTITEM_PER_PAGE) {
    printListItems.items[5].icon = ICONCHAR_BACKGROUND;
    printListItems.items[6].icon = ICONCHAR_BACKGROUND;
  } else {
    if (infoFile.cur_page == 0) {
      printListItems.items[5].icon = ICONCHAR_BACKGROUND;
      printListItems.items[6].icon = ICONCHAR_PAGEDOWN;
    } else if (infoFile.cur_page == (t_pagenum - 1)) {
      printListItems.items[5].icon = ICONCHAR_PAGEUP;
      printListItems.items[6].icon = ICONCHAR_BACKGROUND;
    } else {
      printListItems.items[5].icon = ICONCHAR_PAGEUP;
      printListItems.items[6].icon = ICONCHAR_PAGEDOWN;
    }
  }
  menuDrawListItem(&printListItems.items[5], 5);
  menuDrawListItem(&printListItems.items[6], 6);
}

const int16_t labelVolumeError[] = {LABEL_READ_TFTSD_ERROR, LABEL_READ_U_DISK_ERROR, LABEL_READ_ONBOARDSD_ERROR};

void menuPrintFromSource(void) {
  KEY_VALUES key_num = KEY_IDLE;

  u8 update = 0;

  GUI_Clear(BACKGROUND_COLOR);
  GUI_DispStringInRect(0, 0, LCD_WIDTH, LCD_HEIGHT, textSelect(LABEL_LOADING));

  if (mountFS() == true && scanPrintFiles() == true) {
    if (infoSettings.file_listmode != true) {
      menuDrawPage(&printIconItems);
      gcodeIconDraw();
    } else {
      menuDrawListPage(&printListItems);
      gcodeListDraw();
    }
  } else {
    GUI_DispStringInRect(0, 0, LCD_WIDTH, LCD_HEIGHT, textSelect(labelVolumeError[infoFile.source]));
    Delay_ms(1000);
    infoMenu.active--;
  }

  while (infoMenu.menu[infoMenu.active] == menuPrintFromSource) {
    GUI_SetBkColor(TITLE_BACKGROUND_COLOR);
    Scroll_DispString(&titleScroll, LEFT);   //
    GUI_SetBkColor(BACKGROUND_COLOR);

    if (infoSettings.file_listmode != true) {
      Scroll_DispString(&gcodeScroll, CENTER);   //
    }

    key_num = menuKeyGetValue();

    switch (key_num) {
      case KEY_ICON_5:
        if (infoFile.cur_page > 0) {
          infoFile.cur_page--;
          update = 1;
        }
        break;

      case KEY_ICON_6:
        if (infoFile.cur_page + 1 < (infoFile.F_num + infoFile.f_num + (NUM_PER_PAGE - 1)) / NUM_PER_PAGE) {
          infoFile.cur_page++;
          update = 1;
        }
        break;

      case KEY_ICON_7:
        infoFile.cur_page = 0;
        if (IsRootDir() == true) {
          clearInfoFile();
          infoMenu.active--;
          break;
        } else {
          ExitDir();
          scanPrintFiles();
          update = 1;
        }
        break;

      case KEY_IDLE:
        break;

      default:
        if (key_num <= KEY_ICON_4) {
          u16 start = infoFile.cur_page * NUM_PER_PAGE;
          if (key_num + start < infoFile.F_num)   //folder
          {
            if (EnterDir(infoFile.folder[key_num + start]) == false) break;
            scanPrintFiles();
            update            = 1;
            infoFile.cur_page = 0;
          } else if (key_num + start < infoFile.F_num + infoFile.f_num)   //gcode
          {
            if (infoHost.connected != true) break;
            if (EnterDir(infoFile.file[key_num + start - infoFile.F_num]) == false) break;

            //load bmp preview in flash if file exists
            int gn;
            char* gnew;
            gn   = strlen(infoFile.file[key_num + start - infoFile.F_num]) - 6;   // -6 means ".gcode"
            gnew = malloc(gn + 10);
            strcpy(gnew, getCurFileSource());
            strncat(gnew, infoFile.file[key_num + start - infoFile.F_num], gn);

            // *If <gcodeFilename>.bmp exists, display it as a preview icon
            if (bmpDecode(strcat(gnew, ".bmp"), ICON_ADDR(ICON_PREVIEW)) == true) {
              icon_pre = true;
              // *ElseIf sample.bmp exists, display it as a preview icon
            } else if (bmpDecode("sample.bmp", ICON_ADDR(ICON_PREVIEW)) == true) {
              icon_pre = true;
              // *Else display nothing
            } else {
              icon_pre = false;
            }
            free(gnew);
            //-load bmp preview in flash if file exists - end
            infoMenu.menu[++infoMenu.active] = menuBeforePrinting;
          }
        }

        else if (key_num >= KEY_LABEL_0 && key_num <= KEY_LABEL_4) {
          if (infoSettings.file_listmode != true) {
            if (key_num - KEY_LABEL_0 + infoFile.cur_page * NUM_PER_PAGE < infoFile.F_num + infoFile.f_num) {
              normalNameDisp(gcodeScroll.dimensions, gcodeScroll.text);
              scrollFileNameCreate(key_num - KEY_LABEL_0);
            }
          }
        }
        break;
    }

    if (update) {
      update = 0;

      if (infoSettings.file_listmode != true) {
        gcodeIconDraw();
      } else {
        gcodeListDraw();
      }
    }

#ifdef SD_CD_PIN
    if (isVolumeExist(infoFile.source) != true) {
      resetInfoFile();
      infoMenu.active--;
    }
#endif
    runUpdateLoop();
  }
}

MENUITEMS sourceSelItems = {
    //  title
    LABEL_CNC,
    // icon                       label
    {
        {ICON_SD_SOURCE, LABEL_TFTSD},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
#ifdef ONBOARD_SD_SUPPORT
        {ICON_BSD_SOURCE, LABEL_ONBOARDSD},
#endif
#ifdef U_DISK_SUPPORT
        {ICON_U_DISK, LABEL_U_DISK},
#else
        {ICON_BACKGROUND, LABEL_BACKGROUND},
#endif
#ifndef ONBOARD_SD_SUPPORT
        {ICON_BACKGROUND, LABEL_BACKGROUND},
#endif
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACKGROUND, LABEL_BACKGROUND},
        {ICON_BACK, LABEL_BACK},
    }};

void menuPrint(void) {
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&sourceSelItems);
  while (infoMenu.menu[infoMenu.active] == menuPrint) {
    key_num = menuKeyGetValue();
    switch (key_num) {
      case KEY_ICON_0:
        infoFile.source                  = TFT_SD;
        infoMenu.menu[++infoMenu.active] = menuPrintFromSource;
        // infoMenu.menu[++infoMenu.active] = menuPowerOff; // *Disabling resume after power loss (for now)
        goto selectEnd;

#ifdef ONBOARD_SD_SUPPORT
      case KEY_ICON_3:
        infoFile.source                  = BOARD_SD;
        infoMenu.menu[++infoMenu.active] = menuPrintFromSource;   //TODO: fix here,  onboard sd card PLR feature
        goto selectEnd;
#endif

#ifdef U_DISK_SUPPORT
  #ifdef ONBOARD_SD_SUPPORT
      case KEY_ICON_4:
  #else
      case KEY_ICON_3:
  #endif
        infoFile.source                  = TFT_UDISK;
        infoMenu.menu[++infoMenu.active] = menuPrintFromSource;
        infoMenu.menu[++infoMenu.active] = menuPowerOff;
        goto selectEnd;
#endif

      case KEY_ICON_7:
        infoMenu.active--;
        return;

      default:
        break;
    }
    runUpdateLoop();
  }

selectEnd:
  resetInfoFile();
  powerFailedSetDriverSource(getCurFileSource());
}
