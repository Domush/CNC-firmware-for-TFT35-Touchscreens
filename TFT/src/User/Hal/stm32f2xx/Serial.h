#ifndef _SERIAL_H_
#define _SERIAL_H_
#include "stm32f2xx.h"

typedef struct SERIAL_RECEIVE_BUFFER {
  char *responseBuffer;
  uint16_t pendingIndex;
  uint16_t processedIndex;
} SERIAL_RECEIVE_BUFFER;

extern SERIAL_RECEIVE_BUFFER cncIncoming[_USART_CNT];

void Serial_DMAClearFlag(uint8_t port);
void Serial_Init(u32 baud);
void Serial_DeInit(void);
void sendCommand(uint8_t port, char *command, ...);

#endif
