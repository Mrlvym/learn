#include "freertos_soft_timer.h"

#include "hal_debug_usart.h"
#include "hal_led.h"
#include "hal_key.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#if (configUSE_TIMERS)
static uint32_t TmrCb_Count1 = 0;
static uint32_t TmrCb_Count2 = 0;

static TimerHandle_t Test_Timer1_Handle = NULL;
static TimerHandle_t Test_Timer2_Handle = NULL;
#endif /* #if (configUSE_TIMERS) */

static void Test_Timer1_CallBack_Func( TimerHandle_t xTimer );
static void Test_Timer2_CallBack_Func( TimerHandle_t xTimer );

void FreeRTOS_Soft_Timer_Bsp_Init(void)
{
	Hal_Led_Init();

	Hal_Key_Init();
}


void FreeRTOS_Soft_Timer_Task(void)
{
	FreeRTOS_Soft_Timer_Init();
}


void FreeRTOS_Soft_Timer_Init(void)
{
#if (configUSE_TIMERS)
	Test_Timer1_Handle = xTimerCreate((const char *) TEST_TIMER1_NAME,
									  (TickType_t) pdMS_TO_TICKS(1000),
									  (UBaseType_t) pdTRUE,
									  (void *) 1,
									  (TimerCallbackFunction_t) Test_Timer1_CallBack_Func);
	if(NULL != Test_Timer1_Handle){
		Hal_Debug(INFO_LEVEL, "Test Timer1 Creat Success ... \r\n");
		xTimerStart(Test_Timer1_Handle, 0);
	}

	Test_Timer2_Handle = xTimerCreate((const char *)TEST_TIMER2_NAME,
									  (TickType_t) pdMS_TO_TICKS(5000),
									  (UBaseType_t) pdTRUE,
									  (void *) 2,
									  (TimerCallbackFunction_t) Test_Timer2_CallBack_Func);
	if(NULL != Test_Timer2_Handle){
		Hal_Debug(INFO_LEVEL, "Test Timer2 Creat Success ... \r\n");	
		xTimerStart(Test_Timer2_Handle, 0);
	}
#endif /* #if (configUSE_TIMERS) */
}


static void Test_Timer1_CallBack_Func( TimerHandle_t xTimer )
{

#if (configUSE_TIMERS)
	TickType_t t1_tick_num = 0;

	t1_tick_num = xTaskGetTickCount();

	TmrCb_Count1++;

	Hal_Led_turn(LED0);
	
	Hal_Debug(PRINTF, "TmrCb_Count1 : %d \r\n", TmrCb_Count1);
	Hal_Debug(PRINTF, "t1_tick_num : %d \r\n", t1_tick_num);
#endif /* #if (configUSE_TIMERS) */

}
static void Test_Timer2_CallBack_Func( TimerHandle_t xTimer )
{
#if (configUSE_TIMERS)
	TickType_t t2_tick_num = 0;

	t2_tick_num = xTaskGetTickCount();

	TmrCb_Count2++;
	
	Hal_Led_turn(LED1);

	Hal_Debug(PRINTF, "TmrCb_Count12 : %d \r\n", TmrCb_Count2);
	Hal_Debug(PRINTF, "t2_tick_num : %d \r\n", t2_tick_num);
#endif /* #if (configUSE_TIMERS) */
}

