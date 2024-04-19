#include "freertos_semaphore.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "hal_debug_usart.h"
#include "hal_key.h"
#include "hal_led.h"

#ifdef FREERTOS_TEST_SEMAPHORE_BINARY
static SemaphoreHandle_t Test_SemaphoreBinary_Handle = NULL;
#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_BINARY */

#ifdef FREERTOS_TEST_SEMAPHORE_COUNT
static SemaphoreHandle_t Test_SemaphoreCount_Handle = NULL;
#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_COUNT */

#if !(defined (FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE) && defined (FREERTOS_TEST_SEMAPHORE_COUNT))

#if (defined(FREERTOS_TEST_SEMAPHORE_BINARY) || defined(FREERTOS_TEST_SEMAPHORE_COUNT))
static TaskHandle_t Semaphore_Take_Task_Handle = NULL;
static void Semaphore_Take_Task(void* pvParameters);
#endif /* defined(FREERTOS_TEST_SEMAPHORE_COUNT) || (define(FREERTOS_TEST_SEMAPHORE_BINARY)) */

#ifndef FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE
static TaskHandle_t Semaphore_Give_Task_Handle = NULL;
static void Semaphore_Give_Task(void* pvParameters);
#endif /* #define FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE */

#endif /* #if !(defined (FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE) && defined (FREERTOS_TEST_SEMAPHORE_COUNT)) */

void FreeRTOS_Semaphore_Bsp_Init(void)
{
#ifdef FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE
	Hal_Key_Exit_Init();
#else
	Hal_Key_Init();
#endif /* #ifdef FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE */

	Hal_Led_Init();
}


