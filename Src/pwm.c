#include "stm32f7xx_hal.h"
#include "pwm.h"
TIM_HandleTypeDef    TimPWMHandle;
TIM_OC_InitTypeDef sConfig;
uint32_t uhPrescalerValue = 0;
void PWM_Config(void)
{
  /* Compute the prescaler value to have TIM2 counter clock equal to 20000 Hz */
  //uhPrescalerValue = (uint32_t)((SystemCoreClock/2) / 20000) - 1;	
	
	TimPWMHandle.Instance = TIM_PWM;

  TimPWMHandle.Init.Prescaler         = 0;
  TimPWMHandle.Init.Period            = (uint32_t)((SystemCoreClock/2) / 20000) - 1;
  TimPWMHandle.Init.ClockDivision     = 0;
  TimPWMHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimPWMHandle.Init.RepetitionCounter = 0;
  if (HAL_TIM_PWM_Init(&TimPWMHandle) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }	
  /*##-2- Configure the PWM channels #########################################*/
  /* Common configuration for all channels */
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

  /* Set the pulse value for channel 1 */
  sConfig.Pulse = TimPWMHandle.Init.Period/2;
  if (HAL_TIM_PWM_ConfigChannel(&TimPWMHandle, &sConfig, TIM_PWM_CHANNEL) != HAL_OK)
  {
    /* Configuration Error */
    while(1);
  }	
	
  /* Start channel 1 */
  if (HAL_TIM_PWM_Start(&TimPWMHandle, TIM_PWM_CHANNEL) != HAL_OK)
  {
    /* PWM Generation Error */
    while(1);
  }	
}
void PWM_Set(uint32_t freq, uint32_t duty)
{
	uint32_t period;
  period = (uint32_t)((SystemCoreClock/2) / freq) - 1;	
	if (duty>100)
		duty = 100;		
	__HAL_TIM_SET_AUTORELOAD(&TimPWMHandle, period);
	__HAL_TIM_SET_COMPARE(&TimPWMHandle, TIM_PWM_CHANNEL, period*duty/100);
}
