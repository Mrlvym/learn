#ifndef __FREERTOS_QUEUE_H__
#define __FREERTOS_QUEUE_H__

//#define FREERTOS_USED_KEY_EXIT_IRQ_QUEUE

#ifdef FREERTOS_USED_KEY_EXIT_IRQ_QUEUE
#define FreeRTOS_Key0_SendQueue_IRQHandler	EXTI4_IRQHandler
#define FreeRTOS_Key2_SendQueue_IRQHandler	EXTI2_IRQHandler
#endif

#define QUEUE_KEY0_LEN	4
#define QUEUE_KEY0_SIZE	1

#define QUEUE_KEY2_LEN	4
#define QUEUE_KEY2_SIZE	1

void FreeRTOS_Queue_Bsp_Init(void);

void FreeRTOS_Queue_Task(void);

void FreeRTOS_Queue_Init(void);

#endif


