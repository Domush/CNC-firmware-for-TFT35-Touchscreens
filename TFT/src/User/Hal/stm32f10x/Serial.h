#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "stm32f10x.h"
#include "parseACK.h"
#include "usart.h"

typedef struct
{
  char *cache;             // Incoming cncResponse buffer
  uint16_t pendingIndex;   // Index of cncResponse which is waiting to be parsed
  uint16_t parsedIndex;    // Index of cncResponse which has been processed
} DMA_CIRCULAR_BUFFER;

#define DMA_TRANS_LEN MAX_RESPONSE_SIZE

extern DMA_CIRCULAR_BUFFER cncIncoming[_USART_CNT];

void Serial_Init(u32 baud);
void Serial_DeInit(void);
void Serial_Puts(uint8_t port, char *command, ...);

#endif
