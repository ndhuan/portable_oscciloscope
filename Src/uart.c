#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h> 
#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"
#include "def.h"
#include "uart.h"
#include "my_int_handler.h"

#define TX_BUFF_SIZE 1000
#define MAX_MSG_LEN 100

UART_HandleTypeDef UartHandle;
static volatile bool tx_done = true;
#warning check if msg is placed in DMA-enabled mem
static volatile uint8_t tx_buff[TX_BUFF_SIZE];
static volatile uint8_t msg[MAX_MSG_LEN];
int tx_head=0, tx_tail=0;
int tx_last_head=0;

static void USARTx_TxCpltCallback(UART_HandleTypeDef *huart);

void UART_Config(void)
{
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;

	UART_Register_TxCallback(USARTx, USARTx_TxCpltCallback);
	
	if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    ;
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    ;
  }	
}
void UART_Transmit(const uint8_t *pTxData, uint16_t Size)
{
	int i=0;
	int tx_head_tmp = tx_head;
	int tx_tail_tmp = tx_tail;	
	while (Size--)
	{
		tx_buff[tx_head_tmp++] = pTxData[i++];
		if (tx_head_tmp == TX_BUFF_SIZE)
			tx_head_tmp = 0;
		if (tx_head_tmp == tx_tail)//overflow
		{
			break;
		}
	}
	tx_head = tx_head_tmp;
	//if (UartHandle.gState == HAL_UART_STATE_READY)//Check that a Tx process is not already ongoing
	if (tx_done)
	{
		if (tx_tail_tmp != tx_head_tmp)
		{
			int size;
			tx_done = false;
			BSP_LED_Off(LED1);
			if (tx_head_tmp > tx_tail_tmp)
				size = tx_head_tmp - tx_tail_tmp;
			else
				size = tx_head_tmp - tx_tail_tmp + TX_BUFF_SIZE;
			if (size > MAX_MSG_LEN)
			{
				size = MAX_MSG_LEN;
			}
			for (i=0;i<size;i++)
			{
				msg[i] = tx_buff[(tx_tail+i)%TX_BUFF_SIZE];
			}
			HAL_UART_Transmit_DMA(&UartHandle, msg, size);
			tx_last_head = (tx_tail + size)%TX_BUFF_SIZE;
		}
	}
}
void UART_Print(const char * __restrict format, ...) {
	volatile char Tx_Buf[512];
	va_list arg;

	memset((void *) Tx_Buf, 0, sizeof(Tx_Buf));
	//
	// Start the varargs processing.
	//
	va_start(arg, format);

	//
	// Call vsnprintf to perform the conversion.  Use a large number for the
	// buffer size.
	//
	vsprintf((char *) Tx_Buf, format, arg);

	//
	// End the varargs processing.
	//
	va_end(arg);

	UART_Transmit((const uint8_t *) Tx_Buf, strlen((const char *) Tx_Buf));
}
/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA  
  *         used for USART data transmission     
  */

void USARTx_TxCpltCallback(UART_HandleTypeDef *huart)
{
	int tx_tail_tmp;
	int tx_head_tmp;
	tx_tail = tx_last_head;
	tx_tail_tmp = tx_tail;
	tx_head_tmp = tx_head;
	if (tx_tail_tmp != tx_head_tmp)
	{
		int size;
		int i;
		tx_done = false;
		BSP_LED_Off(LED1);
		if (tx_head_tmp > tx_tail_tmp)
			size = tx_head_tmp - tx_tail_tmp;
		else
			size = tx_head_tmp - tx_tail_tmp + TX_BUFF_SIZE;
		for (i=0;i<size;i++)
		{
			msg[i] = tx_buff[(tx_tail+i)%TX_BUFF_SIZE];
		}
		//HAL_UART_Transmit_DMA(&UartHandle, msg, size);
		HAL_UART_Transmit_DMA(&UartHandle, msg, size);
		tx_last_head = (tx_tail + size)%TX_BUFF_SIZE;
	}
	else
	{
		tx_done = true;
		BSP_LED_On(LED1);
	}
}

