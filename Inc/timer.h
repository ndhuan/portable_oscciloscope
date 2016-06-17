#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include  <stdbool.h>

#define EXTERN extern

typedef void (*TIMER_CALLBACK_FUNC)(); 

typedef unsigned char TIMER_ID;

#define INVALID_TIMER_ID 0xff

EXTERN void Timer_Init(void);
EXTERN void Timer_GetTick(void); 
EXTERN void Timer_Handle(void);
EXTERN TIMER_ID TIMER_RegisterEvent(TIMER_CALLBACK_FUNC callback, unsigned long ms); //"ms" should be multiple of 100ms
EXTERN bool TIMER_UnregisterEvent(TIMER_ID timer_id);       //unregister a timeout event


#endif  //TIMER_H


