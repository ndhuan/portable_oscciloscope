#ifndef __PWM_H
#define __PWM_H

#include <stdint.h>

#define TIM_PWM TIM12
#define TIM_PWM_CHANNEL TIM_CHANNEL_2
#define TIM_PWM_CLK_ENABLE() __HAL_RCC_TIM12_CLK_ENABLE()
#define TIM_PWM_PORT_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIM_PWM_GPIO_AF GPIO_AF9_TIM12
#define TIM_PWM_PIN GPIO_PIN_15
#define TIM_PWM_PORT GPIOB

void PWM_Config(void);
void PWM_Set(uint32_t freq, uint32_t duty);

#endif
