#ifndef __FREERTOS_SEMAPHORE_H__
#define __FREERTOS_SEMAPHORE_H__

//#define FREERTOS_TEST_SEMAPHORE_BINARY
#define FREERTOS_TEST_SEMAPHORE_COUNT

#define FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE

#ifdef FREERTOS_USED_KEY_EXIT_IRQ_SEMAPHORE
#define FreeRTOS_Key0_Semaphore_IRQHandler	EXTI4_IRQHandler
#define FreeRTOS_Key2_Semaphore_IRQHandler	EXTI2_IRQHandler
#endif

#ifdef FREERTOS_TEST_SEMAPHORE_COUNT
#define FREERTOS_SEMAPHORE_COUNT_MAX	5
#define FREERTOS_SEMAPHORE_COUNT_INIT	5
#endif

void FreeRTOS_Semaphore_Bsp_Init(void);

void FreeRTOS_Semaphore_Task(void);

void FreeRTOS_Semaphore_Init(void);

#endif

