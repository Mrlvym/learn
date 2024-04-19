#include "freertos_event.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "hal_debug_usart.h"
#include "hal_led.h"
#include "hal_key.h"

static EventGroupHandle_t FreeRTOS_Event_Handle = NULL;

TaskHandle_t Event_Set_Task_Handle = NULL;
TaskHandle_t Event_Get_Task_Handle = NULL;

static void Event_Set_Task(void* pvParameters);
static void Event_Get_Task(void* pvParameters);

void FreeRTOS_Event_Bsp_Init(void)
{
	Hal_Led_Init();

	Hal_Key_Init();
}


void FreeRTOS_Event_Task(void)
{
	BaseType_t xReturn = pdPASS;

	FreeRTOS_Event_Init();

	xReturn = xTaskCreate(	(TaskFunction_t) Event_Set_Task,
							(const char * )"Event_Set_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )2,
							(TaskHandle_t * )&Event_Set_Task_Handle);
	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Event_Set_Task Creat success ! \r\n");

	xReturn = xTaskCreate(	(TaskFunction_t) Event_Get_Task,
							(const char * )"Event_Get_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )2,
							(TaskHandle_t * )&Event_Get_Task_Handle);
	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Event_Get_Task Creat success ! \r\n");

}


void FreeRTOS_Event_Init(void)
{
	FreeRTOS_Event_Handle = xEventGroupCreate();

	if(NULL != FreeRTOS_Event_Handle){
		Hal_Debug(INFO_LEVEL, "Creat Event Group success ... \r\n");
	}
}



static void Event_Set_Task(void* pvParameters)
{
	while(1)
	{
		if(!Hal_Key0_Read_Bit()){
			vTaskDelay(10);
			if(!Hal_Key0_Read_Bit()){			
//				Hal_Debug(INFO_LEVEL, "Key 0 Enter ... \r\n");
				xEventGroupSetBits((EventGroupHandle_t)FreeRTOS_Event_Handle,
								   (EventBits_t)KEY0_EVENT_BIT);
			}			
		}
		
		if(!Hal_Key2_Read_Bit()){
			vTaskDelay(10);
			if(!Hal_Key2_Read_Bit()){
//				Hal_Debug(INFO_LEVEL, "Key 2 Enter ... \r\n");
				xEventGroupSetBits((EventGroupHandle_t)FreeRTOS_Event_Handle,
								   (EventBits_t)KEY2_EVENT_BIT);
			}
		}

		vTaskDelay(10);
	}
}

static void Event_Get_Task(void* pvParameters)
{
	EventBits_t r_event;

	while(1)
	{
	#if (defined(FREERTOS_EVENT_ALLBIT))
		r_event = xEventGroupWaitBits((EventGroupHandle_t)FreeRTOS_Event_Handle,
									   (EventBits_t)(KEY0_EVENT_BIT | KEY2_EVENT_BIT),
									   pdTRUE,
									   pdTRUE,
									   portMAX_DELAY);	
		if((KEY0_EVENT_BIT | KEY2_EVENT_BIT) == (r_event & (KEY0_EVENT_BIT | KEY2_EVENT_BIT))){
			Hal_Debug(INFO_LEVEL, "Key 0 & Key 2 Event is Tig ... \r\n");			
		}
	#else
		r_event = xEventGroupWaitBits((EventGroupHandle_t)FreeRTOS_Event_Handle,
									   (EventBits_t)(KEY0_EVENT_BIT | KEY2_EVENT_BIT),
									   pdTRUE,
									   pdFALSE,
									   portMAX_DELAY);	
		if((KEY0_EVENT_BIT == (r_event & KEY0_EVENT_BIT)) || (KEY2_EVENT_BIT == (r_event & KEY2_EVENT_BIT))){
			Hal_Debug(INFO_LEVEL, "Key 0 || Key 2 Event is Tig ... \r\n");
		
			if((KEY0_EVENT_BIT == (r_event & KEY0_EVENT_BIT))){
				Hal_Debug(INFO_LEVEL, "Key 0 Event is Tig ... \r\n");								
			}
			
			if((KEY2_EVENT_BIT == (r_event & KEY2_EVENT_BIT))){
				Hal_Debug(INFO_LEVEL, "Key 2 Event is Tig ... \r\n");								
			}
		}		
	#endif
	}
}


