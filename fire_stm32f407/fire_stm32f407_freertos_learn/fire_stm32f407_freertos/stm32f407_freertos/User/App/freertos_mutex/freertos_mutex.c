#include "freertos_mutex.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


#include "hal_debug_usart.h"

#if (defined(FREERTOS_TEST_SEMAPHORE_BINARY))
static SemaphoreHandle_t Mutex_Test_SemaphoreBinary_Handle = NULL;
#endif

#if (defined(FREERTOS_TEST_SEMAPHORE_MUTEX))
static SemaphoreHandle_t Mutex_Test_SemaphoreMutex_Handle = NULL;
#endif


TaskHandle_t Mutex_Low_Task_Handle = NULL;
TaskHandle_t Mutex_Mid_Task_Handle = NULL;
TaskHandle_t Mutex_High_Task_Handle = NULL;

static void Mutex_Low_Task(void* pvParameters);

static void Mutex_Mid_Task(void* pvParameters);

static void Mutex_High_Task(void* pvParameters);

void FreeRTOS_Mutex_Bsp_Init(void)
{
	
}


void FreeRTOS_Mutex_Task(void)
{
	BaseType_t xReturn = pdPASS;

	FreeRTOS_Mutex_Init();

	xReturn = xTaskCreate(	(TaskFunction_t) Mutex_Low_Task,
							(const char * )"Mutex_Low_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )1,
							(TaskHandle_t * )&Mutex_Low_Task_Handle);
	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Mutex_Low_Task Creat success ! \r\n");

	xReturn = xTaskCreate(	(TaskFunction_t) Mutex_Mid_Task,
							(const char * )"Mutex_Mid_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )2,
							(TaskHandle_t * )&Mutex_Mid_Task_Handle);
	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Mutex_Mid_Task Creat success ! \r\n");

	xReturn = xTaskCreate(	(TaskFunction_t) Mutex_High_Task,
							(const char * )"Mutex_High_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )3,
							(TaskHandle_t * )&Mutex_High_Task_Handle);
	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Mutex_High_Task Creat success ! \r\n");
}

void FreeRTOS_Mutex_Init(void)
{
	BaseType_t xReturn = pdFALSE; 
	
#if (defined(FREERTOS_TEST_SEMAPHORE_BINARY))

	Mutex_Test_SemaphoreBinary_Handle = xSemaphoreCreateBinary();

	if(NULL != Mutex_Test_SemaphoreBinary_Handle){
		xReturn = xSemaphoreGive(Mutex_Test_SemaphoreBinary_Handle);

		if(pdFALSE != xReturn){
			Hal_Debug(INFO_LEVEL, "Creat Mutex_Test_SemaphoreBinary_Handle success ! \r\n");
		}
	}
	
#endif

#if (defined(FREERTOS_TEST_SEMAPHORE_MUTEX))

	Mutex_Test_SemaphoreMutex_Handle = xSemaphoreCreateMutex();

	if(NULL != Mutex_Test_SemaphoreMutex_Handle){
		xReturn = xSemaphoreGive(Mutex_Test_SemaphoreMutex_Handle);

		if(pdFALSE != xReturn){
			Hal_Debug(INFO_LEVEL, "Creat Mutex_Test_SemaphoreMutex_Handle success ! \r\n");
		}
	}
	
#endif

}

static void Mutex_Low_Task(void* pvParameters)
{
	static uint32_t i = 0;
	BaseType_t xReturn = pdFALSE; 

	while(1)
	{
	#if (defined(FREERTOS_TEST_SEMAPHORE_BINARY))
		Hal_Debug(PRINTF, "Mutex_Low_Task T  BinarySem ... \r\n");
		xReturn = xSemaphoreTake((QueueHandle_t)Mutex_Test_SemaphoreBinary_Handle,  
								 (TickType_t)portMAX_DELAY);

		if(pdTRUE == xReturn){
			Hal_Debug(PRINTF, "Mutex_Low_Task is Running ... \r\n");
		}

		for(i = 0; i < 8000000; i++){
			taskYIELD();
		}
		Hal_Debug(PRINTF, "Mutex_Low_Task G  BinarySem ... \r\n");
		xReturn = xSemaphoreGive((QueueHandle_t)Mutex_Test_SemaphoreBinary_Handle);
	#endif

	#if (defined(FREERTOS_TEST_SEMAPHORE_MUTEX))
		Hal_Debug(PRINTF, "Mutex_Low_Task T  MutexSem ... \r\n");
		xReturn = xSemaphoreTake((QueueHandle_t)Mutex_Test_SemaphoreMutex_Handle,  
								 (TickType_t)portMAX_DELAY);

		if(pdTRUE == xReturn){
			Hal_Debug(PRINTF, "Mutex_Low_Task is Running ... \r\n");
		}

		for(i = 0; i < 8000000; i++){
			taskYIELD();
		}
		Hal_Debug(PRINTF, "Mutex_Low_Task G  MutexSem ... \r\n");
		xReturn = xSemaphoreGive((QueueHandle_t)Mutex_Test_SemaphoreMutex_Handle);

	#endif
		vTaskDelay(500);		
	}
}

static void Mutex_Mid_Task(void* pvParameters)
{
	while(1)
	{
		Hal_Debug(PRINTF, "Mutex_Mid_Task is Running ... \r\n");
		
		vTaskDelay(500);	
	}
	
}

static void Mutex_High_Task(void* pvParameters)
{
	BaseType_t xReturn = pdFALSE; 
	
	while(1)
	{
	#if (defined(FREERTOS_TEST_SEMAPHORE_BINARY))
	
		Hal_Debug(PRINTF, "Mutex_High_Task T  BinarySem ... \r\n");
		xReturn = xSemaphoreTake((QueueHandle_t)Mutex_Test_SemaphoreBinary_Handle,	
								 (TickType_t)portMAX_DELAY);

		if(pdTRUE == xReturn){
			Hal_Debug(PRINTF, "Mutex_High_Task is Running ... \r\n");
		}

		Hal_Debug(PRINTF, "Mutex_High_Task G  BinarySem ... \r\n");
		xReturn = xSemaphoreGive((QueueHandle_t)Mutex_Test_SemaphoreBinary_Handle);

	#endif

	#if (defined(FREERTOS_TEST_SEMAPHORE_MUTEX))

	
		Hal_Debug(PRINTF, "Mutex_High_Task T  MutexSem ... \r\n");
		xReturn = xSemaphoreTake((QueueHandle_t)Mutex_Test_SemaphoreMutex_Handle,	
								 (TickType_t)portMAX_DELAY);

		if(pdTRUE == xReturn){
			Hal_Debug(PRINTF, "Mutex_High_Task is Running ... \r\n");
		}

		Hal_Debug(PRINTF, "Mutex_High_Task G  MutexSem ... \r\n");
		xReturn = xSemaphoreGive((QueueHandle_t)Mutex_Test_SemaphoreMutex_Handle);

	#endif
	
		vTaskDelay(500);	
	}
	
}




