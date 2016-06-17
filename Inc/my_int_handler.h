#ifndef __MY_INT_HANDLER_H
#define __MY_INT_HANDLER_H

#include "stm32f7xx_hal.h"

typedef void (*UART_TX_CPLT_FUNC)(UART_HandleTypeDef *huart);
typedef void (*UART_RX_CPLT_FUNC)(UART_HandleTypeDef *huart);
typedef struct{
	USART_TypeDef *instance;
	UART_TX_CPLT_FUNC tx_callback;
	UART_RX_CPLT_FUNC rx_callback;	
}STRU_UART_HANDLE_T;

typedef void (*TIMER_ELAPSED_FUNC)(TIM_HandleTypeDef *htim);
typedef struct{
	TIM_TypeDef *instance;
	TIMER_ELAPSED_FUNC timer_callback;	
}STRU_TIMER_HANDLE_T;

void UART_Register_TxCallback(USART_TypeDef *instance, UART_TX_CPLT_FUNC tx_callback);
void UART_Register_TxCallback(USART_TypeDef *instance, UART_TX_CPLT_FUNC tx_callback);
void Timer_Register_Callback(	TIM_TypeDef *instance, TIMER_ELAPSED_FUNC timer_callback);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif
