#include "GUI.h"
#include "includes.h"

uint16_t foreGroundColor  = WHITE;
uint16_t backGroundColor  = BLACK;
GUI_TEXT_MODE guiTextMode = GUI_TEXTMODE_NORMAL;
GUI_NUM_MODE guiNumMode   = GUI_NUMMODE_SPACE;

void LCD_SetWindow(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey) {
  LCD_WR_REG(0x2A);
  LCD_WR_DATA(sx >> 8);
  LCD_WR_DATA(sx & 0xFF);
  LCD_WR_DATA(ex >> 8);
  LCD_WR_DATA(ex & 0xFF);
  LCD_WR_REG(0x2B);
  LCD_WR_DATA(sy >> 8);
  LCD_WR_DATA(sy & 0xFF);
  LCD_WR_DATA(ey >> 8);
  LCD_WR_DATA(ey & 0xFF);
}

void GUI_SetColor(uint16_t color) {
  foreGroundColor = color;
}

uint16_t GUI_GetColor(void) {
  return foreGroundColor;
}

void GUI_SetBkColor(uint16_t bkcolor) {
  backGroundColor = bkcolor;
}

uint16_t GUI_GetBkColor(void) {
  return backGroundColor;
}

void GUI_SetTextMode(GUI_TEXT_MODE mode) {
  guiTextMode = mode;
}

GUI_TEXT_MODE GUI_GetTextMode(void) {
  return guiTextMode;
}
void GUI_SetNumMode(GUI_NUM_MODE mode) {
  guiNumMode = mode;
}

GUI_NUM_MODE GUI_GetNumMode(void) {
  return guiNumMode;
}

void GUI_Clear(uint16_t color) {
  uint32_t index = 0;
  LCD_SetWindow(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
  LCD_WR_REG(0x2C);
  for (index = 0; index < LCD_WIDTH * LCD_HEIGHT; index++) {
    LCD_WR_16BITS_DATA(color);
  }
}

static uint8_t pixel_limit_flag = 0;
static GUI_RECT pixel_limit_rect;

void GUI_SetRange(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  pixel_limit_flag    = 1;
  pixel_limit_rect.x0 = x0;
  pixel_limit_rect.x1 = x1;
  pixel_limit_rect.y0 = y0;
  pixel_limit_rect.y1 = y1;
}

void GUI_CancelRange(void) {
  pixel_limit_flag = 0;
}

void GUI_DrawPixel(int16_t x, int16_t y, uint16_t color) {
  if (pixel_limit_flag == 1 && (x < pixel_limit_rect.x0 || x >= pixel_limit_rect.x1 || y < pixel_limit_rect.y0 || y >= pixel_limit_rect.y1))
    return;

  LCD_SetWindow(x, y, x, y);
  LCD_WR_REG(0x2C);
  LCD_WR_16BITS_DATA(color);
}

void GUI_DrawPoint(uint16_t x, uint16_t y) {
  LCD_SetWindow(x, y, x, y);
  LCD_WR_REG(0x2C);
  LCD_WR_16BITS_DATA(foreGroundColor);
}

void GUI_FillRect(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey) {
  uint16_t i = 0, j = 0;
  LCD_SetWindow(sx, sy, ex - 1, ey - 1);
  LCD_WR_REG(0x2C);
  for (i = sx; i < ex; i++) {
    for (j = sy; j < ey; j++) {
      LCD_WR_16BITS_DATA(foreGroundColor);
    }
  }
}

void GUI_FillPrect(const GUI_RECT *dimensions) {
  GUI_FillRect(dimensions->x0, dimensions->y0, dimensions->x1, dimensions->y1);
}

void GUI_ClearRect(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey) {
  uint16_t i = 0, j = 0;
  LCD_SetWindow(sx, sy, ex - 1, ey - 1);
  LCD_WR_REG(0x2C);
  for (i = sx; i < ex; i++) {
    for (j = sy; j < ey; j++) {
      LCD_WR_16BITS_DATA(backGroundColor);
    }
  }
}

void GUI_ClearPrect(const GUI_RECT *dimensions) {
  GUI_ClearRect(dimensions->x0, dimensions->y0, dimensions->x1, dimensions->y1);
}

void GUI_FillRectColor(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color) {
  uint16_t i = 0, j = 0;
  LCD_SetWindow(sx, sy, ex - 1, ey - 1);
  LCD_WR_REG(0x2C);
  for (i = sx; i < ex; i++) {
    for (j = sy; j < ey; j++) {
      LCD_WR_16BITS_DATA(color);
    }
  }
}
void GUI_FillRectArry(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint8_t *arry) {
  uint16_t i = 0, j = 0, color;
  LCD_SetWindow(sx, sy, ex - 1, ey - 1);
  LCD_WR_REG(0x2C);
  for (i = sx; i < ex; i++) {
    for (j = sy; j < ey; j++) {
      color = *arry;
      arry++;
      color = (color << 8) | (*arry);
      arry++;
      LCD_WR_16BITS_DATA(color);
    }
  }
}

//Copy
//x1,y1:Do you want to copy?
//x2,y2:Copy
void GUI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  uint16_t t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;
  delta_x = x2 - x1;   //������������
  delta_y = y2 - y1;
  uRow    = x1;
  uCol    = y1;
  if (delta_x > 0)
    incx = 1;   //Should be copied
  else if (delta_x == 0)
    incx = 0;   //Span
  else {
    incx    = -1;
    delta_x = -delta_x;
  }

  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0;   //Horizontal copy
  else {
    incy    = -1;
    delta_y = -delta_y;
  }

  if (delta_x > delta_y)
    distance = delta_x;   //ѡȡ��������������
  else
    distance = delta_y;

  for (t = 0; t <= distance + 1; t++)   //�������?
  {
    GUI_DrawPoint(uRow, uCol);   //Copy
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance) {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance) {
      yerr -= distance;
      uCol += incy;
    }
  }
}

