#include "SendGcode.h"
#include "includes.h"

typedef enum {
  SOFT_KEY_123 = 0,
  SOFT_KEY_ABC,
} SOFT_KEY_TYPE;

// 26 buttons
const GUI_RECT rect_of_Gkey[] = {
    {0 * GKEY_WIDTH, 0 * GKEY_HEIGHT + ICON_START_Y, 1 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y},
    {1 * GKEY_WIDTH, 0 * GKEY_HEIGHT + ICON_START_Y, 2 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y},
    {2 * GKEY_WIDTH, 0 * GKEY_HEIGHT + ICON_START_Y, 3 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y},
    {3 * GKEY_WIDTH, 0 * GKEY_HEIGHT + ICON_START_Y, 4 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y},
    {4 * GKEY_WIDTH, 0 * GKEY_HEIGHT + ICON_START_Y, 5 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y},
    {5 * GKEY_WIDTH, 0 * GKEY_HEIGHT + ICON_START_Y, 6 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y},   // Del

    {0 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y, 1 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y},
    {1 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y, 2 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y},
    {2 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y, 3 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y},
    {3 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y, 4 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y},
    {4 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y, 5 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y},
    {5 * GKEY_WIDTH, 1 * GKEY_HEIGHT + ICON_START_Y, 6 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y},   // Space

    {0 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y, 1 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y},
    {1 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y, 2 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y},
    {2 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y, 3 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y},
    {3 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y, 4 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y},
    {4 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y, 5 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y},
    {5 * GKEY_WIDTH, 2 * GKEY_HEIGHT + ICON_START_Y, 6 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y},   // ABC to 123

    {0 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y, 1 * GKEY_WIDTH, 4 * GKEY_HEIGHT + ICON_START_Y},
    {1 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y, 2 * GKEY_WIDTH, 4 * GKEY_HEIGHT + ICON_START_Y},
    {2 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y, 3 * GKEY_WIDTH, 4 * GKEY_HEIGHT + ICON_START_Y},
    {3 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y, 4 * GKEY_WIDTH, 4 * GKEY_HEIGHT + ICON_START_Y},
    {4 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y, 5 * GKEY_WIDTH, 4 * GKEY_HEIGHT + ICON_START_Y},
    {5 * GKEY_WIDTH, 3 * GKEY_HEIGHT + ICON_START_Y, 6 * GKEY_WIDTH, 4 * GKEY_HEIGHT + ICON_START_Y},

    // Back
    {0 * GKEY_WIDTH + 5, 0 * GKEY_HEIGHT + 5, 1 * GKEY_WIDTH - 5, ICON_START_Y - 5},
    // Send
    {5 * GKEY_WIDTH + 5, 0 * GKEY_HEIGHT + 5, 6 * GKEY_WIDTH - 5, ICON_START_Y - 5},
};

const char *const softKeyValue[][26] = {
    {
        "1",
        "2",
        "3",
        "M",
        "G",
        "Del",
        "4",
        "5",
        "6",
        "X",
        "Y",
        "Space",
        "7",
        "8",
        "9",
        "Z",
        "E",
        "ABC",
        ".",
        "0",
        "-",
        "S",
        "T",
        " ",
        "Back",
        "Send",
    },
    {
        "A",
        "B",
        "C",
        "D",
        "F",
        "Del",
        "H",
        "I",
        "J",
        "K",
        "L",
        "Space",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "123",
        "U",
        "V",
        "W",
        " ",
        " ",
        " ",
        "Back",
        "Send",
    }};

SOFT_KEY_TYPE softKeyType = SOFT_KEY_123;

GKEY_VALUES GKeyGetValue(void) {
  return (GKEY_VALUES)KEY_GetValue(sizeof(rect_of_Gkey) / sizeof(rect_of_Gkey[0]), rect_of_Gkey);
}

