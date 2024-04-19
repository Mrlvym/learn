#include "freertos_CPU_usageRate.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hal_debug_usart.h"
#include "hal_timer.h"
#include "hal_led.h"

/* LED1  ������ */
static TaskHandle_t LED1_Task1_Handle = NULL;

/* LED2  ������ */
static TaskHandle_t LED2_Task1_Handle = NULL;

#ifdef USE_FreeRTOS_CPU_UsageRate
static TaskHandle_t CPU_UsageRate_Task_Handle = NULL;
#endif /* #ifdef USE_FreeRTOS_CPU_UsageRate */


static void LED1_Task1(void* pvParameters);/* LED1_Task  ����ʵ�� */
static void LED2_Task1(void* pvParameters);/* LED2_Task  ����ʵ�� */

#ifdef USE_FreeRTOS_CPU_UsageRate
static void CPU_UsageRate_Task(void* pvParameters);
#endif /* #ifdef USE_FreeRTOS_CPU_UsageRate */




void FreeRTOS_CPU_UsageRate_Bsp_Init(void)
{
	Hal_Led_Init();

#ifdef USE_FreeRTOS_CPU_UsageRate
	Hal_TIMx_Cfg();
#endif /* #ifdef USE_FreeRTOS_CPU_UsageRate */
}

void FreeRTOS_CPU_UsageRate_Task(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(	(TaskFunction_t) LED1_Task1,
							(const char * )"LED1_Task1",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )2,
							(TaskHandle_t * )&LED1_Task1_Handle);

	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "LED1_Task1 Creat success ! \r\n");
	}

	xReturn = xTaskCreate(	(TaskFunction_t) LED2_Task1,
							(const char * )"LED2_Task1",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )2,
							(TaskHandle_t * )&LED2_Task1_Handle);

	if(pdPASS == xReturn){	
		Hal_Debug(INFO_LEVEL, "LED2_Task1 Creat success ! \r\n");
	}
#ifdef USE_FreeRTOS_CPU_UsageRate	
	xReturn = xTaskCreate(	(TaskFunction_t) CPU_UsageRate_Task,
							(const char * )"CPU_UsageRate_Task",
							(uint16_t) 512,
							(void * )NULL,
							(UBaseType_t )3,
							(TaskHandle_t * )&CPU_UsageRate_Task_Handle);

	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "CPU_UsageRate_Task Creat success ! \r\n");
	}
#endif /* #ifdef USE_FreeRTOS_CPU_UsageRate */

}

static void LED1_Task1(void* pvParameters)
{
	uint32_t i = 0;
	
	while(1)
	{
		Hal_Led0_on();
		for(i = 0; i < 200000; i++){}
		vTaskDelay(1000);
		
		Hal_Led0_off();
		for(i = 0; i < 200000; i++){}
		vTaskDelay(1000);
	}
}

static void LED2_Task1(void* pvParameters)
{
	while(1)
	{
		Hal_Led1_on();
		vTaskDelay(500);
		
		Hal_Led1_off();
		vTaskDelay(500);		
	}
}

#ifdef USE_FreeRTOS_CPU_UsageRate
	static void CPU_UsageRate_Task(void* pvParameters)
	{
		uint8_t CPU_RunInfo[400];
	
		while(1)
		{
			memset(CPU_RunInfo, 0, 400);
			vTaskList((char *)&CPU_RunInfo); //��ȡ��������ʱ����Ϣ

			Hal_Debug(PRINTF, "*********************************************\r\n");
			Hal_Debug(PRINTF, "������ ����״̬ ���ȼ� ʣ��ջ �������\r\n");
			Hal_Debug(PRINTF, "%s\r\n", CPU_RunInfo);
			Hal_Debug(PRINTF, "*********************************************\r\n\r\n");

			memset(CPU_RunInfo, 0, 400);
			vTaskGetRunTimeStats((char *)&CPU_RunInfo);

			Hal_Debug(PRINTF, "*********************************************\r\n");
			Hal_Debug(PRINTF, "������ ���м��� ʹ����\r\n");
			Hal_Debug(PRINTF, "%s\r\n", CPU_RunInfo);
			Hal_Debug(PRINTF, "*********************************************\r\n");
			
			vTaskDelay(1000);
		}
	}
#endif /* #ifdef USE_FreeRTOS_CPU_UsageRate */



