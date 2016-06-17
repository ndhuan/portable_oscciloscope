#include <stdint.h>
#include "stm32f7xx_hal.h"
#include "timer.h"
#include "my_int_handler.h"
#define TIMER_PERIOD_MS 10
#define MAX_TIMEOUT_EVT 10

typedef struct
{
  TIMER_CALLBACK_FUNC callback;
  unsigned long period_cnt;
}TIMEOUT_EVT;

static TIMEOUT_EVT timer_event_list[MAX_TIMEOUT_EVT];
static bool b_init_done = false;
TIM_HandleTypeDef    TimHandle;
static void TIM3_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Timer_Init()
{
  if (!b_init_done)
  {
		uint32_t uwPrescalerValue = 0;
		/* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
		uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 10000) - 1;

		/* Set TIMx instance */
		TimHandle.Instance = TIM3;

		/* Initialize TIMx peripheral as follows:
				 + Period = 10000 - 1
				 + Prescaler = ((SystemCoreClock / 2)/10000) - 1
				 + ClockDivision = 0
				 + Counter direction = Up
		*/
		TimHandle.Init.Period            = 10*TIMER_PERIOD_MS - 1;
		TimHandle.Init.Prescaler         = uwPrescalerValue;
		TimHandle.Init.ClockDivision     = 0;
		TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
		TimHandle.Init.RepetitionCounter = 0;

		Timer_Register_Callback(TIM3, TIM3_PeriodElapsedCallback);
		
		if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
		{
			/* Initialization Error */
			;
		}

		/*##-2- Start the TIM Base generation in interrupt mode ####################*/
		/* Start Channel1 */
		if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
		{
			/* Starting Error */
			;
		}		
  }
}

TIMER_ID TIMER_RegisterEvent(TIMER_CALLBACK_FUNC callback, unsigned long ms)
{
  int i;
  for(i=0; i< MAX_TIMEOUT_EVT; i++)
  {
    if(timer_event_list[i].period_cnt == 0) break;
  }
  if(i == MAX_TIMEOUT_EVT) return INVALID_TIMER_ID;
  
  timer_event_list[i].period_cnt = (unsigned long)(ms/TIMER_PERIOD_MS);
  timer_event_list[i].callback = callback;
 
  return (TIMER_ID)i;  
}

bool TIMER_UnregisterEvent(TIMER_ID timer_id)
{
  bool ret = false;
  if(timer_id < MAX_TIMEOUT_EVT)
  {
      timer_event_list[timer_id].period_cnt = 0;
      timer_event_list[timer_id].callback = NULL;
      ret = true;
  }
  return ret;  
}
void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}
void TIM3_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  int i;
  for(i=0; i<MAX_TIMEOUT_EVT; i++)
  {
      if(timer_event_list[i].period_cnt > 0)
      {
          timer_event_list[i].period_cnt--;
          if(timer_event_list[i].period_cnt == 0 && timer_event_list[i].callback != NULL)
          {
            (timer_event_list[i].callback)();
						if (timer_event_list[i].period_cnt == 0)
							timer_event_list[i].callback = NULL;
          }
      }
  }
}