#include "math.h"
void GUI_DrawAngleLine(uint16_t x, uint16_t y, uint16_t radius, int16_t angle) {
  uint16_t ex, ey;
  float a = angle * 3.1415926f / 180;
  ex      = x + cos(a) * radius;
  ey      = y - sin(a) * radius;
  GUI_DrawLine(x, y, ex, ey);
}
void GUI_HLine(uint16_t x1, uint16_t y, uint16_t x2) {
  uint16_t i = 0;
  LCD_SetWindow(x1, y, x2 - 1, y);
  LCD_WR_REG(0x2C);
  for (i = x1; i < x2; i++) {
    LCD_WR_16BITS_DATA(foreGroundColor);
  }
}
void GUI_VLine(uint16_t x, uint16_t y1, uint16_t y2) {
  uint16_t i = 0;
  LCD_SetWindow(x, y1, x, y2 - 1);
  LCD_WR_REG(0x2C);
  for (i = y1; i < y2; i++) {
    LCD_WR_16BITS_DATA(foreGroundColor);
  }
}

//Copy
//(x1,y1),(x2,y2):���εĶԽ�����
void GUI_DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  GUI_HLine(x1, y1, x2);
  GUI_HLine(x1, y2 - 1, x2);
  GUI_VLine(x1, y1, y2);
  GUI_VLine(x2 - 1, y1, y2);
}

void GUI_DrawPrect(const GUI_RECT *dimensions) {
  GUI_DrawRect(dimensions->x0, dimensions->y0, dimensions->x1, dimensions->y1);
}

