#ifndef _LEDCOLOR_H_
#define _LEDCOLOR_H_
#include "includes.h"

//24bit                           // G  R  B
#define LED_OFF    0x00000000
#define LED_WHITE  0x00FFFFFF
#define LED_RED    0x0000FF00
#define LED_ORANGE 0x00FF8C00
#define LED_YELLOW 0x00FFFF00
#define LED_GREEN  0x00FF0000
#define LED_BLUE   0x000000FF
#define LED_INDIGO 0x004B0082D
#define LED_VIOLET 0x0000FEFE

/*Color macro definition*/      /*R G B*/
#define COLOR_BLACK      0x0000 /*   0,   0,   0 */
#define COLOR_NAVY       0x000F /*   0,   0, 128 */
#define COLOR_DARK_GREEN 0x03E0 /*   0, 128,   0 */
#define COLOR_DARK_CYAN  0x03EF /*   0, 128, 128 */
#define COLOR_MAROON     0x7800 /* 128,   0,   0 */
#define COLOR_PURPLE     0x780F /* 128,   0, 128 */
#define COLOR_OLIVE      0x7BE0 /* 128, 128,   0 */
#define COLOR_LIGHT_GREY 0xC618 /* 192, 192, 192 */
#define COLOR_DARK_GREY  0x7BEF /* 128, 128, 128 */
#define COLOR_BLUE       0x001F /*   0,   0, 255 */
#define COLOR_GREEN      0x07E0 /*   0, 255,   0 */
#define COLOR_CYAN       0x07FF /*   0, 255, 255 */
#define COLOR_RED        0xF800 /* 255,   0,   0 */
#define COLOR_MAGENTA    0xF81F /* 255,   0, 255 */
#define COLOR_YELLOW     0xFFE0 /* 255, 255, 0   */
#define COLOR_WHITE      0xFFFF /* 255, 255, 255 */

void led_color_Init(uint16_t psc, uint16_t arr);   //
void LED_color_PIN_IPN(void);
void ws2812_send_DAT(uint32_t ws2812_dat);
#endif
