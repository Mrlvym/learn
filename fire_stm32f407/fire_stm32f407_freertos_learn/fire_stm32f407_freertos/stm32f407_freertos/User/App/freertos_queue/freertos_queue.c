#include "freertos_queue.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "hal_debug_usart.h"
#include "hal_key.h"
#include "hal_led.h"


static QueueHandle_t Queue_Key0_Handle = NULL;
static QueueHandle_t Queue_Key2_Handle = NULL;

static uint8_t Key0_Queue_Send_Val = 0;
static uint8_t Key2_Queue_Send_Val = 0;

static uint8_t Key_Queue_Rec_Val = 0;

#ifndef FREERTOS_USED_KEY_EXIT_IRQ_QUEUE
static TaskHandle_t Queue_Send_Task_Handle = NULL;
static void Queue_Send_Task(void* pvParameters);
#endif

static TaskHandle_t Queue_Rec_Task_Handle = NULL;
static void Queue_Rec_Task(void* pvParameters);

static TaskHandle_t Queue_Led0_Task_Handle = NULL;
static void Queue_Led0_Task(void* pvParameters);



void FreeRTOS_Queue_Bsp_Init(void)
{
#ifdef FREERTOS_USED_KEY_EXIT_IRQ_QUEUE
	Hal_Key_Exit_Init();
#else
	Hal_Key_Init();
#endif

	Hal_Led_Init();
}

void FreeRTOS_Queue_Task(void)
{
	
	BaseType_t xReturn = pdPASS;

	FreeRTOS_Queue_Init();

	xReturn = xTaskCreate(	(TaskFunction_t) Queue_Rec_Task,
							(const char * )"Queue_Rec_Task",
							(uint16_t) 512,
							(void * )NULL,
							(UBaseType_t )1,
							(TaskHandle_t * )&Queue_Rec_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Queue_Rec_Task Creat success ! \r\n");

#ifndef FREERTOS_USED_KEY_EXIT_IRQ_QUEUE

	xReturn = xTaskCreate(	(TaskFunction_t) Queue_Send_Task,
							(const char * )"Queue_Send_Task",
							(uint16_t) 512,
							(void * )NULL,
							(UBaseType_t )3,
							(TaskHandle_t * )&Queue_Send_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Queue_Send_Task Creat success ! \r\n");

#endif
	
	xReturn = xTaskCreate(	(TaskFunction_t) Queue_Led0_Task,
							(const char * )"Queue_Led0_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )1,
							(TaskHandle_t * )&Queue_Led0_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Queue_Led0_Task Creat success ! \r\n");
	
}

void FreeRTOS_Queue_Init(void)
{
	Queue_Key0_Handle = xQueueCreate( (UBaseType_t) QUEUE_KEY0_LEN, 
									  (UBaseType_t) QUEUE_KEY0_SIZE);
	if(NULL != Queue_Key0_Handle)
		Hal_Debug(INFO_LEVEL, "Creat Queue_Key0 success ! \r\n");

	Queue_Key2_Handle = xQueueCreate( (UBaseType_t) QUEUE_KEY2_LEN, 
									  (UBaseType_t) QUEUE_KEY2_SIZE);
	if(NULL != Queue_Key2_Handle)
		Hal_Debug(INFO_LEVEL, "Creat Queue_Key2 success ! \r\n");

}

#ifndef FREERTOS_USED_KEY_EXIT_IRQ_QUEUE

static void Queue_Send_Task(void* pvParameters)
{
	BaseType_t xReturn = pdPASS;
	
	while(1)
	{
		if(!Hal_Key0_Read_Bit())
		{	
			vTaskDelay(50);
			if(!Hal_Key0_Read_Bit())
			{
				Key0_Queue_Send_Val = 'A';
				xReturn = xQueueSend( (QueueHandle_t) Queue_Key0_Handle, 
									  (void *) &Key0_Queue_Send_Val, 
									  (TickType_t) 0);
				if(pdPASS == xReturn)
					Hal_Debug(INFO_LEVEL, "Key0 send queue msg success ! \r\n");
			}
		}
		if(!Hal_Key2_Read_Bit())
		{
			vTaskDelay(50);
			if(!Hal_Key2_Read_Bit())
			{			
				Key2_Queue_Send_Val = 'B';
				xReturn = xQueueSend( (QueueHandle_t) Queue_Key0_Handle, 
									  (void *) &Key2_Queue_Send_Val, 
									  (TickType_t) 0);
				if(pdPASS == xReturn)
					Hal_Debug(INFO_LEVEL, "Key2 send queue msg success ! \r\n");
								
			}
		}

		vTaskDelay(20);
	}
}

#endif

static void Queue_Rec_Task(void* pvParameters)
{
	BaseType_t xReturn = pdPASS;
	
	while(1)
	{
//		vTaskDelay(10);
		xReturn = xQueueReceive( (QueueHandle_t) Queue_Key0_Handle, 
								 (void *) &Key_Queue_Rec_Val, 
								 (TickType_t) portMAX_DELAY);
		if(pdPASS == xReturn){
			Hal_Debug(INFO_LEVEL, "Queue rec val %c \r\n", Key_Queue_Rec_Val);
		}else{
			Hal_Debug(INFO_LEVEL, "Queue rec error 0x%lx \r\n", xReturn);
		}

//		xReturn = xQueueReceive( (QueueHandle_t) Queue_Key2_Handle, 
//								 (void *) &Key_Queue_Rec_Val, 
//								 (TickType_t) 0);
//		if(pdPASS == xReturn){
//			Hal_Debug(INFO_LEVEL, "Queue rec val %c \r\n", Key_Queue_Rec_Val);
//		}else{
//			Hal_Debug(INFO_LEVEL, "Queue rec error 0x%lx \r\n", xReturn);
//		}
	}
}

static void Queue_Led0_Task(void* pvParameters)
{
	while(1){
		Hal_Led0_on();
		vTaskDelay(500);
	
		Hal_Led0_off();
		vTaskDelay(500);
	}
}


#ifdef FREERTOS_USED_KEY_EXIT_IRQ_QUEUE
void FreeRTOS_Key0_SendQueue_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken  = pdFAIL;

	Key0_Queue_Send_Val = 'C';
	
	if(EXTI_GetFlagStatus(EXTI_Line4) != RESET)
	{
		xQueueSendFromISR( (QueueHandle_t) Queue_Key0_Handle, 
						   (void *) &Key0_Queue_Send_Val, 
						   (BaseType_t *) &pxHigherPriorityTaskWoken);

		if(pdPASS == pxHigherPriorityTaskWoken){
			portYIELD();
		}
	}
	
	EXTI_ClearITPendingBit(EXTI_Line4); 
}

void FreeRTOS_Key2_SendQueue_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken  = pdFAIL;

	Key2_Queue_Send_Val = 'D';
	
	if(EXTI_GetFlagStatus(EXTI_Line2) != RESET)
	{
		xQueueSendFromISR( (QueueHandle_t) Queue_Key0_Handle, 
						   (void *) &Key2_Queue_Send_Val, 
						   (BaseType_t *) &pxHigherPriorityTaskWoken);

		if(pdPASS == pxHigherPriorityTaskWoken){
			portYIELD();
		}
	}
	
	EXTI_ClearITPendingBit(EXTI_Line2); 
}
#endif