//� �
//(x, y): copy
//radius: The commandment path
void GUI_DrawCircle(uint16_t x0, uint16_t y0, uint16_t radius) {
  int16_t a  = 0,
          b  = radius,
          di = 3 - (radius << 1);   //Howling

  while (a <= b) {
    GUI_DrawPoint(x0 + a, y0 - b);   //5
    GUI_DrawPoint(x0 + b, y0 - a);   //0
    GUI_DrawPoint(x0 + b, y0 + a);   //4
    GUI_DrawPoint(x0 + a, y0 + b);   //6
    GUI_DrawPoint(x0 - a, y0 + b);   //1
    GUI_DrawPoint(x0 - b, y0 + a);
    GUI_DrawPoint(x0 - a, y0 - b);   //2
    GUI_DrawPoint(x0 - b, y0 - a);   //7
    a++;
    //Make a copy of Bresenham's sister-in-law
    if (di < 0)
      di += (a << 2) + 6;
    else {
      di += 10 + ((a - b) << 2);
      b--;
    }
  }
}
void GUI_FillCircle(uint16_t x0, uint16_t y0, uint16_t radius) {
  int16_t draw_x0, draw_y0;   // ��ͼ���������?
  int16_t draw_x1, draw_y1;
  int16_t draw_x2, draw_y2;
  int16_t draw_x3, draw_y3;
  int16_t draw_x4, draw_y4;
  int16_t draw_x5, draw_y5;
  int16_t draw_x6, draw_y6;
  int16_t draw_x7, draw_y7;
  int16_t fill_x0, fill_y0;   // �������ı�����ʹ�ô�ֱ�����?
  int16_t fill_x1;
  int16_t xx, yy;   // ��Բ���Ʊ���

  int16_t di;   // Do

  /* �������� */
  if (0 == radius) return;

  /* �����?4�������?(0��90��180��270��)��������ʾ */
  draw_x0 = draw_x1 = x0;
  draw_y0 = draw_y1 = y0 + radius;
  if (draw_y0 < LCD_HEIGHT) {
    GUI_DrawPoint(draw_x0, draw_y0);   // 90 kg
  }

  draw_x2 = draw_x3 = x0;
  draw_y2 = draw_y3 = y0 - radius;
  if (draw_y2 >= 0) {
    GUI_DrawPoint(draw_x2, draw_y2);   // 270 pounds
  }

  draw_x4 = draw_x6 = x0 + radius;
  draw_y4 = draw_y6 = y0;
  if (draw_x4 < LCD_WIDTH) {
    GUI_DrawPoint(draw_x4, draw_y4);   // 0
    fill_x1 = draw_x4;
  } else {
    fill_x1 = LCD_WIDTH;
  }

  fill_y0 = y0;            // �������������ʼ��fill_x0
  fill_x0 = x0 - radius;   // �����������������fill_y1
  if (fill_x0 < 0)
    fill_x0 = 0;

  GUI_HLine(fill_x0, fill_y0, fill_x1);

  draw_x5 = draw_x7 = x0 - radius;
  draw_y5 = draw_y7 = y0;
  if (draw_x5 >= 0) {
    GUI_DrawPoint(draw_x5, draw_y5);   // 180 kg
  }
  if (1 == radius) return;

  /* Bresenham */
  di = 3 - 2 * radius;   // ��ʼ�����߱���
  xx = 0;
  yy = radius;
  while (xx < yy) {
    if (di < 0) {
      di += 4 * xx + 6;
    } else {
      di += 4 * (xx - yy) + 10;

      yy--;
      draw_y0--;
      draw_y1--;
      draw_y2++;
      draw_y3++;
      draw_x4--;
      draw_x5++;
      draw_x6--;
      draw_x7++;
    }

    xx++;
    draw_x0++;
    draw_x1--;
    draw_x2++;
    draw_x3--;
    draw_y4++;
    draw_y5++;
    draw_y6--;
    draw_y7--;

    /* Ҫ�жϵ�ǰ���Ƿ�����Ч��Χ�� */
    if ((draw_x0 <= LCD_WIDTH) && (draw_y0 >= 0)) {
      GUI_DrawPoint(draw_x0, draw_y0);
    }
    if ((draw_x1 >= 0) && (draw_y1 >= 0)) {
      GUI_DrawPoint(draw_x1, draw_y1);
    }

    /* �ڶ���ˮֱ�����?(�°�Բ�ĵ�) */
    if (draw_x1 >= 0) { /* �������������ʼ��fill_x0 */
      fill_x0 = draw_x1;
      /* �������������ʼ��fill_y0 */
      fill_y0 = draw_y1;
      if (fill_y0 > LCD_HEIGHT)
        fill_y0 = LCD_HEIGHT;
      if (fill_y0 < 0)
        fill_y0 = 0;
      /* �����������������fill_x1 */
      fill_x1 = x0 * 2 - draw_x1;
      if (fill_x1 > LCD_WIDTH)
        fill_x1 = LCD_WIDTH;
      GUI_HLine(fill_x0, fill_y0, fill_x1);
    }

    if ((draw_x2 <= LCD_WIDTH) && (draw_y2 <= LCD_HEIGHT)) {
      GUI_DrawPoint(draw_x2, draw_y2);
    }

    if ((draw_x3 >= 0) && (draw_y3 <= LCD_HEIGHT)) {
      GUI_DrawPoint(draw_x3, draw_y3);
    }

    /* Do you want to copy the point vertically? Do you want to copy it? */
    if (draw_x3 >= 0) { /* �������������ʼ��fill_x0 */
      fill_x0 = draw_x3;
      /* �������������ʼ��fill_y0 */
      fill_y0 = draw_y3;
      if (fill_y0 > LCD_HEIGHT)
        fill_y0 = LCD_HEIGHT;
      if (fill_y0 < 0)
        fill_y0 = 0;
      /* �����������������fill_x1 */
      fill_x1 = x0 * 2 - draw_x3;
      if (fill_x1 > LCD_WIDTH)
        fill_x1 = LCD_WIDTH;
      GUI_HLine(fill_x0, fill_y0, fill_x1);
    }

    if ((draw_x4 <= LCD_WIDTH) && (draw_y4 >= 0)) {
      GUI_DrawPoint(draw_x4, draw_y4);
    }
    if ((draw_x5 >= 0) && (draw_y5 >= 0)) {
      GUI_DrawPoint(draw_x5, draw_y5);
    }

    /* Do you copy my sister vertically? */
    if (draw_x5 >= 0) { /* �������������ʼ��fill_x0 */
      fill_x0 = draw_x5;
      /* �������������ʼ��fill_y0 */
      fill_y0 = draw_y5;
      if (fill_y0 > LCD_HEIGHT)
        fill_y0 = LCD_HEIGHT;
      if (fill_y0 < 0)
        fill_y0 = 0;
      /* �����������������fill_x1 */
      fill_x1 = x0 * 2 - draw_x5;
      if (fill_x1 > LCD_WIDTH)
        fill_x1 = LCD_WIDTH;
      GUI_HLine(fill_x0, fill_y0, fill_x1);
    }

    if ((draw_x6 <= LCD_WIDTH) && (draw_y6 <= LCD_HEIGHT)) {
      GUI_DrawPoint(draw_x6, draw_y6);
    }

    if ((draw_x7 >= 0) && (draw_y7 <= LCD_HEIGHT)) {
      GUI_DrawPoint(draw_x7, draw_y7);
    }

    /* � section eight o'clock vertical copy? */
    if (draw_x7 >= 0) { /* �������������ʼ��fill_x0 */
      fill_x0 = draw_x7;
      /* �������������ʼ��fill_y0 */
      fill_y0 = draw_y7;
      if (fill_y0 > LCD_HEIGHT)
        fill_y0 = LCD_HEIGHT;
      if (fill_y0 < 0)
        fill_y0 = 0;
      /* �����������������fill_x1 */
      fill_x1 = x0 * 2 - draw_x7;
      if (fill_x1 > LCD_WIDTH)
        fill_x1 = LCD_WIDTH;
      GUI_HLine(fill_x0, fill_y0, fill_x1);
    }
  }
}

