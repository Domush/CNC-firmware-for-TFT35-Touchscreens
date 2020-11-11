#ifndef TFT_SRC_USER_HAL_STM32F2XX_LCD_DMA
#define TFT_SRC_USER_HAL_STM32F2XX_LCD_DMA

void LCD_DMA_Config(void);
void lcd_frame_display(u16 sx, u16 sy, u16 w, u16 h, u32 addr);

#endif   // TFT_SRC_USER_HAL_STM32F2XX_LCD_DMA
