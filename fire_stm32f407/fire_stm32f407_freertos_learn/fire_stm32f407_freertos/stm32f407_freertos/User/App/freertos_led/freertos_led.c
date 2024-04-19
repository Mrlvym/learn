#include "freertos_led.h"
#include "hal_led.h"
#include "hal_debug_usart.h"
#include "FreeRTOS.h"
#include "task.h"


/* LED1  任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;

/* LED2  任务句柄 */
static TaskHandle_t LED2_Task_Handle = NULL;


static void LED1_Task(void* pvParameters);/* LED1_Task  任务实现 */
static void LED2_Task(void* pvParameters);/* LED2_Task  任务实现 */

void FreeRTOS_Led_Init(void)
{
	Hal_Led_Init();
}

void FreeRTOS_Led_Task(void)
{

	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(	(TaskFunction_t) LED1_Task,
							(const char * )"LED1_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )2,
							(TaskHandle_t * )&LED1_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "LED1_Task Creat success ! \r\n");

	xReturn = xTaskCreate(	(TaskFunction_t) LED2_Task,
							(const char * )"LED2_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )3,
							(TaskHandle_t * )&LED2_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "LED2_Task Creat success ! \r\n");
}

static void LED1_Task(void* pvParameters)
{
	while(1)
	{
		Hal_Led0_on();
		vTaskDelay(500);
		
		Hal_Led0_off();
		vTaskDelay(500);
	}
}

static void LED2_Task(void* pvParameters)
{
	while(1)
	{
		Hal_Led1_on();
		vTaskDelay(500);
		
		Hal_Led1_off();
		vTaskDelay(500);		
	}
}




