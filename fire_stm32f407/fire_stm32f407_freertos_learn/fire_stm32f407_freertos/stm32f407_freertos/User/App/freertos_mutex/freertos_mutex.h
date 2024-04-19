#ifndef __FREERTOS_MUTEX_H__
#define __FREERTOS_MUTEX_H__

//#define FREERTOS_TEST_SEMAPHORE_BINARY
#define FREERTOS_TEST_SEMAPHORE_MUTEX


void FreeRTOS_Mutex_Bsp_Init(void);
void FreeRTOS_Mutex_Task(void);
void FreeRTOS_Mutex_Init(void);


#endif