//
CHAR_INFO GUI_DispOne(int16_t sx, int16_t sy, const uint8_t *string) {
  CHAR_INFO info = {.bytes = 0};

  if (string == NULL || *string == 0) return info;

  getCharacterInfo(string, &info);

  uint8_t x           = 0,
          y           = 0,
          j           = 0,
          i           = 0;
  uint16_t bitMapSize = (info.pixelHeight * info.pixelWidth / 8);
  uint8_t font[bitMapSize];
  uint32_t temp = 0;

  W25Qxx_ReadBuffer(font, info.bitMapAddr, bitMapSize);

  for (x = 0; x < info.pixelWidth; x++) {
    for (j = 0; j < (info.pixelHeight + 8 - 1) / 8; j++) {
      temp <<= 8;
      temp |= font[i++];
    }

    for (y = 0; y < info.pixelHeight; y++) {
      if (temp & (1 << (info.pixelHeight - 1)))
        GUI_DrawPixel(sx, sy + y, foreGroundColor);
      else if (guiTextMode == GUI_TEXTMODE_NORMAL)
        GUI_DrawPixel(sx, sy + y, backGroundColor);
      temp <<= 1;
    }
    sx++;
  }
  return info;
}

void GUI_DispString(int16_t x, int16_t y, const uint8_t *string) {
  CHAR_INFO info;
  if (string == NULL) return;

  while (*string) {
    info = GUI_DispOne(x, y, string);
    x += info.pixelWidth;
    string += info.bytes;
  }
}

const uint8_t *GUI_DispLenString(int16_t x, int16_t y, const uint8_t *string, uint16_t pixelWidth) {
  CHAR_INFO info;
  uint16_t curPixelWidth = 0;
  if (string == NULL) return NULL;

  while (curPixelWidth < pixelWidth && *string) {
    getCharacterInfo(string, &info);
    if (curPixelWidth + info.pixelWidth > pixelWidth) return string;
    GUI_DispOne(x, y, string);
    x += info.pixelWidth;
    curPixelWidth += info.pixelWidth;
    string += info.bytes;
  }
  return string;
}

