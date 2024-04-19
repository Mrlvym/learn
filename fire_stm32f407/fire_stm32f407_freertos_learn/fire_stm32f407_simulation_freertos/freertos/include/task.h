#ifndef INC_TASK_H
#define INC_TASK_H

#include "list.h"

/* �����������ȼ��궨�壬�� task.h �ж��� */
#define tskIDLE_PRIORITY  ( ( UBaseType_t ) 0U )


/* ==========�����ٽ�Σ������жϱ����汾������Ƕ��=============== */                                
#define taskENTER_CRITICAL() portENTER_CRITICAL()
/* ==========�˳��ٽ�Σ������жϱ����汾������Ƕ��=============== */
#define taskEXIT_CRITICAL() portEXIT_CRITICAL()



/* ==========�����ٽ�Σ����жϱ����汾������Ƕ��=============== */
#define taskENTER_CRITICAL_FROM_ISR() portSET_INTERRUPT_MASK_FROM_ISR()	
/* ==========�˳��ٽ�Σ����жϱ����汾������Ƕ��=============== */
#define taskEXIT_CRITICAL_FROM_ISR( x ) portCLEAR_INTERRUPT_MASK_FROM_ISR( x )                        



typedef struct tskTaskControlBlock
{
	volatile StackType_t    *pxTopOfStack;    /* 栈顶 */

	ListItem_t		xStateListItem;   /* 任务节点 */
    
    StackType_t             *pxStack;         /* 任务栈起始地址 */
	                                          /* 任务名称，字符串形式 */
	char                    pcTaskName[ configMAX_TASK_NAME_LEN ];  

	TickType_t xTicksToDelay; /* ������ʱ */   

	UBaseType_t			uxPriority;	/* �������ȼ� */
} tskTCB;
typedef tskTCB TCB_t;



#define taskYIELD()			portYIELD()

/* ������ */
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
