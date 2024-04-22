#ifndef __FREERTOS_NOTIFY_H__
#define __FREERTOS_NOTIFY_H__

#define NOTIFY_REPLACE_QUEUE		1
#define NOTIFY_REPLACE_SEMBIN		2
#define NOTIFY_REPLACE_SEMCOUNT		3
#define NOTIFY_REPLACE_EVENT		4

#define NOTIFY_REPLACE_SEL	NOTIFY_REPLACE_EVENT


#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE)

//#define TEST_QUEUE_USE_CHAR		1

#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE) */

#if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_EVENT)

#define EVENT_KEY0		(1 << 0)
#define EVENT_KEY2		(1 << 1)

//#define EVENT_INT_ALL	1

#endif /* #if (NOTIFY_REPLACE_SEL == NOTIFY_REPLACE_QUEUE) */



void FreeRTOS_Notify_Bsp_Init(void);

void FreeRTOS_Notify_Task(void);


#endif