void GUI_DispStringRight(int16_t x, int16_t y, const uint8_t *string) {
  x -= GUI_StrPixelWidth(string);
  GUI_DispString(x, y, string);
}

void GUI_DispStringInRect(int16_t sx, int16_t sy, int16_t ex, int16_t ey, const uint8_t *string) {
  uint16_t stringlen = GUI_StrPixelWidth(string);
  uint16_t width     = ex - sx;
  uint16_t height    = ey - sy;
  uint8_t nline      = (stringlen + width - 1) / width;

  if (nline > height / BYTE_HEIGHT)
    // nline = height / BYTE_HEIGHT;
    height = BYTE_HEIGHT;

  uint16_t x_offset = stringlen >= width ? 0 : (width - stringlen) >> 1;
  uint16_t y_offset = (nline * BYTE_HEIGHT) >= height ? 0 : ((height - (nline * BYTE_HEIGHT)) >> 1);
  uint16_t x = sx + x_offset, y = sy + y_offset;

  uint8_t i = 0;
  for (i = 0; i < nline; i++) {
    string = GUI_DispLenString(x, y, string, width);
    y += BYTE_HEIGHT;
  }
}

void GUI_DispStringInPrect(const GUI_RECT *dimensions, const uint8_t *string) {
  GUI_DispStringInRect(dimensions->x0, dimensions->y0, dimensions->x1, dimensions->y1, string);
}

void GUI_DispStringInRectEOL(int16_t sx, int16_t sy, int16_t ex, int16_t ey, const uint8_t *string) {
  if (string == NULL || *string == 0) return;
  CHAR_INFO info;
  int16_t x = sx;
  while (*string) {
    getCharacterInfo(string, &info);
    if (x + info.pixelWidth > ex || (*string == '\n' && x != sx)) {
      x = sx;
      sy += info.pixelHeight;
      if (sy + info.pixelHeight > ey) return;
    }
    if (*string != '\n') {
      GUI_DispOne(x, sy, string);
      x += info.pixelWidth;
    }
    string += info.bytes;
  }
}

const uint32_t GUI_Pow10[10] = {
    1, 10, 100, 1000, 10000,
    100000, 1000000, 10000000, 100000000, 1000000000};

void GUI_DispDec(int16_t x, int16_t y, int32_t value, uint8_t numDigits, uint8_t LEFTorRIGHT) {
  uint8_t i;
  uint8_t bit_value;
  uint8_t blank_bit_len = 0;
  uint8_t notZero       = 0;
  char isNegative       = 0;
  uint8_t decBuf[64];
  uint8_t bufIndex = 0;

  if (value < 0) {
    value      = -value;
    isNegative = 1;
    numDigits--;   // Negative '-' takes up a display length
  }
  for (i = 0; i < numDigits; i++) {
    bit_value = (value / GUI_Pow10[numDigits - i - 1]) % 10;
    if (notZero == 0) {
      if (bit_value == 0 && i < (numDigits - 1)) {
        if (LEFTorRIGHT == RIGHT) {
          decBuf[bufIndex++] = (guiNumMode == GUI_NUMMODE_SPACE) ? ' ' : '0';
        } else {
          blank_bit_len++;
        }
        continue;
      } else {
        notZero = 1;
        if (isNegative) {
          decBuf[bufIndex++] = '-';
        }
      }
    }
    decBuf[bufIndex++] = bit_value + '0';
  }
  for (; blank_bit_len > 0; blank_bit_len--) {
    decBuf[bufIndex++] = ' ';
  }
  decBuf[bufIndex] = 0;
  GUI_DispString(x, y, decBuf);
}

