#include "stm32f7xx_hal.h"

#define TIM_PERIOD_US 5
#define SEGMENT_SIZE 1024

/* TIM handler declaration */
static TIM_HandleTypeDef  htim;
ADC_HandleTypeDef    AdcHandle;
extern UART_HandleTypeDef UartHandle;
uint16_t Scan_Buffer[SEGMENT_SIZE];
static void Tim_ADC_Config()
{
  TIM_MasterConfigTypeDef sMasterConfig;

  /* Time Base configuration */
  htim.Instance = TIM2;

  htim.Init.Period            = 50*TIM_PERIOD_US;//APB1 CLK=50MHz
  htim.Init.Prescaler         = 0;
  htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.RepetitionCounter = 0x0;

  if (HAL_TIM_Base_Init(&htim) != HAL_OK)
  {
    /* Timer initialization Error */
    while(1);
  }

  /* Timer TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    while(1);
  }
	while (HAL_TIM_Base_Start(&htim) != HAL_OK);
}
void ADC_Start()
{
	while (HAL_ADC_Start_DMA(&AdcHandle,
        (uint32_t *)Scan_Buffer,
				SEGMENT_SIZE) != HAL_OK);
//	if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
//  {
//    /* Start Conversation Error */
//    while(1);
//  }
}
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim);
}
void ADC_Stop()
{
	//HAL_TIM_Base_Stop(&htim);	
}
void ADC_Config(void)
{
  ADC_ChannelConfTypeDef sConfig;

  /* ADC Initialization */
  AdcHandle.Instance          = ADC3;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;//25 MHz
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T2_TRGO;       /* Conversion start trigged at each external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization Error */
    while(1);
  }

  /* Configure ADC regular channel */
  sConfig.Channel      = ADC_CHANNEL_8;
  sConfig.Rank         = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    while(1);
  }

	Tim_ADC_Config();
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
	//HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)Scan_Buffer, SEGMENT_SIZE*2);	
}
