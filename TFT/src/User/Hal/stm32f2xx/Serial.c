#include "Serial.h"

// dma rx buffer
SERIAL_RECEIVE_BUFFER cncIncoming[_USART_CNT];

// Config for USART Channel
//USART1 RX DMA2 Channel4 Steam2/5
//USART2 RX DMA1 Channel4 Steam5
//USART3 RX DMA1 Channel4 Steam1
//UART4  RX DMA1 Channel4 Steam2
//UART5  RX DMA1 Channel4 Steam0
//USART6 RX DMA2 Channel5 Steam1/2

// Config for USART Channel
typedef struct
{
  USART_TypeDef *uart;
  uint32_t dma_rcc;
  uint8_t dma_channel;
  DMA_Stream_TypeDef *dma_stream;
} SERIAL_CFG;

static const SERIAL_CFG Serial[_USART_CNT] = {
    {USART1, RCC_AHB1Periph_DMA2, 4, DMA2_Stream2},
    {USART2, RCC_AHB1Periph_DMA1, 4, DMA1_Stream5},
    {USART3, RCC_AHB1Periph_DMA1, 4, DMA1_Stream1},
    {UART4, RCC_AHB1Periph_DMA1, 4, DMA1_Stream2},
    {UART5, RCC_AHB1Periph_DMA1, 4, DMA1_Stream0},
    {USART6, RCC_AHB1Periph_DMA2, 5, DMA2_Stream1},
};

void Serial_DMA_Config(uint8_t port) {
  const SERIAL_CFG *cfg = &Serial[port];

  RCC_AHB1PeriphClockCmd(cfg->dma_rcc, ENABLE);   // DMA RCC EN

  cfg->dma_stream->CR &= ~(1 << 0);   // Disable DMA
  Serial_DMAClearFlag(port);
  cfg->uart->CR3 |= 1 << 6;   // DMA enable receiver

  cfg->dma_stream->PAR  = (u32)(&cfg->uart->DR);
  cfg->dma_stream->M0AR = (u32)(cncIncoming[port].responseBuffer);
  cfg->dma_stream->NDTR = RESPONSE_BUFFER_SIZE;

  cfg->dma_stream->CR = cfg->dma_channel << 25;
  cfg->dma_stream->CR |= 3 << 16;   // Priority level: Very high
  cfg->dma_stream->CR |= 0 << 13;   // Memory data size: 8
  cfg->dma_stream->CR |= 0 << 11;   // Peripheral data size: 8
  cfg->dma_stream->CR |= 1 << 10;   // Memory increment mode
  cfg->dma_stream->CR |= 0 << 9;    // Peripheral not increment mode
  cfg->dma_stream->CR |= 1 << 8;    // Circular mode enabled
  cfg->dma_stream->CR |= 0 << 6;    // Data transfer direction: Peripheral-to-memory
  cfg->dma_stream->CR |= 1 << 0;    // Enable DMA
}

void Serial_Config(uint8_t port, u32 baud) {
  cncIncoming[port].processedIndex = cncIncoming[port].pendingIndex = 0;
  cncIncoming[port].responseBuffer                                  = malloc(RESPONSE_BUFFER_SIZE);
  while (!cncIncoming[port].responseBuffer)
    ;                                        // malloc failed
  USART_Config(port, baud, USART_IT_IDLE);   // IDLE interrupt
  Serial_DMA_Config(port);
}

void Serial_DeConfig(uint8_t port) {
  free(cncIncoming[port].responseBuffer);
  cncIncoming[port].responseBuffer = NULL;
  Serial[port].dma_stream->CR &= ~(1 << 0);   // Disable DMA
  Serial_DMAClearFlag(port);
  USART_DeConfig(port);
}

void Serial_Init(u32 baud) {
  Serial_Config(SERIAL_PORT, baud);

#ifdef SERIAL_PORT_2
  Serial_Config(SERIAL_PORT_2, baud);
#endif

#ifdef SERIAL_PORT_3
  Serial_Config(SERIAL_PORT_3, baud);
#endif

#ifdef SERIAL_PORT_4
  Serial_Config(SERIAL_PORT_4, baud);
#endif
}

void Serial_DeInit(void) {
  Serial_DeConfig(SERIAL_PORT);

#ifdef SERIAL_PORT_2
  Serial_DeConfig(SERIAL_PORT_2);
#endif

#ifdef SERIAL_PORT_3
  Serial_DeConfig(SERIAL_PORT_3);
#endif

#ifdef SERIAL_PORT_4
  Serial_DeConfig(SERIAL_PORT_4);
#endif
}

void Serial_DMAClearFlag(uint8_t port) {
  switch (port) {
    case _USART1:
      DMA2->LIFCR = (0x3F << 16);
      break;   // DMA2_Stream2 low  bits:16-21
    case _USART2:
      DMA1->HIFCR = (0xFC << 4);
      break;   // DMA1_Stream5 high bits: 6-11
    case _USART3:
      DMA1->LIFCR = (0xFC << 4);
      break;   // DMA1_Stream1 low  bits: 6-11
    case _UART4:
      DMA1->LIFCR = (0x3F << 16);
      break;   // DMA1_Stream2 low  bits:16-21
    case _UART5:
      DMA1->LIFCR = (0x3F << 0);
      break;   // DMA1_Stream0 low  bits: 0-5
    case _USART6:
      DMA2->LIFCR = (0xFC << 4);
      break;   // DMA2_Stream1 low  bits: 6-11
  }
}

void USART_IRQHandler(uint8_t port) {
  if ((Serial[port].uart->SR & (1 << 4)) != 0) {
    Serial[port].uart->SR;
    Serial[port].uart->DR;

    cncIncoming[port].pendingIndex = RESPONSE_BUFFER_SIZE - Serial[port].dma_stream->NDTR;
    uint16_t pendingIndex          = (cncIncoming[port].pendingIndex == 0) ? RESPONSE_BUFFER_SIZE : cncIncoming[port].pendingIndex;
    if (cncIncoming[port].responseBuffer[pendingIndex - 1] == '\n') {
      // Receive completed
      infoHost.responseReceived[port] = true;
    }
  }
}

void USART1_IRQHandler(void) {
  USART_IRQHandler(_USART1);
}

void USART2_IRQHandler(void) {
  USART_IRQHandler(_USART2);
}

void USART3_IRQHandler(void) {
  USART_IRQHandler(_USART3);
}

void UART4_IRQHandler(void) {
  USART_IRQHandler(_UART4);
}

void UART5_IRQHandler(void) {
  USART_IRQHandler(_UART5);
}

void USART6_IRQHandler(void) {
  USART_IRQHandler(_USART6);
}

void sendCommand(uint8_t port, char *command, ...) {
  char buffer[GCODE_MAX_CHARACTERS];
  char *gString;
  my_va_list ap;
  my_va_start(ap, command);
  my_vsprintf(buffer, command, ap);
  my_va_end(ap);
  gString = &buffer[0];

  while (*gString) {
    while ((Serial[port].uart->SR & USART_FLAG_TC) == (uint16_t)RESET)
      ;
    Serial[port].uart->DR = ((u16)*gString++ & (uint16_t)0x01FF);
  }
}

#include "stdio.h"
int fputc(int ch, FILE *f) {
  while ((Serial[SERIAL_PORT].uart->SR & 0X40) == 0)
    ;
  Serial[SERIAL_PORT].uart->DR = (u8)ch;
  return ch;
}
