#include "freertos_key.h"
#include "hal_key.h"
#include "hal_led.h"
#include "hal_debug_usart.h"
#include "task.h"

#ifdef FREERTOS_USED_KEY_EXIT_IRQ
static portCHAR g_key_sta = 0;
#endif

/* LED1  任务句柄 */
static TaskHandle_t Key_LED1_Task_Handle = NULL;

/* LED2  任务句柄 */
static TaskHandle_t Key_Task_Handle = NULL;

static void Key_LED1_Task(void* pvParameters);/*Key LED1_Task  任务实现 */
static void Key_Task(void* pvParameters);/*Key Task  任务实现 */

#ifdef FREERTOS_USED_KEY_EXIT_IRQ
typedef enum key_sta{
	KEY_NULL = 0,
	KEY_SUSPEND = 1,
	KEY_RESUME = 2,
}e_key_sta_t;
#endif

void FreeRTOS_Key_Init(void)
{
#ifdef FREERTOS_USED_KEY_EXIT_IRQ
	Hal_Key_Exit_Init();
#else
	Hal_Key_Init();
#endif

	Hal_Led_Init();
}

void FreeRTOS_Key_Task(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(	(TaskFunction_t) Key_LED1_Task,
							(const char * )"Key_LED1_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )1,
							(TaskHandle_t * )&Key_LED1_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Key LED1_Task Creat success ! \r\n");

	xReturn = xTaskCreate(	(TaskFunction_t) Key_Task,
							(const char * )"Key_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )2,
							(TaskHandle_t * )&Key_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "Key Key_Task Creat success ! \r\n");
}

static void Key_LED1_Task(void* pvParameters)/*Key LED1_Task  任务实现 */
{
	while(1)
	{
		Hal_Led1_on();
		vTaskDelay(1000);

		Hal_Led1_off();
		vTaskDelay(1000);
	}
}
static void Key_Task(void* pvParameters)/*Key Task  任务实现 */
{
	while(1)
	{
	#ifdef FREERTOS_USED_KEY_EXIT_IRQ
		Scan_Key_Sta_Exit();
	#else
		Scan_Key_Sta();
	#endif
	
		vTaskDelay(20);
	}
}

#ifdef FREERTOS_USED_KEY_EXIT_IRQ
void Scan_Key_Sta_Exit(void)
{
	if((portCHAR)KEY_SUSPEND == Get_Key_Sta())
	{
		vTaskDelay(50);
		if(!Hal_Key0_Read_Bit())
		{
			Hal_Debug(INFO_LEVEL, "key exit led task suspend ! \r\n");
			vTaskSuspend(Key_LED1_Task_Handle);	
			Hal_Debug(INFO_LEVEL, "key exit led task suspend success ! \r\n");

			Set_Key_Sta((portCHAR)KEY_NULL);
		}
	}
	else if((portCHAR)KEY_RESUME == Get_Key_Sta())
	{
		vTaskDelay(50);
		if(!Hal_Key2_Read_Bit())
		{
			Hal_Debug(INFO_LEVEL, "key exit led task resume ! \r\n");
			vTaskResume(Key_LED1_Task_Handle);
			Hal_Debug(INFO_LEVEL, "key exit led task resume success ! \r\n");
			
			Set_Key_Sta((portCHAR)KEY_NULL);
		}
	}
}
#else
void Scan_Key_Sta(void)
{
	if(!Hal_Key0_Read_Bit())
	{	
		vTaskDelay(50);
		if(!Hal_Key0_Read_Bit())
		{
			Hal_Debug(INFO_LEVEL, "key led task suspend ! \r\n");
			vTaskSuspend(Key_LED1_Task_Handle);	
			Hal_Debug(INFO_LEVEL, "key led task suspend success ! \r\n");
		}
	}
	if(!Hal_Key2_Read_Bit())
	{
		vTaskDelay(50);
		if(!Hal_Key2_Read_Bit())
		{			
			Hal_Debug(INFO_LEVEL, "key led task resume ! \r\n");
			vTaskResume(Key_LED1_Task_Handle);
			Hal_Debug(INFO_LEVEL, "key led task resume success ! \r\n");
		}
	}
	
}
#endif

#ifdef FREERTOS_USED_KEY_EXIT_IRQ
void Key_Sta_Add(void)
{
	uint32_t ret = 0;
	ret = taskENTER_CRITICAL_FROM_ISR();
	{
		g_key_sta++;
	}
	taskEXIT_CRITICAL_FROM_ISR(ret);
}

portCHAR Get_Key_Sta(void)
{
	return g_key_sta;
}

void Set_Key_Sta(portCHAR sta)
{
	g_key_sta = sta;
}
#endif

#ifdef  FREERTOS_USED_KEY_EXIT_IRQ
void FreeRTOS_Key0_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line4) != RESET)
	{
		Set_Key_Sta((portCHAR)KEY_SUSPEND);
	}
	
	EXTI_ClearITPendingBit(EXTI_Line4); 
}

void FreeRTOS_Key2_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line2) != RESET)
	{		
		Set_Key_Sta((portCHAR)KEY_RESUME);
	}
	EXTI_ClearITPendingBit(EXTI_Line2); 
}

#endif

