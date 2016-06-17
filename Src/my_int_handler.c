#include "stm32f7xx_hal.h"
#include "my_int_handler.h"
#include "def.h"
STRU_UART_HANDLE_T uart_handler[]={
	{USART6,0,0}
};
STRU_TIMER_HANDLE_T timer_handler[]={
	{TIM3,0}
};

void UART_Register_TxCallback(USART_TypeDef *instance, UART_TX_CPLT_FUNC tx_callback)
{
	int i;
	for (i=0;i<SIZEOF_ARRAY(uart_handler);i++)
	{
		if (uart_handler[i].instance == instance)
		{
			uart_handler[i].tx_callback = tx_callback;
			break;
		}
	}
}
void UART_Register_RxCallback(USART_TypeDef *instance, UART_RX_CPLT_FUNC rx_callback)
{
	int i;
	for (i=0;i<SIZEOF_ARRAY(uart_handler);i++)
	{
		if (uart_handler[i].instance == instance)
		{
			uart_handler[i].rx_callback = rx_callback;
			break;
		}
	}	
}
void Timer_Register_Callback(	TIM_TypeDef *instance, TIMER_ELAPSED_FUNC timer_callback)
{
	int i;
	for (i=0;i<SIZEOF_ARRAY(timer_handler);i++)
	{
		if (timer_handler[i].instance == instance)
		{
			timer_handler[i].timer_callback = timer_callback;
			break;
		}
	}	
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	int i;
	for (i=0;i<SIZEOF_ARRAY(uart_handler);i++)
	{
		if (uart_handler[i].instance == huart->Instance)
		{
			if (uart_handler[i].tx_callback)
				uart_handler[i].tx_callback(huart);
			break;
		}
	}		
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	int i;
	for (i=0;i<SIZEOF_ARRAY(uart_handler);i++)
	{
		if (uart_handler[i].instance == huart->Instance)
		{
			if (uart_handler[i].rx_callback)
				uart_handler[i].rx_callback(huart);
			break;
		}
	}	
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	int i;
	for (i=0;i<SIZEOF_ARRAY(timer_handler);i++)
	{
		if (timer_handler[i].instance == htim->Instance)
		{
			if (timer_handler[i].timer_callback)
				timer_handler[i].timer_callback(htim);
			break;
		}
	}
}