void GUI_DispFloat(int16_t x, int16_t y, float value, uint8_t leftDigits, uint8_t rightDigits, uint8_t LEFTorRIGHT) {
  uint8_t alen    = 0;
  uint8_t i       = 0;
  uint8_t notZero = 0;
  char isNegative = 0;
  uint8_t floatBuf[64];
  uint8_t bufIndex = 0;

  if (value < 0) {
    value      = -value;
    isNegative = 1;
    leftDigits--;   // Negative '-' takes up a display length
  }

  value *= GUI_Pow10[(unsigned)rightDigits];
  value += 0.5f;
  value = (float)floor(value);
  for (i = 0; i < leftDigits; i++) {
    uint8_t bit_value = ((uint32_t)(value / GUI_Pow10[leftDigits + rightDigits - 1 - i])) % 10;
    if (notZero == 0) {
      if (bit_value == 0 && i < (leftDigits - 1)) {
        if (LEFTorRIGHT == RIGHT) {
          floatBuf[bufIndex++] = (guiNumMode == GUI_NUMMODE_SPACE) ? ' ' : '0';
          alen++;
        }
        continue;
      } else {
        notZero = 1;
        if (isNegative) {
          floatBuf[bufIndex++] = '-';
        }
      }
    }
    floatBuf[bufIndex++] = bit_value + '0';
    alen++;
  }
  floatBuf[bufIndex++] = '.';
  alen++;

  for (i = 0; i < rightDigits; i++) {
    floatBuf[bufIndex++] = (int)(value / GUI_Pow10[rightDigits - 1 - i]) % 10 + '0';
    alen++;
  }
  for (; alen < leftDigits + rightDigits + 1; alen++) {
    floatBuf[bufIndex++] = ' ';
  }
  floatBuf[bufIndex] = 0;
  GUI_DispString(x, y, floatBuf);
}

/****************************************************     Widget    *******************************************************************/
#define RADIO_SELECTED_COLOR GREEN
#define RADIO_IDLE_COLOR WHITE
void RADIO_Create(RADIO *radioInfo) {
  u16 origColor = GUI_GetColor();
  uint8_t i     = 0;
  for (i = 0; i < radioInfo->value; i++) {
    if (i == radioInfo->select)
      GUI_SetColor(RADIO_SELECTED_COLOR);
    else
      GUI_SetColor(RADIO_IDLE_COLOR);
    GUI_FillCircle(radioInfo->sx + BYTE_HEIGHT / 2, i * radioInfo->distance + radioInfo->sy + BYTE_HEIGHT / 2, BYTE_HEIGHT / 8);
    GUI_DrawCircle(radioInfo->sx + BYTE_HEIGHT / 2, i * radioInfo->distance + radioInfo->sy + BYTE_HEIGHT / 2, BYTE_HEIGHT / 4);
    GUI_DispString(radioInfo->sx + BYTE_HEIGHT, i * radioInfo->distance + radioInfo->sy, radioInfo->context[i]);
  }
  GUI_SetColor(origColor);
}

void RADIO_Select(RADIO *radioInfo, uint8_t select) {
  u16 origColor = GUI_GetColor();
  uint8_t i     = 0;
  if (radioInfo->select == select)
    return;
  for (i = 0; i < 2; i++) {
    if (i == 0) {
      GUI_SetColor(RADIO_IDLE_COLOR);
    } else {
      radioInfo->select = select;
      GUI_SetColor(RADIO_SELECTED_COLOR);
    }
    GUI_FillCircle(radioInfo->sx + BYTE_HEIGHT / 2, radioInfo->select * radioInfo->distance + radioInfo->sy + BYTE_HEIGHT / 2, BYTE_HEIGHT / 8);
    GUI_DrawCircle(radioInfo->sx + BYTE_HEIGHT / 2, radioInfo->select * radioInfo->distance + radioInfo->sy + BYTE_HEIGHT / 2, BYTE_HEIGHT / 4);
    GUI_DispString(radioInfo->sx + BYTE_HEIGHT, radioInfo->select * radioInfo->distance + radioInfo->sy, radioInfo->context[radioInfo->select]);
  }
  GUI_SetColor(origColor);
}

//
void Scroll_CreatePara(SCROLL *scrollInfo, uint8_t *pstr, const GUI_RECT *dimensions) {
  memset(scrollInfo, 0, sizeof(SCROLL));
  scrollInfo->text          = pstr;
  scrollInfo->maxByte       = strlen((char *)pstr);
  scrollInfo->curPixelWidth = scrollInfo->totalPixelWidth = GUI_StrPixelWidth(pstr);
  scrollInfo->maxPixelWidth                               = dimensions->x1 - dimensions->x0;
  scrollInfo->dimensions                                  = dimensions;
}

void Scroll_DispString(SCROLL *scrollInfo, uint8_t align) {
  uint16_t i = 0;
  CHAR_INFO info;

  if (scrollInfo->text == NULL) return;
  if (scrollInfo->totalPixelWidth > scrollInfo->maxPixelWidth) {
    if (OS_GetTime() - scrollInfo->time > 5)   // 50ms
    {
      scrollInfo->time = OS_GetTime();
      GUI_SetRange(scrollInfo->dimensions->x0, scrollInfo->dimensions->y0, scrollInfo->dimensions->x1, scrollInfo->dimensions->y1);
      if (scrollInfo->curByte < scrollInfo->maxByte) {
        getCharacterInfo(&scrollInfo->text[scrollInfo->curByte], &info);
        scrollInfo->off_head++;
        if (scrollInfo->off_head == info.pixelWidth) {
          scrollInfo->curByte += info.bytes;
          scrollInfo->off_head = 0;
        }

        GUI_DispLenString(scrollInfo->dimensions->x0 - scrollInfo->off_head, scrollInfo->dimensions->y0, &scrollInfo->text[scrollInfo->curByte], scrollInfo->maxPixelWidth + info.pixelWidth);

        scrollInfo->curPixelWidth--;
        if (scrollInfo->curPixelWidth < scrollInfo->maxPixelWidth) {
          for (i = scrollInfo->dimensions->y0; i < scrollInfo->dimensions->y1; i++) {
            GUI_DrawPixel(scrollInfo->dimensions->x0 + scrollInfo->curPixelWidth, i, backGroundColor);
          }
        }
      }

      if (scrollInfo->curPixelWidth + 2 * BYTE_WIDTH < scrollInfo->maxPixelWidth) {
        scrollInfo->off_tail++;
        GUI_DispLenString(scrollInfo->dimensions->x1 - scrollInfo->off_tail, scrollInfo->dimensions->y0, scrollInfo->text, scrollInfo->off_tail);
        if (scrollInfo->off_tail + scrollInfo->dimensions->x0 >= scrollInfo->dimensions->x1) {
          scrollInfo->off_head      = 0;
          scrollInfo->off_tail      = 0;
          scrollInfo->curByte       = 0;
          scrollInfo->curPixelWidth = scrollInfo->totalPixelWidth;
        }
      }
      GUI_CancelRange();
    }
  } else if (!scrollInfo->has_disp) {
    switch (align) {
      case LEFT: {
        GUI_DispString(scrollInfo->dimensions->x0, scrollInfo->dimensions->y0, scrollInfo->text);
        break;
      }
      case RIGHT: {
        uint16_t x_offset = (scrollInfo->dimensions->x1 - scrollInfo->totalPixelWidth);
        GUI_DispString(x_offset, scrollInfo->dimensions->y0, scrollInfo->text);
        break;
      }
      case CENTER: {
        uint16_t x_offset = ((scrollInfo->dimensions->x1 - scrollInfo->dimensions->x0 - scrollInfo->totalPixelWidth) >> 1);
        GUI_DispString(scrollInfo->dimensions->x0 + x_offset, scrollInfo->dimensions->y0, scrollInfo->text);
        break;
      }
    }
    scrollInfo->has_disp = 1;
  }
}

//
void GUI_DrawButton(const BUTTON *buttonInfo, uint8_t pressed) {
  const uint16_t radius           = buttonInfo->radius;
  const uint16_t lineWidth        = buttonInfo->lineWidth;
  const int16_t sx                = buttonInfo->dimensions.x0,
                sy                = buttonInfo->dimensions.y0,
                ex                = buttonInfo->dimensions.x1,
                ey                = buttonInfo->dimensions.y1;
  const uint16_t nowBackColor     = GUI_GetBkColor();
  const uint16_t nowFontColor     = GUI_GetColor();
  const GUI_TEXT_MODE nowTextMode = GUI_GetTextMode();

  const uint16_t lineColor = pressed ? buttonInfo->pLineColor : buttonInfo->lineColor;
  const uint16_t backColor = pressed ? buttonInfo->pBackColor : buttonInfo->backColor;
  const uint16_t fontColor = pressed ? buttonInfo->pFontColor : buttonInfo->fontColor;

  GUI_SetColor(lineColor);
  GUI_FillCircle(sx + radius, sy + radius, radius);   //�ĸ��ǵ�Բ��
  GUI_FillCircle(ex - radius - 1, sy + radius, radius);
  GUI_FillCircle(sx + radius, ey - radius - 1, radius);
  GUI_FillCircle(ex - radius - 1, ey - radius - 1, radius);

  for (uint16_t i = 0; i < lineWidth; i++) {
    GUI_HLine(sx + radius, sy + i, ex - radius);   //�ĸ����?
    GUI_HLine(sx + radius, ey - 1 - i, ex - radius);
    GUI_VLine(sx + i, sy + radius, ey - radius);
    GUI_VLine(ex - 1 - i, sy + radius, ey - radius);
  }
  GUI_SetColor(backColor);
  GUI_FillCircle(sx + radius, sy + radius, radius - lineWidth);   //����ĸ��ǵ�Բ��?
  GUI_FillCircle(ex - radius - 1, sy + radius, radius - lineWidth);
  GUI_FillCircle(sx + radius, ey - radius - 1, radius - lineWidth);
  GUI_FillCircle(ex - radius - 1, ey - radius - 1, radius - lineWidth);
  GUI_FillRect(sx + radius, sy + lineWidth, ex - radius, sy + lineWidth + radius);
  GUI_FillRect(sx + lineWidth, sy + lineWidth + radius, ex - lineWidth, ey - lineWidth - radius);
  GUI_FillRect(sx + radius, ey - lineWidth - radius, ex - radius, ey - lineWidth);

  GUI_SetColor(fontColor);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_DispStringInPrect(&buttonInfo->dimensions, buttonInfo->context);

  GUI_SetBkColor(nowBackColor);
  GUI_SetColor(nowFontColor);
  GUI_SetTextMode(nowTextMode);
}

