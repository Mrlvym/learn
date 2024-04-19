#include "freertos_notify.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hal_debug_usart.h"
#include "hal_led.h"
#include "hal_key.h"


#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE)
static TaskHandle_t Notify_Replace_Queue_Rec1_Handle = NULL;
static TaskHandle_t Notify_Replace_Queue_Rec2_Handle = NULL;
static TaskHandle_t Notify_Replace_Queue_Send_Handle = NULL;

static void Notify_Replace_Queue_Rec1_Task(void* pvParameters);
static void Notify_Replace_Queue_Rec2_Task(void* pvParameters);
static void Notify_Replace_Queue_Send_Task(void* pvParameters);
#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMBIN)
static TaskHandle_t Notify_Replace_SemBin_Take1_Handle = NULL;
static TaskHandle_t Notify_Replace_SemBin_Take2_Handle = NULL;
static TaskHandle_t Notify_Replace_SemBin_Give_Handle = NULL;

static void Notify_Replace_SemBin_Take1_Task(void* pvParameters);
static void Notify_Replace_SemBin_Take2_Task(void* pvParameters);
static void Notify_Replace_SemBin_Give_Task(void* pvParameters);
#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMBIN) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMCOUNT)
static TaskHandle_t Notify_Replace_SemCount_Take_Handle = NULL;
static TaskHandle_t Notify_Replace_SemCount_Give_Handle = NULL;

static void Notify_Replace_SemCount_Take_Task(void* pvParameters);
static void Notify_Replace_SemCount_Give_Task(void* pvParameters);
#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMCOUNT) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_EVENT)
static TaskHandle_t Notify_Replace_Event_Rec_Handle = NULL;
static TaskHandle_t Notify_Replace_Event_Send_Handle = NULL;

static void Notify_Replace_Event_Rec_Task(void* pvParameters);
static void Notify_Replace_Event_Send_Task(void* pvParameters);
#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_EVENT) */



void FreeRTOS_Notify_Bsp_Init(void)
{
	Hal_Led_Init();

	Hal_Key_Init();
}

void FreeRTOS_Notify_Task(void)
{
	BaseType_t xReturn = pdPASS;
	
#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE)

	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_Queue_Rec1_Task,
							(const char * ) "Notify_Replace_Queue_Rec1_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_Queue_Rec1_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_Queue_Rec1_Task Creat success ! \r\n");
	}

	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_Queue_Rec2_Task,
							(const char * ) "Notify_Replace_Queue_Rec2_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_Queue_Rec2_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_Queue_Rec2_Task Creat success ! \r\n");
	}

	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_Queue_Send_Task,
							(const char * ) "Notify_Replace_Queue_Send_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_Queue_Send_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_Queue_Send_Task Creat success ! \r\n");
	}

#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMBIN)
	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_SemBin_Take1_Task,
							(const char * ) "Notify_Replace_SemBin_Take1_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_SemBin_Take1_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_SemBin_Take1_Task Creat success ! \r\n");
	}

	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_SemBin_Take2_Task,
							(const char * ) "Notify_Replace_SemBin_Take2_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_SemBin_Take2_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_SemBin_Take2_Task Creat success ! \r\n");
	}

	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_SemBin_Give_Task,
							(const char * ) "Notify_Replace_SemBin_Give_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_SemBin_Give_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_SemBin_Give_Task Creat success ! \r\n");
	}
#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMBIN) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMCOUNT)
	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_SemCount_Take_Task,
							(const char * ) "Notify_Replace_SemCount_Take_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_SemCount_Take_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_SemCount_Take_Task Creat success ! \r\n");
	}

	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_SemCount_Give_Task,
							(const char * ) "Notify_Replace_SemCount_Give_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_SemCount_Give_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_SemCount_Give_Task Creat success ! \r\n");
	}

#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMCOUNT) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_EVENT)
	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_Event_Rec_Task,
							(const char * ) "Notify_Replace_Event_Rec_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_Event_Rec_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_Event_Rec_Task Creat success ! \r\n");
	}
	
	xReturn = xTaskCreate(	(TaskFunction_t) Notify_Replace_Event_Send_Task,
							(const char * ) "Notify_Replace_Event_Send_Task",
							(uint16_t) 128,
							(void * ) NULL,
							(UBaseType_t ) 2,
							(TaskHandle_t * )&Notify_Replace_Event_Send_Handle);
	if(pdPASS == xReturn){
		Hal_Debug(INFO_LEVEL, "Notify_Replace_Event_Send_Task Creat success ! \r\n");
	}

#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_EVENT) */

}


#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE)

