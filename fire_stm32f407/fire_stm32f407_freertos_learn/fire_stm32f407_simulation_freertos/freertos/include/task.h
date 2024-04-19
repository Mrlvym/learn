#ifndef INC_TASK_H
#define INC_TASK_H

#include "list.h"

/* 空闲任务优先级宏定义，在 task.h 中定义 */
#define tskIDLE_PRIORITY  ( ( UBaseType_t ) 0U )


/* ==========进入临界段，不带中断保护版本，不能嵌套=============== */                                
#define taskENTER_CRITICAL() portENTER_CRITICAL()
/* ==========退出临界段，不带中断保护版本，不能嵌套=============== */
#define taskEXIT_CRITICAL() portEXIT_CRITICAL()



/* ==========进入临界段，带中断保护版本，可以嵌套=============== */
#define taskENTER_CRITICAL_FROM_ISR() portSET_INTERRUPT_MASK_FROM_ISR()	
/* ==========退出临界段，带中断保护版本，可以嵌套=============== */
#define taskEXIT_CRITICAL_FROM_ISR( x ) portCLEAR_INTERRUPT_MASK_FROM_ISR( x )                        



typedef struct tskTaskControlBlock
{
	volatile StackType_t    *pxTopOfStack;    /* 鏍堥《 */

	ListItem_t		xStateListItem;   /* 浠诲姟鑺傜偣 */
    
    StackType_t             *pxStack;         /* 浠诲姟鏍堣捣濮嬪湴鍧� */
	                                          /* 浠诲姟鍚嶇О锛屽瓧绗︿覆褰㈠紡 */
	char                    pcTaskName[ configMAX_TASK_NAME_LEN ];  

	TickType_t xTicksToDelay; /* 用于延时 */   

	UBaseType_t			uxPriority;	/* 用于优先级 */
} tskTCB;
typedef tskTCB TCB_t;



#define taskYIELD()			portYIELD()

/* 任务句柄 */
typedef void * TaskHandle_t;


#if( configSUPPORT_STATIC_ALLOCATION == 1 )
TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,
					            const char * const pcName,
					            const uint32_t ulStackDepth,
					            void * const pvParameters,
					            UBaseType_t uxPriority,
					            StackType_t * const puxStackBuffer,
					            TCB_t * const pxTaskBuffer );
#endif /* configSUPPORT_STATIC_ALLOCATION */
                                
void prvInitialiseTaskLists( void );                                
void vTaskStartScheduler( void );
void vTaskSwitchContext( void );
											
void task_test(void);
                                
#endif /* INC_TASK_H */ 