void sendGcodeReDrawButton(u8 positon, u8 pressed) {
  if (positon > GKEY_SEND) return;
  if (pressed) {
    GUI_SetColor(WHITE);
    GUI_SetBkColor(BLACK);
  }
  GUI_ClearPrect(rect_of_Gkey + positon);
  GUI_DispStringInPrect(rect_of_Gkey + positon, (u8 *)softKeyValue[softKeyType][positon]);
  if (pressed) {
    GUI_SetColor(BLACK);
    GUI_SetBkColor(WHITE);
  }
}

void menuDrawSendGcode(void) {
  GUI_RECT gcodeRect = {rect_of_Gkey[GKEY_BACK].x1 + 10, rect_of_Gkey[GKEY_BACK].y0, rect_of_Gkey[GKEY_SEND].x0 - 10, rect_of_Gkey[GKEY_SEND].y1};

  GUI_SetBkColor(BLUE);
  GUI_ClearRect(0, 0, LCD_WIDTH, rect_of_Gkey[0].y0);
  GUI_SetBkColor(WHITE);
  GUI_ClearPrect(&gcodeRect);
  GUI_ClearRect(0, rect_of_Gkey[0].y0, LCD_WIDTH, LCD_HEIGHT);

  GUI_SetColor(YELLOW);
  GUI_FillPrect(&rect_of_Gkey[GKEY_BACK]);   // Back
  GUI_FillPrect(&rect_of_Gkey[GKEY_SEND]);   // Send
  GUI_SetColor(BLACK);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  for (uint8_t i = 0; i < COUNT(softKeyValue[0]); i++) {
    //    GUI_DrawPrect(rect_of_Gkey + i);  // Draw keyboard border
    GUI_DispStringInPrect(rect_of_Gkey + i, (u8 *)softKeyValue[softKeyType][i]);
  }
  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  TSC_ReDrawIcon = sendGcodeReDrawButton;
}

void menuSendGcode(void) {
  GUI_RECT gcodeRect = {rect_of_Gkey[GKEY_BACK].x1 + 10, rect_of_Gkey[GKEY_BACK].y0, rect_of_Gkey[GKEY_SEND].x0 - 10, rect_of_Gkey[GKEY_SEND].y1};

  char gcodeBuf[GCODE_MAX_CHARACTERS] = {0};
  uint8_t nowIndex                    = 0,
          lastIndex                   = 0;
  GKEY_VALUES key_num                 = GKEY_IDLE;
  menuDrawSendGcode();
  while (infoMenu.menu[infoMenu.active] == menuSendGcode) {
    key_num = GKeyGetValue();

    switch (key_num) {
      case GKEY_IDLE:
        break;

      case GKEY_BACK:
        infoMenu.active--;
        break;

      case GKEY_SEND:
        if (nowIndex) {
          gcodeBuf[nowIndex++] = '\n';   // End char '\n' for Gcode
          gcodeBuf[nowIndex]   = 0;
          storeCmd(gcodeBuf);
          gcodeBuf[nowIndex = 0] = 0;
        }
        infoMenu.menu[++infoMenu.active] = menuTerminal;
        break;

      case GKEY_ABC_123:
        softKeyType = (softKeyType == SOFT_KEY_123) ? SOFT_KEY_ABC : SOFT_KEY_123;
        for (uint8_t i = 0; i < GKEY_BACK; i++) {
          GUI_DispStringInPrect(rect_of_Gkey + i, (u8 *)softKeyValue[softKeyType][i]);
        }
        break;

      case GKEY_DEL:
        if (nowIndex)
          gcodeBuf[--nowIndex] = 0;
        break;

      case GKEY_SPACE:
        if (nowIndex < GCODE_MAX_CHARACTERS - 1) {
          gcodeBuf[nowIndex++] = ' ';
          gcodeBuf[nowIndex]   = 0;
        }
        break;

      default:
        if (nowIndex < GCODE_MAX_CHARACTERS - 1) {
          gcodeBuf[nowIndex++] = softKeyValue[softKeyType][key_num][0];
          gcodeBuf[nowIndex]   = 0;
        }
        break;
    }
    if (lastIndex != nowIndex) {
      lastIndex = nowIndex;
      GUI_ClearPrect(&gcodeRect);
      GUI_DispStringInPrect(&gcodeRect, (u8 *)gcodeBuf);
    }

    processGcode();
  }
  GUI_RestoreColorDefault();
}

