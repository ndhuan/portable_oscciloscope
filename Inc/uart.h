#ifndef __UART_H
#define __UART_H

#include <stdint.h>

void UART_Config(void);
void UART_Transmit(const uint8_t *pTxData, uint16_t Size);
void UART_Print(const char * __restrict format, ...);
#endif