static void Notify_Replace_Queue_Rec1_Task(void* pvParameters)
{
	BaseType_t xReturn = pdPASS;

#if (defined(TEST_QUEUE_USE_CHAR))
	uint8_t *char_rec1 = NULL;
#else
	uint32_t rec1 = 0;
#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */
	
	while(1)
	{
		xReturn = xTaskNotifyWait((uint32_t) 0,
								  (uint32_t) 0xFFFFFFFF,
							#if (defined(TEST_QUEUE_USE_CHAR))
								   (uint32_t *) &char_rec1,
							#else
								   (uint32_t *) &rec1,
							#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */
								   (uint32_t) portMAX_DELAY);
		if(pdPASS == xReturn){
		#if (defined(TEST_QUEUE_USE_CHAR))
			Hal_Debug(PRINTF, "Notify_Replace_Queue_Rec1_Task Notify %s \r\n", char_rec1);
		#else
			Hal_Debug(PRINTF, "Notify_Replace_Queue_Rec1_Task Notify %d \r\n", rec1);
		#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */
		}
	}
}

static void Notify_Replace_Queue_Rec2_Task(void* pvParameters)
{
	BaseType_t xReturn = pdPASS;

#if (defined(TEST_QUEUE_USE_CHAR))
	uint8_t *char_rec2 = NULL;
#else
	uint32_t rec2 = 0;
#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */
	
	while(1)
	{
		xReturn = xTaskNotifyWait((uint32_t) 0,
								  (uint32_t) 0xFFFFFFFF,
							#if (defined(TEST_QUEUE_USE_CHAR))
								   (uint32_t *) &char_rec2,
							#else
								   (uint32_t *) &rec2,
							#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */
								   (uint32_t) portMAX_DELAY);
		if(pdPASS == xReturn){
		#if (defined(TEST_QUEUE_USE_CHAR))
			Hal_Debug(PRINTF, "Notify_Replace_Queue_Rec2_Task Notify %s \r\n", char_rec2);
		#else
			Hal_Debug(PRINTF, "Notify_Replace_Queue_Rec2_Task Notify %d \r\n", rec2);
		#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */
		}
	}
}

static void Notify_Replace_Queue_Send_Task(void* pvParameters)
{
	BaseType_t xReturn = pdPASS;

#if (defined(TEST_QUEUE_USE_CHAR))
	static uint8_t char_test1[] = "key 0 test ...";
	static uint8_t char_test2[] = "key 2 test ...";
#else
	static uint32_t send_val1 = 10;
	static uint32_t send_val2 = 20;
#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */

	while(1)
	{
		if(!Hal_Key0_Read_Bit()){		
			vTaskDelay(10);
			if(!Hal_Key0_Read_Bit()){
				xReturn = xTaskNotify((TaskHandle_t) Notify_Replace_Queue_Rec1_Handle, 
						#if (defined(TEST_QUEUE_USE_CHAR))
							(uint32_t) &char_test1, 
						#else
							(uint32_t) send_val1,
						#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */
							(eNotifyAction) eSetValueWithOverwrite);
				if(pdPASS == xReturn){
//					Hal_Debug(INFO_LEVEL, "Notify_Replace_Queue Key 0 Send success ! \r\n");					
				}
			}
		}

		if(!Hal_Key2_Read_Bit()){		
			vTaskDelay(10);
			if(!Hal_Key2_Read_Bit()){
				xReturn = xTaskNotify((TaskHandle_t) Notify_Replace_Queue_Rec2_Handle, 
						#if (defined(TEST_QUEUE_USE_CHAR))
							(uint32_t) &char_test2, 
						#else
							(uint32_t) send_val2,
						#endif /* #if (defined(TEST_QUEUE_USE_CHAR)) */
							(eNotifyAction) eSetValueWithOverwrite);
				if(pdPASS == xReturn){
//					Hal_Debug(INFO_LEVEL, "Notify_Replace_Queue Key 2 Send success ! \r\n");					
				}
			}
		}
	}
}

#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMBIN)
static void Notify_Replace_SemBin_Take1_Task(void* pvParameters)
{
	uint32_t r_Return = 0;

	while(1)
	{
		r_Return = ulTaskNotifyTake((BaseType_t) pdTRUE, (TickType_t) portMAX_DELAY);

		Hal_Debug(PRINTF, "Notify_Replace_SemBin_Take1_Task Rec success %d ... \r\n", r_Return);
	}
}

static void Notify_Replace_SemBin_Take2_Task(void* pvParameters)
{
	uint32_t r_Return = 0;

	while(1)
	{
		r_Return = ulTaskNotifyTake((BaseType_t) pdTRUE, (TickType_t) portMAX_DELAY);

		Hal_Debug(PRINTF, "Notify_Replace_SemBin_Take2_Task Rec success %d ... \r\n", r_Return);
	}
}

static void Notify_Replace_SemBin_Give_Task(void* pvParameters)
{
	while(1)
	{
		if(!Hal_Key0_Read_Bit()){		
			vTaskDelay(10);
			if(!Hal_Key0_Read_Bit()){
				xTaskNotifyGive((TaskHandle_t) Notify_Replace_SemBin_Take1_Handle);
			}
		}

		if(!Hal_Key2_Read_Bit()){		
			vTaskDelay(10);
			if(!Hal_Key2_Read_Bit()){
				xTaskNotifyGive((TaskHandle_t) Notify_Replace_SemBin_Take2_Handle);
			}
		}
	}
}
#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMBIN) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMCOUNT)
static void Notify_Replace_SemCount_Take_Task(void* pvParameters)
{
	uint32_t t_Return = 0;
	
	while(1)
	{
		if(!Hal_Key0_Read_Bit()){		
			vTaskDelay(10);
			if(!Hal_Key0_Read_Bit()){			
				t_Return = ulTaskNotifyTake((BaseType_t) pdFALSE, (TickType_t) 0);

				if(t_Return > 0){
					Hal_Debug(PRINTF, "Key 0 Enter T Success , Num : %d \r\n", t_Return-1);
				}
				else{
					Hal_Debug(PRINTF, "Key 0 Enter T Falied , Num : %d \r\n", t_Return);
				}
			}
		}
	}
}
static void Notify_Replace_SemCount_Give_Task(void* pvParameters)
{
	BaseType_t g_Return = pdFAIL;
	
	while(1)
	{
		if(!Hal_Key2_Read_Bit()){		
			vTaskDelay(10);
			if(!Hal_Key2_Read_Bit()){
				g_Return = xTaskNotifyGive((TaskHandle_t) Notify_Replace_SemCount_Take_Handle);

				if(pdPASS == g_Return){
					Hal_Debug(PRINTF, "Key 2 Enter G Success ... \r\n");
				}
				else{
					Hal_Debug(PRINTF, "Key 2 Enter G Falied ... \r\n");					
				}
			}
		}
	}
}
#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_SEMCOUNT) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_EVENT)
static void Notify_Replace_Event_Rec_Task(void* pvParameters)
{
	BaseType_t xReturn = pdFAIL;
	uint32_t r_event = 0;
#if (defined(EVENT_INT_ALL))
	uint32_t last_event = 0;
#endif /* #if (defiend(EVENT_INT_ALL)) */
	
	while(1)
	{
		xReturn = xTaskNotifyWait((uint32_t) 0x00, 
								  (uint32_t) 0xFFFFFFFF, 
								  (uint32_t *) &r_event, 
								  (TickType_t) portMAX_DELAY);
		if(pdPASS == xReturn){
		#if (defined(EVENT_INT_ALL))
			last_event |= r_event;
			if((last_event & (EVENT_KEY0 | EVENT_KEY2)) == (EVENT_KEY0 | EVENT_KEY2)){
				last_event = 0;
				Hal_Debug(PRINTF, "Key 0 && Key 2 Event is Trig ... \r\n");
			}
		#else
			if(((r_event & EVENT_KEY0) == EVENT_KEY0) || ((r_event & EVENT_KEY2) == EVENT_KEY2)){
				Hal_Debug(PRINTF, "Key 0 || Key 2 Event is Trig ... \r\n");
				
				if(((r_event & EVENT_KEY0) == EVENT_KEY0)){			
					Hal_Debug(PRINTF, "Key 0 Event is Trig ... \r\n");
				}
				
				if(((r_event & EVENT_KEY2) == EVENT_KEY2)){			
					Hal_Debug(PRINTF, "Key 2 Event is Trig ... \r\n");
				}
			}
		#endif /* #if (defiend(EVENT_INT_ALL)) */
		}
	#if (defined(EVENT_INT_ALL))
		else{
			last_event = r_event;
		}
	#endif /* #if (defiend(EVENT_INT_ALL)) */
	}
}

static void Notify_Replace_Event_Send_Task(void* pvParameters)
{
	BaseType_t xReturn = pdFAIL;

	while(1)
	{
		if(!Hal_Key0_Read_Bit()){		
			vTaskDelay(10);
			if(!Hal_Key0_Read_Bit()){
				xReturn = xTaskNotify((TaskHandle_t) Notify_Replace_Event_Rec_Handle, 
									  (uint32_t) EVENT_KEY0, 
									  (eNotifyAction) eSetBits);

				if(pdPASS == xReturn){
//					Hal_Debug(PRINTF, "Key 0 Enter , Send  EVENT_KEY0 ... \r\n");
				}				
			}
		}

		if(!Hal_Key2_Read_Bit()){		
			vTaskDelay(10);
			if(!Hal_Key2_Read_Bit()){
				xReturn = xTaskNotify((TaskHandle_t) Notify_Replace_Event_Rec_Handle, 
									  (uint32_t) EVENT_KEY2, 
									  (eNotifyAction) eSetBits);

				if(pdPASS == xReturn){
//					Hal_Debug(PRINTF, "Key 2 Enter , Send  EVENT_KEY2 ... \r\n");
				}
			}
		}
	}
}
#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_EVENT) */