#define TERMINAL_MAX_CHAR (LCD_WIDTH / BYTE_WIDTH * (LCD_HEIGHT - BYTE_HEIGHT) / BYTE_HEIGHT)

char terminalBuf[TERMINAL_MAX_CHAR];
void sendGcodeTerminalCache(char *serial_text, COMMAND_SOURCE src) {
  if (strstr(serial_text, (const char *)"wait") && src) return;
  const char *const terminalSign[] = {"Sent: ", "Rcv: "};
  // if (infoMenu.menu[infoMenu.active] != menuSendGcode && infoMenu.menu[infoMenu.active] != menuTerminal) return;
  if (strlen(terminalBuf) + strlen(serial_text) + strlen(terminalSign[src]) >= TERMINAL_MAX_CHAR) {
    terminalBuf[0] = 0;
  }
  strlcat(terminalBuf, terminalSign[src], TERMINAL_MAX_CHAR);
  strlcat(terminalBuf, serial_text, TERMINAL_MAX_CHAR);
}

#define CURSOR_START_X 0
#define CURSOR_END_X   LCD_WIDTH
#define CURSOR_START_Y (BYTE_HEIGHT * 2)
#define CURSOR_END_Y   LCD_HEIGHT
void menuTerminal(void) {
  const GUI_RECT terminalRect = {0, 0, LCD_WIDTH, LCD_HEIGHT};
  CHAR_INFO info;
  int16_t cursorX            = CURSOR_START_X,
          cursorY            = CURSOR_START_Y;
  uint16_t lastTerminalIndex = 0;
  u16 key_num                = IDLE_TOUCH;

  GUI_ClearRect(0, 0, LCD_WIDTH, CURSOR_START_Y);
  GUI_DispStringInRect(0, 0, LCD_WIDTH, CURSOR_START_Y, textSelect(LABEL_TOUCH_TO_EXIT));
  GUI_SetColor(BLACK);
  GUI_SetBkColor(GRAY);
  GUI_ClearRect(CURSOR_START_X, CURSOR_START_Y, CURSOR_END_X, CURSOR_END_Y);
  TSC_ReDrawIcon = NULL;   // Disable icon redraw callback function

  while (infoMenu.menu[infoMenu.active] == menuTerminal) {
    key_num = KEY_GetValue(1, &terminalRect);

    if (key_num != IDLE_TOUCH) {
      infoMenu.active--;
    }

    while (terminalBuf[lastTerminalIndex]) {
      getCharacterInfo((u8 *)&terminalBuf[lastTerminalIndex], &info);
      if (cursorX + info.pixelWidth > CURSOR_END_X || (terminalBuf[lastTerminalIndex] == '\n' && cursorX != CURSOR_START_X))   // Next Line
      {
        cursorX = CURSOR_START_X;
        cursorY += info.pixelHeight;
        if (cursorY + info.pixelHeight > CURSOR_END_Y) {
          lastTerminalIndex = 0;
          terminalBuf[0]    = 0;
          break;
        }
      }
      if (terminalBuf[lastTerminalIndex] != '\n') {
        if (cursorY + info.pixelHeight > CURSOR_END_Y)   // Clear window
        {
          cursorX = CURSOR_START_X;
          cursorY = CURSOR_START_Y;
          GUI_ClearRect(CURSOR_START_X, CURSOR_START_Y, CURSOR_END_X, CURSOR_END_Y);
        }
        GUI_DispOne(cursorX, cursorY, (u8 *)&terminalBuf[lastTerminalIndex]);
        cursorX += info.pixelWidth;
      }
      lastTerminalIndex += info.bytes;
    }

    processGcode();
  }
  GUI_RestoreColorDefault();
}

