#ifndef __FREERTOS_EVENT_H__
#define __FREERTOS_EVENT_H__

//#define FREERTOS_EVENT_ALLBIT


#define KEY0_EVENT_BIT	(1 << 0)
#define KEY2_EVENT_BIT	(1 << 1)

void FreeRTOS_Event_Bsp_Init(void);

void FreeRTOS_Event_Task(void);

void FreeRTOS_Event_Init(void);

#endif

