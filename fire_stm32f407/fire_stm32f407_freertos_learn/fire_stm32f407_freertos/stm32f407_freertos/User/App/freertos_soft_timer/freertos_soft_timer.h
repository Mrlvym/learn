#ifndef __FREERTOS_SOFT_TIMER_H__
#define __FREERTOS_SOFT_TIMER_H__

#define TEST_TIMER1_NAME	"Test_Timer1"
#define TEST_TIMER2_NAME	"Test_Timer2"


void FreeRTOS_Soft_Timer_Bsp_Init(void);

void FreeRTOS_Soft_Timer_Task(void);

void FreeRTOS_Soft_Timer_Init(void);

#endif