void FreeRTOS_Semaphore_Task(void)
{
	BaseType_t xReturn = pdPASS;
	
	FreeRTOS_Semaphore_Init();

#if !(defined (FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE) && defined (FREERTOS_TEST_SEMAPHORE_COUNT))

#if (defined(FREERTOS_TEST_SEMAPHORE_BINARY) || defined(FREERTOS_TEST_SEMAPHORE_COUNT))
	xReturn = xTaskCreate(	(TaskFunction_t) Semaphore_Take_Task,
							(const char * )"Semaphore_Take_Task",
							(uint16_t) 512,
							(void * )NULL,
							(UBaseType_t )1,
							(TaskHandle_t * )&Semaphore_Take_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Semaphore_Take_Task Creat success ! \r\n");
#endif /* defined(FREERTOS_TEST_SEMAPHORE_COUNT) || (define(FREERTOS_TEST_SEMAPHORE_BINARY)) */

#ifndef FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE
	xReturn = xTaskCreate(	(TaskFunction_t) Semaphore_Give_Task,
							(const char * )"Semaphore_Give_Task",
							(uint16_t) 512,
							(void * )NULL,
							(UBaseType_t )1,
							(TaskHandle_t * )&Semaphore_Give_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Semaphore_Give_Task Creat success ! \r\n");

#endif /* #define FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE */

#endif /* #if !(defined (FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE) && defined (FREERTOS_TEST_SEMAPHORE_COUNT)) */
}

void FreeRTOS_Semaphore_Init(void)
{

#ifdef FREERTOS_TEST_SEMAPHORE_BINARY
	BaseType_t xReturn = pdFALSE; 

	Test_SemaphoreBinary_Handle = xSemaphoreCreateBinary();

	if(NULL != Test_SemaphoreBinary_Handle){
		xReturn = xSemaphoreGive(Test_SemaphoreBinary_Handle);

		if(pdFALSE != xReturn){
			Hal_Debug(INFO_LEVEL, "Creat Test_SemaphoreBinary success ! \r\n");
		}
	}
#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_BINARY */

#ifdef FREERTOS_TEST_SEMAPHORE_COUNT
	Test_SemaphoreCount_Handle = xSemaphoreCreateCounting( (UBaseType_t) FREERTOS_SEMAPHORE_COUNT_MAX, 
														   (UBaseType_t) FREERTOS_SEMAPHORE_COUNT_INIT);

	if(NULL != Test_SemaphoreCount_Handle){
		Hal_Debug(INFO_LEVEL, "Creat Test_SemaphoreCount success ! \r\n");
	}
#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_COUNT */

}

#if !(defined (FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE) && defined (FREERTOS_TEST_SEMAPHORE_COUNT))

#if (defined(FREERTOS_TEST_SEMAPHORE_BINARY) || defined(FREERTOS_TEST_SEMAPHORE_COUNT))
static void Semaphore_Take_Task(void* pvParameters)
{
	BaseType_t xReturn = pdFALSE;

	while(1)
	{
	#ifdef FREERTOS_TEST_SEMAPHORE_BINARY
		xReturn = xSemaphoreTake( (QueueHandle_t)Test_SemaphoreBinary_Handle, 
								  (TickType_t)portMAX_DELAY);
		if(pdFALSE != xReturn){
			Hal_Debug(INFO_LEVEL, "Test_SemaphoreBinary Take success !\r\n");
		}
	#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_BINARY */

	#ifdef FREERTOS_TEST_SEMAPHORE_COUNT
		if(!Hal_Key2_Read_Bit())
		{	
			vTaskDelay(50);
			if(!Hal_Key2_Read_Bit())
			{
				xReturn = xSemaphoreTake( (QueueHandle_t)Test_SemaphoreCount_Handle,
										  (TickType_t)0);
				if(pdFALSE != xReturn){
					Hal_Debug(INFO_LEVEL, "Take 1 !\r\n");
				}
				else{				
					Hal_Debug(INFO_LEVEL, "No num to take !\r\n");
				}
			}
		}	

	#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_COUNT */
	}
}
#endif /* defined(FREERTOS_TEST_SEMAPHORE_COUNT) || (define(FREERTOS_TEST_SEMAPHORE_BINARY)) */

#ifndef FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE
static void Semaphore_Give_Task(void* pvParameters)
{
	BaseType_t xReturn = pdFALSE;

	while(1)
	{
	#ifdef FREERTOS_TEST_SEMAPHORE_BINARY
		if(!Hal_Key0_Read_Bit())
		{	
			vTaskDelay(50);
			if(!Hal_Key0_Read_Bit())
			{
				xReturn = xSemaphoreGive( (QueueHandle_t)Test_SemaphoreBinary_Handle);
				if(pdFALSE != xReturn){
//					Hal_Debug(INFO_LEVEL, "Test_SemaphoreBinary Give success !\r\n");
				}
			}
		}	
	#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_BINARY */

	#ifdef FREERTOS_TEST_SEMAPHORE_COUNT
		if(!Hal_Key0_Read_Bit())
		{	
			vTaskDelay(50);
			if(!Hal_Key0_Read_Bit())
			{
				xReturn = xSemaphoreGive( (QueueHandle_t)Test_SemaphoreCount_Handle);
				if(pdFALSE != xReturn){
					Hal_Debug(INFO_LEVEL, "Give 1 !\r\n");
				}
				else{
					Hal_Debug(INFO_LEVEL, "No num to give !\r\n");
				}
			}
		}
		
	#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_COUNT */

		vTaskDelay(20);
	}
}
#endif /* #define FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE */

#endif /* #if !(defined (FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE) && defined (FREERTOS_TEST_SEMAPHORE_COUNT)) */






#ifdef FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE

#if defined(FREERTOS_TEST_SEMAPHORE_BINARY) || defined(FREERTOS_TEST_SEMAPHORE_COUNT)
void FreeRTOS_Key0_Semaphore_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken = pdFAIL;
	BaseType_t xReturn  = pdFAIL;
	
	if(EXTI_GetFlagStatus(EXTI_Line4) != RESET)
	{
		if(!Hal_Key0_Read_Bit()){
		#ifdef FREERTOS_TEST_SEMAPHORE_BINARY
			xReturn = xSemaphoreGiveFromISR((QueueHandle_t)Test_SemaphoreBinary_Handle, 
								  			(BaseType_t *) &pxHigherPriorityTaskWoken);
		#endif
		#ifdef FREERTOS_TEST_SEMAPHORE_COUNT
			xReturn = xSemaphoreGiveFromISR((QueueHandle_t)Test_SemaphoreCount_Handle, 
								  			(BaseType_t *) &pxHigherPriorityTaskWoken);
			if(pdPASS == xReturn){
				printf("G 1\r\n");
			}
			else{
				printf("No to G\r\n");
			}
		#endif
			if(pdPASS == pxHigherPriorityTaskWoken){
				portYIELD();
			}
		}
	}
	
	EXTI_ClearITPendingBit(EXTI_Line4); 

}
#endif /* #if defined(FREERTOS_TEST_SEMAPHORE_BINARY) || defined(FREERTOS_TEST_SEMAPHORE_COUNT) */

#ifdef FREERTOS_TEST_SEMAPHORE_COUNT
void FreeRTOS_Key2_Semaphore_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken = pdFAIL;
	BaseType_t xReturn	= pdFAIL;

	if(EXTI_GetFlagStatus(EXTI_Line2) != RESET)
	{
		if(!Hal_Key2_Read_Bit())
		{
			xReturn = xSemaphoreTakeFromISR((QueueHandle_t)Test_SemaphoreCount_Handle, 
								  			(BaseType_t *) &pxHigherPriorityTaskWoken);

			if(pdPASS == xReturn){
				printf("T 1\r\n");
			}
			else{
				printf("No to T\r\n");
			}
		}

		if(pdPASS == pxHigherPriorityTaskWoken){
			portYIELD();
		}
	}
	
	EXTI_ClearITPendingBit(EXTI_Line2); 
}
#endif /* #ifdef FREERTOS_TEST_SEMAPHORE_COUNT */

#endif /* #ifdef FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE */

