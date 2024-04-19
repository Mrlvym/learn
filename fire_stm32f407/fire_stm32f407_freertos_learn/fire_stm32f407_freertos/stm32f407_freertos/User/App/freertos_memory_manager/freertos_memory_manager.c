#include "freertos_memory_manager.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hal_debug_usart.h"
#include "hal_led.h"
#include "hal_key.h"


uint8_t * Test_Ptr = NULL;

static TaskHandle_t Memory_LED_Task_Handle = NULL;
static TaskHandle_t Memory_KEY_Task_Handle = NULL;

static void Memory_LED_Task( void * arg);
static void Memory_KEY_Task( void * arg);


void FreeRTOS_Memory_Manger_Bsp_Init(void)
{
	Hal_Led_Init();

	Hal_Key_Init();
}



void FreeRTOS_Memory_Manger_Task(void)
{
	BaseType_t xReturn = pdFAIL;

	 xReturn = xTaskCreate(	(TaskFunction_t) Memory_LED_Task,
							(const char *) "Memory_LED_Task",
							(uint16_t) 128,
							(void *) NULL,
							(UBaseType_t) 2,
							(TaskHandle_t *) &Memory_LED_Task_Handle );

	if(xReturn != pdFAIL){
		Hal_Debug(INFO_LEVEL, "Memory_LED_Task Creat success ... \r\n");
	}

	 xReturn = xTaskCreate(	(TaskFunction_t) Memory_KEY_Task,
							(const char *) "Memory_KEY_Task",
							(uint16_t) 128,
							(void *) NULL,
							(UBaseType_t) 2,
							(TaskHandle_t *) &Memory_KEY_Task_Handle );

	if(xReturn != pdFAIL){
		Hal_Debug(INFO_LEVEL, "Memory_KEY_Task Creat success ... \r\n");
	}
}


static void Memory_LED_Task( void * arg)
{
	while(1)
	{
		Hal_Led_turn(LED_ALL);
		
		vTaskDelay(1000);
	}
}
static void Memory_KEY_Task( void * arg)
{
	uint32_t g_memsize = 0;
	
	while(1)
	{
		if(Hal_Key_Scan(HAL_KEY0, HAL_KEY_ONCE)){
			if(NULL == Test_Ptr){
				g_memsize = xPortGetFreeHeapSize();
				Hal_Debug(PRINTF, "Sys Cur Memsize is %d \r\n", g_memsize);

				Test_Ptr = pvPortMalloc(1024);
				if(NULL != Test_Ptr){
					Hal_Debug(PRINTF, "Malloc mem success \r\n");
					Hal_Debug(PRINTF, "Malloc mem address is %#x \r\n", (int)Test_Ptr);
				
					g_memsize = xPortGetFreeHeapSize();
					Hal_Debug(PRINTF, "Sys Cur SY Memsize is %d \r\n", g_memsize);

					sprintf((char *)Test_Ptr, "Cur TickCount %d \r\n", xTaskGetTickCount());
				}
			}
			else{
				Hal_Debug(PRINTF, "Enter Key 2 To Free Memory ... \r\n");
			}
		}

		if(Hal_Key_Scan(HAL_KEY2, HAL_KEY_ONCE)){
			if(NULL != Test_Ptr){
				Hal_Debug(PRINTF, "Read : %s", (char *)Test_Ptr);
				Hal_Debug(PRINTF, "Free Mem ... \r\n");
			
				vPortFree((void *) Test_Ptr);
				Test_Ptr = NULL;
				
				g_memsize = xPortGetFreeHeapSize();
				Hal_Debug(PRINTF, "Sys Cur Memsize is %d , Mem Free success \r\n", g_memsize);
			}
			else{
				Hal_Debug(PRINTF, "Enter Key 0 To Malloc Memory ... \r\n");
			}
			
		}
	}
}


