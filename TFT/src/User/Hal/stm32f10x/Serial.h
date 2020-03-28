#ifndef _SERIAL_H_
#define _SERIAL_H_
#include "includes.h"

#include "stm32f10x.h"
// #include "Gcode/gcodeResponseHandler.h"
#include "usart.h"

typedef struct
{
  char *responseBuffer;      // Incoming cncResponse buffer
  uint16_t pendingIndex;     // Index of cncResponse which is waiting to be parsed
  uint16_t processedIndex;   // Index of cncResponse which has been processed
} SERIAL_RECEIVE_BUFFER;

extern SERIAL_RECEIVE_BUFFER cncIncoming[_USART_CNT];

void Serial_Init(u32 baud);
void Serial_DeInit(void);
void Serial_Puts(uint8_t port, char *command, ...);

#endif
