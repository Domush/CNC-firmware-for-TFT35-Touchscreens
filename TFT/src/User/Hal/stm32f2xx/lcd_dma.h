#ifndef _LCD_DMA_H_
#define _LCD_DMA_H_
#include "includes.h"

// #include "System/variants.h"

void LCD_DMA_Config(void);
void lcd_frame_display(u16 sx, u16 sy, u16 w, u16 h, u32 addr);

#endif
