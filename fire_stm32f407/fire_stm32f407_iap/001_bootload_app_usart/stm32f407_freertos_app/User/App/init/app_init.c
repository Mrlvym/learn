#include "app_init.h"
#include "hal_delay.h"
#include "hal_debug_usart.h"
#include "FreeRTOS.h"
#include "task.h"

#include "freertos_led.h"


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

	#define NVIC_VTOR_MASK       0x3FFFFF80
	#define APP_PART_ADDR        0x08004000
	SCB->VTOR = APP_PART_ADDR & NVIC_VTOR_MASK;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

//	Hal_Delay_Init(168);
	
	Hal_Debug_Usart_Config();
	
	RCC_GetClocksFreq(&RCC_Clocks);
	Hal_Debug(DEBUG_LEVEL, "sysclk : %d, hclk : %d, pclk1 : %d, pclk2 : %d \r\n", \
						  RCC_Clocks.SYSCLK_Frequency,\
						  RCC_Clocks.HCLK_Frequency,\
						  RCC_Clocks.PCLK1_Frequency,\
						  RCC_Clocks.PCLK2_Frequency);

	FreeRTOS_Led_Init();

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
		FreeRTOS_Led_Task();

		vTaskDelete(AppTaskCreat_Handle);
	}

	taskEXIT_CRITICAL(); //退出临界区
}