/**
 * showGcodeStatus.
 *
 * @version	v1.0.0	Sunday, February 23rd, 2020.
 * @version	v1.0.1	Monday, February 24th, 2020.
 * @global
 * @param	char        	*serial_text
 * @param	terminal_src	src
 * @return	void
 */
void showGcodeStatus(char *serial_text, COMMAND_SOURCE src) {
  static char *lastReceivedText;
  static char *lastSentText;
  // *Disable on pages that need the extra space
  if (infoMenu.menu[infoMenu.active] == menuSettings ||
      infoMenu.menu[infoMenu.active] == menuFeatureSettings ||
      infoMenu.menu[infoMenu.active] == menuPrint ||
      infoMenu.menu[infoMenu.active] == menuPrintFromSource ||
      infoMenu.menu[infoMenu.active] == menuSendGcode ||
      infoMenu.menu[infoMenu.active] == menuBeforePrinting ||
      infoMenu.menu[infoMenu.active] == menuMachineSettings ||
      infoMenu.menu[infoMenu.active] == parametersetting ||
      infoMenu.menu[infoMenu.active] == menuTerminal ||
      !infoHost.connected) {
    return;
  }
  // *If outgoing (sent) gcode, filter status commands from display
  if (!src) {
    const char *const ignored_commands[] = {"M105", "M118", "M114", "M117"};   // *G-code commands which won't be displayed
    for (u8 i = 0; i < COUNT(ignored_commands); i++) {
      if (strstr(serial_text, ignored_commands[i])) {
        return;   // *abort if filtered string found
      }
    }
  }
  const char *const prefix[] = {"Cmd:", "Rsp:"};
  int begin_x;
  int end_x;
  int width_x;
  char *final_text = serial_text;
  // *Strip out any leftover responses
  const char ch[5] = "\n";
  char *token;
  // *get the first token
  token = strtok(serial_text, ch);
  // *walk through other tokens
  while (token != NULL) {
    if (strlen(token) > 1) {
      final_text = token;
    }
    token = strtok(NULL, ch);
  }

  if (src) {   // *If incoming (Rcv), then display text in the Rcv section of the gcode status
    if (lastReceivedText == final_text) {
      return;   // *Don't update if the message hasn't changed (prevents flickering)
    } else {
      lastReceivedText = final_text;
    }
    begin_x = (LCD_WIDTH / 3) * 2;
    end_x   = LCD_WIDTH;
    width_x = (LCD_WIDTH / 3) - BYTE_WIDTH - BYTE_WIDTH * strlen(prefix[src]);

  } else {   // *else display text in the Sent section
    if (lastSentText == final_text) {
      return;   // *Don't update if the message hasn't changed since (prevents flickering)
    } else {
      lastSentText = final_text;
    }
    begin_x = 0;
    end_x   = (LCD_WIDTH / 3) * 2;
    width_x = (LCD_WIDTH / 3) * 2 - BYTE_WIDTH - BYTE_WIDTH * strlen(prefix[src]);
  }
  GUI_ClearRect(begin_x, BYTE_HEIGHT, end_x, BYTE_HEIGHT * 2);   // *clear the previous status
  GUI_SetColor(GRAY);
  GUI_DispString(begin_x, BYTE_HEIGHT, (u8 *)prefix[src]);   // Display the Sent/Rcv text
  GUI_RestoreColorDefault();

  if (infoMenu.menu[infoMenu.active] == menuTerminal) {
    GUI_ClearRect(0, BYTE_HEIGHT, LCD_WIDTH, BYTE_HEIGHT * 2);   // *remove the gcode status line to avoid confusion
  } else {
    GUI_DispLenString(begin_x + BYTE_WIDTH * strlen(prefix[src]), BYTE_HEIGHT, (u8 *)final_text, width_x);
  }
}
