#include "app_init.h"
#include "hal_delay.h"
#include "hal_debug_usart.h"
#include "FreeRTOS.h"
#include "task.h"

#include "freertos_led.h"
#include "freertos_key.h"
#include "freertos_queue.h"
#include "freertos_semaphore.h"
#include "freertos_mutex.h"
#include "freertos_event.h"
#include "freertos_soft_timer.h"
#include "freertos_notify.h"
#include "freertos_CPU_usageRate.h"
#include "freertos_memory_manager.h"


/* 创建任务句柄 */
static TaskHandle_t AppTaskCreat_Handle = NULL;

static void AppTaskCreate(void);/* 用于创建任务 */


void App_HW_Init(void)
{
	RCC_ClocksTypeDef RCC_Clocks;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//	Hal_Delay_Init(168);
	
	Hal_Debug_Usart_Config();
	
	RCC_GetClocksFreq(&RCC_Clocks);
	Hal_Debug(DEBUG_LEVEL, "sysclk : %d, hclk : %d, pclk1 : %d, pclk2 : %d \r\n", \
						  RCC_Clocks.SYSCLK_Frequency,\
						  RCC_Clocks.HCLK_Frequency,\
						  RCC_Clocks.PCLK1_Frequency,\
						  RCC_Clocks.PCLK2_Frequency);
}


void App_SW_Init(void)
{
	
}

void App_FreeRTOS_BSP_Init(void)
{
	RCC_ClocksTypeDef RCC_Clocks;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

//	Hal_Delay_Init(168);
	
	Hal_Debug_Usart_Config();
	
	RCC_GetClocksFreq(&RCC_Clocks);
	Hal_Debug(DEBUG_LEVEL, "sysclk : %d, hclk : %d, pclk1 : %d, pclk2 : %d \r\n", \
						  RCC_Clocks.SYSCLK_Frequency,\
						  RCC_Clocks.HCLK_Frequency,\
						  RCC_Clocks.PCLK1_Frequency,\
						  RCC_Clocks.PCLK2_Frequency);

//	FreeRTOS_Led_Init();

//	FreeRTOS_Key_Init();

//	FreeRTOS_Queue_Bsp_Init();

//	FreeRTOS_Semaphore_Bsp_Init();

//	FreeRTOS_Mutex_Bsp_Init();

//	FreeRTOS_Event_Bsp_Init();

//	FreeRTOS_Soft_Timer_Bsp_Init();

//	FreeRTOS_Notify_Bsp_Init();

//	FreeRTOS_CPU_UsageRate_Bsp_Init();

	FreeRTOS_Memory_Manger_Bsp_Init();
}

void App_FreeRTOS_Task_Init(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(	(TaskFunction_t) AppTaskCreate,
							(const char * )"AppTaskCreate",
							(uint16_t) 512,
							(void * )NULL,
							(UBaseType_t )1,
							(TaskHandle_t * )&AppTaskCreat_Handle);

	if(pdPASS == xReturn)
		vTaskStartScheduler();
	else
		return ;
}



static void AppTaskCreate(void)
{
	taskENTER_CRITICAL(); //进入临界区

	{
//		FreeRTOS_Led_Task();

//		FreeRTOS_Key_Task();

//		FreeRTOS_Queue_Task();

//		FreeRTOS_Semaphore_Task();

//		FreeRTOS_Mutex_Task();

//		FreeRTOS_Event_Task();

//		FreeRTOS_Soft_Timer_Task();

//		FreeRTOS_Notify_Task();

//		FreeRTOS_CPU_UsageRate_Task();

		FreeRTOS_Memory_Manger_Task();

		vTaskDelete(AppTaskCreat_Handle);
	}

	taskEXIT_CRITICAL(); //退出临界区
}