void GUI_DrawWindow(const WINDOW *windowInfo, const uint8_t *title, const uint8_t *content) {
  const uint16_t titleHeight      = windowInfo->title.height;
  const uint16_t infoHeight       = windowInfo->info.height;
  const uint16_t radius           = windowInfo->radius;
  const uint16_t lineWidth        = windowInfo->lineWidth;
  const uint16_t lineColor        = windowInfo->lineColor;
  const uint16_t infoBackColor    = windowInfo->info.backColor;
  const uint16_t bottomBackColor  = windowInfo->bottom.backColor;
  const int16_t sx                = windowInfo->dimensions.x0,
                sy                = windowInfo->dimensions.y0,
                ex                = windowInfo->dimensions.x1,
                ey                = windowInfo->dimensions.y1;
  const uint16_t nowBackColor     = GUI_GetBkColor();
  const uint16_t nowFontColor     = GUI_GetColor();
  const GUI_TEXT_MODE nowTextMode = GUI_GetTextMode();

  GUI_SetColor(lineColor);
  GUI_FillCircle(sx + radius, sy + radius, radius);
  GUI_FillCircle(ex - radius - 1, sy + radius, radius);
  GUI_FillRect(sx + radius, sy, ex - radius, sy + radius);
  GUI_FillRect(sx, sy + radius, ex, sy + titleHeight);
  for (uint16_t i = 0; i < lineWidth; i++) {
    GUI_VLine(sx + i, sy + titleHeight, ey - radius);
    GUI_VLine(ex - 1 - i, sy + titleHeight, ey - radius);
    GUI_HLine(sx + radius, ey - 1 - i, ex - radius);
  }
  GUI_FillCircle(sx + radius, ey - radius - 1, radius);
  GUI_FillCircle(ex - radius - 1, ey - radius - 1, radius);

  GUI_SetColor(infoBackColor);
  GUI_FillRect(sx + lineWidth, sy + titleHeight, ex - lineWidth, sy + titleHeight + infoHeight);
  GUI_SetColor(bottomBackColor);
  GUI_FillCircle(sx + radius, ey - radius - 1, radius - lineWidth);
  GUI_FillCircle(ex - radius - 1, ey - radius - 1, radius - lineWidth);
  GUI_FillRect(sx + lineWidth, sy + titleHeight + infoHeight, ex - lineWidth, ey - lineWidth - radius);
  GUI_FillRect(sx + lineWidth + radius, ey - lineWidth - radius, ex - lineWidth - radius, ey - lineWidth);

  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_SetColor(windowInfo->title.fontColor);
  //    GUI_DispStringInRect(dimensions.x0, dimensions.y0, dimensions.x1, dimensions.y0+titleHeight,title,0);
  // GUI_DispString(sx + radius, sy, title);
  GUI_SetColor(windowInfo->info.fontColor);
  GUI_DispStringInRect(sx + lineWidth + BYTE_WIDTH, sy + titleHeight, ex - lineWidth - BYTE_WIDTH, sy + titleHeight + infoHeight, content);

  GUI_SetBkColor(nowBackColor);
  GUI_SetColor(nowFontColor);
  GUI_SetTextMode(nowTextMode);
}
