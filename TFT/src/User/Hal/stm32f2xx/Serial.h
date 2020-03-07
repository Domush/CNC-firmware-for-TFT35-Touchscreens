#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "stm32f2xx.h"
#include "parseACK.h"
#include "usart.h"

typedef struct
{
  char *cache;
  uint16_t pendingIndex;
  uint16_t parsedIndex;
} DMA_CIRCULAR_BUFFER;

#define DMA_TRANS_LEN MAX_RESPONSE_SIZE

extern DMA_CIRCULAR_BUFFER cncIncoming[_USART_CNT];

void Serial_DMAClearFlag(uint8_t port);
void Serial_Init(u32 baud);
void Serial_DeInit(void);
void Serial_Puts(uint8_t port, char *command, ...);

#endif
