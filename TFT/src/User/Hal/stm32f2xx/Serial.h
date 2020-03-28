#ifndef _SERIAL_H_
#define _SERIAL_H_
#include "includes.h"

#include "stm32f2xx.h"
// #include "Gcode/gcodeResponseHandler.h"
#include "usart.h"

typedef struct
{
  char *responseBuffer;
  uint16_t pendingIndex;
  uint16_t processedIndex;
} SERIAL_RECEIVE_BUFFER;

extern SERIAL_RECEIVE_BUFFER cncIncoming[_USART_CNT];

void Serial_DMAClearFlag(uint8_t port);
void Serial_Init(u32 baud);
void Serial_DeInit(void);
void Serial_Puts(uint8_t port, char *command, ...);

#endif
