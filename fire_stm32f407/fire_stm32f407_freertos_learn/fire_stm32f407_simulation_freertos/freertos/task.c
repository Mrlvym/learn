#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"

TaskHandle_t IdleTask_Handle;
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
TCB_t IdleTaskTCB;

TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE                    30
StackType_t Task1Stack[TASK1_STACK_SIZE];
TCB_t Task1TCB;

TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE                    30
StackType_t Task2Stack[TASK2_STACK_SIZE];
TCB_t Task2TCB;

TaskHandle_t Task3_Handle;
#define TASK3_STACK_SIZE                    30
StackType_t Task3Stack[TASK3_STACK_SIZE];
TCB_t Task3TCB;


extern TickType_t xTickCount;

/* ���� uxTopReadyPriority���� task.c �ж��� */
static volatile UBaseType_t uxTopReadyPriority = tskIDLE_PRIORITY;

static volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;

static List_t xDelayedTaskList1;
static List_t xDelayedTaskList2;
static List_t * volatile pxDelayedTaskList;
static List_t * volatile pxOverflowDelayedTaskList;

static volatile TickType_t xNextTaskUnblockTime		= ( TickType_t ) 0U;
static volatile BaseType_t xNumOfOverflows 			= ( BaseType_t ) 0;

/* ����������ȼ��ľ�������ͨ�÷��� */
#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 0 )
	/* uxTopReadyPriority ����Ǿ��������������ȼ� */
//	#define taskRECORD_READY_PRIORITY( uxPriority )
//	{
//		if( ( uxPriority ) > uxTopReadyPriority )
//		{
//			uxTopReadyPriority = uxPriority;
//		}
//	}
/* uxTopReadyPriority ����Ǿ��������������ȼ� */
	#define taskRECORD_READY_PRIORITY( uxPriority )														\
	{																									\
		if( ( uxPriority ) > uxTopReadyPriority )														\
		{																								\
			uxTopReadyPriority = ( uxPriority );														\
		}																								\
	} /* taskRECORD_READY_PRIORITY */

//	#define taskSELECT_HIGHEST_PRIORITY_TASK()
//	{
//		UBaseType_t uxTopPriority = uxTopReadyPriority;
//		/* Ѱ�Ұ������������������ȼ��Ķ��� */\
//		while(listLIST_IS_EMPTY(&pxReadyTasksLists[uxTopPriority]))
//		{
//			--uxTopPriority;
//		}
//		/* ��ȡ���ȼ���ߵľ�������� TCB��Ȼ����µ� pxCurrentTCB */\
//		listGET_OWNER_OF_NEXT_ENTRY(pxCurrentTCB, &pxReadyTasksLists[uxTopPriority]);
//		/* ���� uxTopReadyPriority */\
//		uxTopReadyPriority = uxTopPriority;
//	}
	
	#define taskSELECT_HIGHEST_PRIORITY_TASK()															\
	{																									\
		UBaseType_t uxTopPriority = uxTopReadyPriority;														\
																										\
		/* Ѱ�Ұ������������������ȼ��Ķ��� */                                                          \
		while( listLIST_IS_EMPTY( &( pxReadyTasksLists[ uxTopPriority ] ) ) )							\
		{																								\
			--uxTopPriority;																			\
		}																								\
																										\
		/* ��ȡ���ȼ���ߵľ��������TCB��Ȼ����µ�pxCurrentTCB */							            \
		listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists[ uxTopPriority ] ) );			\
		/* ����uxTopReadyPriority */                                                                    \
		uxTopReadyPriority = uxTopPriority;																\
	} /* taskSELECT_HIGHEST_PRIORITY_TASK */
#else
	#define taskRECORD_READY_PRIORITY( uxPriority )	portRECORD_READY_PRIORITY( uxPriority, uxTopReadyPriority )

	/*-----------------------------------------------------------*/

	#define taskSELECT_HIGHEST_PRIORITY_TASK()														    \
	{																								    \
	UBaseType_t uxTopPriority;																		    \
																									    \
		/* Ѱ��������ȼ� */								                            \
		portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );								    \
		/* ��ȡ���ȼ���ߵľ��������TCB��Ȼ����µ�pxCurrentTCB */                                       \
		listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists[ uxTopPriority ] ) );		    \
	} /* taskSELECT_HIGHEST_PRIORITY_TASK() */

	/*-----------------------------------------------------------*/
#if 1
	#define taskRESET_READY_PRIORITY( uxPriority )														\
	{																									\
		if( listCURRENT_LIST_LENGTH( &( pxReadyTasksLists[ ( uxPriority ) ] ) ) == ( UBaseType_t ) 0 )	\
		{																								\
			portRESET_READY_PRIORITY( ( uxPriority ), ( uxTopReadyPriority ) );							\
		}																								\
	}
#else
    #define taskRESET_READY_PRIORITY( uxPriority )											            \
    {																							        \
            portRESET_READY_PRIORITY( ( uxPriority ), ( uxTopReadyPriority ) );					        \
    }
#endif

#endif

/* ��������ӵ������б� */                                    
#define prvAddTaskToReadyList( pxTCB )																   \
	taskRECORD_READY_PRIORITY( ( pxTCB )->uxPriority );												   \
	vListInsertEnd( &( pxReadyTasksLists[ ( pxTCB )->uxPriority ] ), &( ( pxTCB )->xStateListItem ) ); \


#if 0
/*
*************************************************************************
*                               ������ƿ�
*************************************************************************
*/

/* ��ǰ�������е������������ƿ�ָ�룬Ĭ�ϳ�ʼ��ΪNULL */
TCB_t * volatile pxCurrentTCB = NULL;

/* ��������б� */
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];


static volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;


/*
*************************************************************************
*                               ��������
*************************************************************************
*/

static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* ������� */
									const char * const pcName,              /* �������ƣ��ַ�����ʽ */
									const uint32_t ulStackDepth,            /* ����ջ��С����λΪ�� */
									void * const pvParameters,              /* �����β� */
									TaskHandle_t * const pxCreatedTask,     /* ������ */
									TCB_t *pxNewTCB );


/*
*************************************************************************
*                               �궨��
*************************************************************************
*/
                                    

                                   

/*
*************************************************************************
*                               ��̬���񴴽�����
*************************************************************************
*/
#if( configSUPPORT_STATIC_ALLOCATION == 1 )

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           /* ������� */
					            const char * const pcName,           /* �������ƣ��ַ�����ʽ */
					            const uint32_t ulStackDepth,         /* ����ջ��С����λΪ�� */
					            void * const pvParameters,           /* �����β� */
					            StackType_t * const puxStackBuffer,  /* ����ջ��ʼ��ַ */
					            TCB_t * const pxTaskBuffer )         /* ������ƿ�ָ�� */
{
	TCB_t *pxNewTCB;
	TaskHandle_t xReturn;

	if( ( pxTaskBuffer != NULL ) && ( puxStackBuffer != NULL ) )
	{		
		pxNewTCB = ( TCB_t * ) pxTaskBuffer; 
		pxNewTCB->pxStack = ( StackType_t * ) puxStackBuffer;

		/* �����µ����� */
		prvInitialiseNewTask( pxTaskCode,        /* ������� */
                              pcName,            /* �������ƣ��ַ�����ʽ */
                              ulStackDepth,      /* ����ջ��С����λΪ�� */ 
                              pvParameters,      /* �����β� */
                              &xReturn,          /* ������ */ 
                              pxNewTCB);         /* ����ջ��ʼ��ַ */      

	}
	else
	{
		xReturn = NULL;
	}

	/* ������������������񴴽��ɹ�����ʱxReturnӦ��ָ��������ƿ� */
    return xReturn;
}

#endif /* configSUPPORT_STATIC_ALLOCATION */


static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* ������� */
									const char * const pcName,              /* �������ƣ��ַ�����ʽ */
									const uint32_t ulStackDepth,            /* ����ջ��С����λΪ�� */
									void * const pvParameters,              /* �����β� */
									TaskHandle_t * const pxCreatedTask,     /* ������ */
									TCB_t *pxNewTCB )                       /* ������ƿ�ָ�� */

{
	StackType_t *pxTopOfStack;
	UBaseType_t x;	
	
	/* ��ȡջ����ַ */
	pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - ( uint32_t ) 1 );
	//pxTopOfStack = ( StackType_t * ) ( ( ( portPOINTER_SIZE_TYPE ) pxTopOfStack ) & ( ~( ( portPOINTER_SIZE_TYPE ) portBYTE_ALIGNMENT_MASK ) ) );
	/* ������8�ֽڶ��� */
	pxTopOfStack = ( StackType_t * ) ( ( ( uint32_t ) pxTopOfStack ) & ( ~( ( uint32_t ) 0x0007 ) ) );	

	/* ����������ִ洢��TCB�� */
	for( x = ( UBaseType_t ) 0; x < ( UBaseType_t ) configMAX_TASK_NAME_LEN; x++ )
	{
		pxNewTCB->pcTaskName[ x ] = pcName[ x ];

		if( pcName[ x ] == 0x00 )
		{
			break;
		}
	}
	/* �������ֵĳ��Ȳ��ܳ���configMAX_TASK_NAME_LEN */
	pxNewTCB->pcTaskName[ configMAX_TASK_NAME_LEN - 1 ] = '\0';

    /* ��ʼ��TCB�е�xStateListItem�ڵ� */
    vListInitialiseItem( &( pxNewTCB->xStateListItem ) );
    /* ����xStateListItem�ڵ��ӵ���� */
	listSET_LIST_ITEM_OWNER( &( pxNewTCB->xStateListItem ), pxNewTCB );
    
    
    /* ��ʼ������ջ */
	pxNewTCB->pxTopOfStack = (StackType_t *)pxPortInitialiseStack( pxTopOfStack, pxTaskCode, pvParameters );   


	/* ��������ָ��������ƿ� */
    if( ( void * ) pxCreatedTask != NULL )
	{		
		*pxCreatedTask = ( TaskHandle_t ) pxNewTCB;
	}
}

/* ��ʼ��������ص��б� */
void prvInitialiseTaskLists( void )
{
    UBaseType_t uxPriority;
    
    
    for( uxPriority = ( UBaseType_t ) 0U; uxPriority < ( UBaseType_t ) configMAX_PRIORITIES; uxPriority++ )
	{
		vListInitialise( &( pxReadyTasksLists[ uxPriority ] ) );
	}
}

extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
void vTaskStartScheduler( void )
{
    /* �ֶ�ָ����һ�����е����� */
    pxCurrentTCB = &Task1TCB;
    
    /* ���������� */
    if( xPortStartScheduler() != pdFALSE )
    {
        /* �����������ɹ����򲻻᷵�أ��������������� */
    }
}

void vTaskSwitchContext( void )
{    
    /* �������������л� */
    if( pxCurrentTCB == &Task1TCB )
    {
        pxCurrentTCB = &Task2TCB;
    }
    else
    {
        pxCurrentTCB = &Task1TCB;
    }
}

#else

/* ��������б� */
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
TCB_t * volatile pxCurrentTCB = NULL;


void prvInitialiseTaskLists( void )
{
	UBaseType_t uxPriority;

	for(uxPriority = 0; uxPriority < configMAX_PRIORITIES; uxPriority++)
	{
		vListInitialise(&pxReadyTasksLists[uxPriority]);
	}

	vListInitialise(&xDelayedTaskList1);
	vListInitialise(&xDelayedTaskList2);

	pxDelayedTaskList = &xDelayedTaskList1;
	pxOverflowDelayedTaskList = &xDelayedTaskList2;
}

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );

static void prvAddNewTaskToReadyList( TCB_t *pxNewTCB )
{
	taskENTER_CRITICAL();
	{
		uxCurrentNumberOfTasks++;

		if(pxCurrentTCB == NULL)
		{
			pxCurrentTCB = pxNewTCB;

			if(uxCurrentNumberOfTasks == 1)
			{
				prvInitialiseTaskLists();
			}
		}
		else
		{
			if(pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority)
			{
				pxCurrentTCB = pxNewTCB;
			}
		}

		prvAddTaskToReadyList(pxNewTCB);
	}
	taskEXIT_CRITICAL();
}



static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* ������� */
									const char * const pcName,              /* �������ƣ��ַ�����ʽ */
									const uint32_t ulStackDepth,            /* ����ջ��С����λΪ�� */
									void * const pvParameters,              /* �����β� */
									UBaseType_t uxPriority,                 /* �������ȼ�����ֵԽ�����ȼ�Խ�� */
									TaskHandle_t * const pxCreatedTask,     /* ������ */
									TCB_t *pxNewTCB )                       /* ������ƿ�ָ�� */
{
	StackType_t *pxTopOfStack;
	UBaseType_t x;

	pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (StackType_t)1);
	pxTopOfStack = (StackType_t *)(((StackType_t)pxTopOfStack) & (~((StackType_t)0x0007)));

	for(x = 0; x < (UBaseType_t)configMAX_TASK_NAME_LEN; x++)
	{
		pxNewTCB->pcTaskName[x] = pcName[x];

		if(pcName[x] == '\0')
			break;
	}
	pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN-1] = '\0';

	vListInitialiseItem(&pxNewTCB->xStateListItem);
	listSET_LIST_ITEM_OWNER(&pxNewTCB->xStateListItem, pxNewTCB);

	/* ��ʼ�����ȼ� */
	if( uxPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
	{
		uxPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
	}
	pxNewTCB->uxPriority = uxPriority;

	pxNewTCB->pxTopOfStack = (StackType_t *)pxPortInitialiseStack(pxTopOfStack,
												   pxTaskCode,
												   pvParameters);

	if((void *)pxCreatedTask != NULL)
	{
		*pxCreatedTask = (TaskHandle_t)pxNewTCB;
	}
}


#if( configSUPPORT_STATIC_ALLOCATION == 1 )

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,
					            const char * const pcName,
					            const uint32_t ulStackDepth,
					            void * const pvParameters,
					            UBaseType_t uxPriority,
					            StackType_t * const puxStackBuffer,
					            TCB_t * const pxTaskBuffer )
{

	TCB_t *pxNewTCB;
	TaskHandle_t xReturn;

	if(pxTaskBuffer != NULL && puxStackBuffer != NULL)
	{
		pxNewTCB = (TCB_t *)pxTaskBuffer;
		pxNewTCB->pxStack = (StackType_t *)puxStackBuffer;

		prvInitialiseNewTask(pxTaskCode,
							 pcName,
							 ulStackDepth,
							 pvParameters,
							 uxPriority,
							 &xReturn,
							 pxNewTCB);
		
		prvAddNewTaskToReadyList(pxNewTCB);
	}
	else
	{
		xReturn = NULL;
	}

	return xReturn;
	
}

#endif
void prvIdleTask( void *p_arg );
void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer, 
                                    StackType_t **ppxIdleTaskStackBuffer, 
                                    uint32_t *pulIdleTaskStackSize );

void vTaskStartScheduler( void )
{
	TCB_t *pxIdleTaskTCBBuffer = (TCB_t *)NULL;
	StackType_t * pxIdleTaskStackBuffer = (StackType_t *)NULL;
	uint32_t ulIdleTaskStackSize;

	vApplicationGetIdleTaskMemory(
								  &pxIdleTaskTCBBuffer,
								  &pxIdleTaskStackBuffer,
								  &ulIdleTaskStackSize
								  );
	IdleTask_Handle = xTaskCreateStatic(
						  (TaskFunction_t)prvIdleTask,
						  (char *)"IDEL",
						  ulIdleTaskStackSize,
						  (void *)NULL,
						  (UBaseType_t)tskIDLE_PRIORITY,
						  (StackType_t *)pxIdleTaskStackBuffer,
						  (TCB_t *)pxIdleTaskTCBBuffer);	

//	vListInsertEnd(
//				(List_t *)&pxReadyTasksLists[0], 
//				(ListItem_t *)&pxIdleTaskTCBBuffer->xStateListItem);
//
//	pxCurrentTCB = &Task1TCB;

	xNextTaskUnblockTime = portMAX_DELAY;

	xTickCount = (TickType_t)0U;

	if(xPortStartScheduler() != pdFALSE)
	{
		
	}
}
//void vTaskSwitchContext( void )
//{
//	/* �������������л� */
//    if( pxCurrentTCB == &Task1TCB )
//    {
//        pxCurrentTCB = &Task2TCB;
//    }
//    else
//    {
//        pxCurrentTCB = &Task1TCB;
//    }
//}

//void vTaskSwitchContext( void )
//{
//    if( pxCurrentTCB == (TCB_t *)&IdleTaskTCB )
//    {
//        if(Task1TCB.xTicksToDelay == 0)
//        {
//			pxCurrentTCB = (TCB_t *)&Task1TCB;
//		}
//		else if(Task2TCB.xTicksToDelay == 0)
//		{
//			pxCurrentTCB = (TCB_t *)&Task2TCB;			
//		}
//		else
//		{
//			return ;
//		}
//		
//    }
//    else
//    {
//        if( pxCurrentTCB == (TCB_t *)&Task1TCB )
//        {
//			if(Task2TCB.xTicksToDelay == 0)
//			{
//				pxCurrentTCB = (TCB_t *)&Task2TCB;
//			}
//			else if(pxCurrentTCB->xTicksToDelay != 0)
//			{
//				pxCurrentTCB = (TCB_t *)&IdleTaskTCB;				
//			}
//			else
//			{
//				return ;
//			}
//		}
//		else if( pxCurrentTCB == (TCB_t *)&Task2TCB )
//		{			
//			if(Task1TCB.xTicksToDelay == 0)
//			{
//				pxCurrentTCB = (TCB_t *)&Task1TCB;
//			}
//			else if(pxCurrentTCB->xTicksToDelay != 0)
//			{
//				pxCurrentTCB = (TCB_t *)&IdleTaskTCB;				
//			}
//			else
//			{
//				return ;
//			}
//		}
//    }
//}
void vTaskSwitchContext( void )
{
	taskSELECT_HIGHEST_PRIORITY_TASK();
}
static void prvResetNextTaskUnblockTime( void );
static void prvAddCurrentTaskToDelayedList( TickType_t xTicksToWait );

void vTaskDelay( const TickType_t xTicksToDelay )
{
	TCB_t *pxTCB = NULL;

	/* ��ȡ��ǰ����� TCB */
	pxTCB = pxCurrentTCB; 

//	/* ������ʱʱ�� */
//	pxTCB->xTicksToDelay = xTicksToDelay;

//	taskRESET_READY_PRIORITY(pxTCB->uxPriority);

	prvAddCurrentTaskToDelayedList(xTicksToDelay);


	/* �����л� */
	taskYIELD(); 
}
#if 1
BaseType_t xTaskIncrementTick( void )
{
	TCB_t *pxTCB = NULL;
	BaseType_t i = 0;
	TickType_t xItemValue;
	BaseType_t xSwitchRequired = pdFALSE; 

	const TickType_t xConstTickCount = xTickCount + 1;
	xTickCount = xConstTickCount;

//	for(i = 0; i < configMAX_PRIORITIES; i++)
//	{
//		pxTCB = (TCB_t *)listGET_OWNER_OF_HEAD_ENTRY(&pxReadyTasksLists[i]);

//		if(pxTCB->xTicksToDelay > 0)
//		{
//			pxTCB->xTicksToDelay--;

//			if(pxTCB->xTicksToDelay == 0)
//			{
//				taskRECORD_READY_PRIORITY(pxTCB->uxPriority);
//			}
//		}
//	}

	//��� ���л���ʱ�б�
	if(xConstTickCount == (TickType_t)0U)
	{
		//�л���ʱ�б�
		List_t *pxTemp;
		pxTemp = pxDelayedTaskList;
		pxDelayedTaskList = pxOverflowDelayedTaskList;
		pxOverflowDelayedTaskList = pxTemp;
		xNumOfOverflows++;//�����������
		//��ʼ�����б�xNextTaskUnblockTimeֵ
		prvResetNextTaskUnblockTime();
	}

	//����ʱ����ʱ
	if(xConstTickCount >= xNextTaskUnblockTime)
	{
	
		while(1)
		{
			if(listLIST_IS_EMPTY(pxDelayedTaskList) != pdFALSE)//��ʱ�б�Ϊ�գ���ʼ��ΪportMAX_DELAY
			{
				xNextTaskUnblockTime = portMAX_DELAY;
				break;
			}
			else
			{
				pxTCB = listGET_OWNER_OF_HEAD_ENTRY(pxDelayedTaskList);
				xItemValue = listGET_LIST_ITEM_VALUE(&(pxTCB->xStateListItem));
					
				if(xConstTickCount < xItemValue)
				{
					xNextTaskUnblockTime = xItemValue;
					break;
				}
				uxListRemove(&(pxTCB->xStateListItem));//����ʱ�б�ɾ��

				//��ӵ������б�
				taskRECORD_READY_PRIORITY(pxTCB->uxPriority);
				vListInsertEnd(&pxReadyTasksLists[pxTCB->uxPriority], &pxTCB->xStateListItem);

				#if (configUSE_PREEMPTION == 1)
					if(pxTCB->uxPriority > pxCurrentTCB->uxPriority)
					{
						xSwitchRequired = pdTRUE; 
					}
				#endif
			}
		}
	}
	#if ((configUSE_PREEMPTION == 1) && (configUSE_TIME_SLICING == 1))
		if(listCURRENT_LIST_LENGTH( &( pxReadyTasksLists[ pxCurrentTCB->uxPriority ] )) > 1)
		{
			xSwitchRequired = pdTRUE;
		}
	#endif

//	taskYIELD();
	return xSwitchRequired;
}
#else
#define taskSWITCH_DELAYED_LISTS()\
{\
	List_t *pxTemp;\
	pxTemp = pxDelayedTaskList;\
	pxDelayedTaskList = pxOverflowDelayedTaskList;\
	pxOverflowDelayedTaskList = pxTemp;\
	xNumOfOverflows++;\
	prvResetNextTaskUnblockTime();\
}

void xTaskIncrementTick( void )
{
	TCB_t * pxTCB;
	TickType_t xItemValue;

	const TickType_t xConstTickCount = xTickCount + 1;
	xTickCount = xConstTickCount;

	/* ���xConstTickCount��������л���ʱ�б� */
	if( xConstTickCount == ( TickType_t ) 0U )
	{
		taskSWITCH_DELAYED_LISTS();
	}

	/* �������ʱ������ʱ���� */
	if( xConstTickCount >= xNextTaskUnblockTime )
	{
		for( ;; )
		{
			if( listLIST_IS_EMPTY( pxDelayedTaskList ) != pdFALSE )
			{
				/* ��ʱ�б�Ϊ�գ�����xNextTaskUnblockTimeΪ���ܵ����ֵ */
				xNextTaskUnblockTime = portMAX_DELAY;
				break;
			}
			else /* ��ʱ�б�Ϊ�� */
			{
				pxTCB = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxDelayedTaskList );
				xItemValue = listGET_LIST_ITEM_VALUE( &( pxTCB->xStateListItem ) );

				/* ֱ������ʱ�б���������ʱ���ڵ������Ƴ�������forѭ�� */
                if( xConstTickCount < xItemValue )
				{
					xNextTaskUnblockTime = xItemValue;
					break;
				}

				/* ���������ʱ�б��Ƴ��������ȴ�״̬ */
				( void ) uxListRemove( &( pxTCB->xStateListItem ) );

				/* ������ȴ���������ӵ������б� */
				prvAddTaskToReadyList( pxTCB );
			}
		}
	}/* xConstTickCount >= xNextTaskUnblockTime */
    
    /* �����л� */
    portYIELD();
}
#endif
static void prvResetNextTaskUnblockTime( void )
{
	TCB_t *pxTCB;

	if(listLIST_IS_EMPTY(pxDelayedTaskList) != pdFALSE)//��ʱ�б�Ϊ�գ���ʼ��ΪportMAX_DELAY
	{
		xNextTaskUnblockTime = portMAX_DELAY;
	}
	else//��Ϊ�գ���ʼ��Ϊ��ʱ�б�ĵ�һ��Ԫ�ص�pxTCB->xStateListItem.xItemValue
	{
		pxTCB = (TCB_t *)listGET_OWNER_OF_HEAD_ENTRY(pxDelayedTaskList);
		xNextTaskUnblockTime = listGET_LIST_ITEM_VALUE(&(pxTCB->xStateListItem));	
	}
}


void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer, 
                                    StackType_t **ppxIdleTaskStackBuffer, 
                                    uint32_t *pulIdleTaskStackSize )
{
		*ppxIdleTaskTCBBuffer=&IdleTaskTCB;
		*ppxIdleTaskStackBuffer=IdleTaskStack; 
		*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}

#if 1									
static void prvAddCurrentTaskToDelayedList( TickType_t xTicksToWait )
{
	TickType_t xTimeToWake;
	
	/* ��ȡϵͳʱ�������� xTickCount ��ֵ */
	const TickType_t xConstTickCount = xTickCount;

	/* ������Ӿ����б����Ƴ� */ 
	if ( uxListRemove( &( pxCurrentTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
	{
		/* �����������ȼ�λͼ�ж�Ӧ��λ��� */
		portRESET_READY_PRIORITY(pxCurrentTCB->uxPriority, uxTopReadyPriority);
	}

	/* ����������ʱ����ʱ��ϵͳʱ�������� xTickCount ��ֵ�Ƕ��� */
	xTimeToWake = xConstTickCount + xTicksToWait;

	listSET_LIST_ITEM_VALUE(&(pxCurrentTCB->xStateListItem), xTimeToWake);

	//���
	if(xTimeToWake < xConstTickCount)
	{
		vListInsert(pxOverflowDelayedTaskList, &(pxCurrentTCB->xStateListItem));
	}
	else//δ���
	{
		vListInsert(pxDelayedTaskList, &(pxCurrentTCB->xStateListItem));

		if(xTimeToWake < xNextTaskUnblockTime)
		{
			xNextTaskUnblockTime = xTimeToWake; 
		}
	}
}
#else
static void prvAddCurrentTaskToDelayedList( TickType_t xTicksToWait )
{
    TickType_t xTimeToWake;
    
    /* ��ȡϵͳʱ��������xTickCount��ֵ */
    const TickType_t xConstTickCount = xTickCount;

    /* ������Ӿ����б����Ƴ� */
	if( uxListRemove( &( pxCurrentTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
	{
		/* �����������ȼ�λͼ�ж�Ӧ��λ��� */
        portRESET_READY_PRIORITY( pxCurrentTCB->uxPriority, uxTopReadyPriority );
	}

    /* ������ʱ����ʱ��ϵͳʱ��������xTickCount��ֵ�Ƕ��� */
    xTimeToWake = xConstTickCount + xTicksToWait;

    /* ����ʱ���ڵ�ֵ����Ϊ�ڵ������ֵ */
    listSET_LIST_ITEM_VALUE( &( pxCurrentTCB->xStateListItem ), xTimeToWake );

    /* ��� */
    if( xTimeToWake < xConstTickCount )
    {
        vListInsert( pxOverflowDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );
    }
    else /* û����� */
    {

        vListInsert( pxDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );

        /* ������һ���������ʱ�̱���xNextTaskUnblockTime��ֵ */
        if( xTimeToWake < xNextTaskUnblockTime )
        {
            xNextTaskUnblockTime = xTimeToWake;
        }
    }	
}
#endif

#endif


void Task1_Entry( void *p_arg );
void Task2_Entry( void *p_arg );
void Task3_Entry( void *p_arg );

uint32_t d_task1_upri = 0;
void task_test(void)
{
//	prvInitialiseTaskLists();
	
	d_task1_upri = (uint32_t)&Task1TCB.uxPriority;
	Task1_Handle = xTaskCreateStatic(
					  (TaskFunction_t)Task1_Entry,
					  (char *)"task1",
					  (uint32_t)TASK1_STACK_SIZE,
					  (void *)NULL,
					  (UBaseType_t)1,
					  (StackType_t *)Task1Stack,
					  (TCB_t *)&Task1TCB
					  );
//	vListInsertEnd((List_t *)&pxReadyTasksLists[1], (ListItem_t *)&Task1TCB.xStateListItem);
	
	Task2_Handle = xTaskCreateStatic(
					  (TaskFunction_t)Task2_Entry,
					  (char *)"task2",
					  (uint32_t)TASK2_STACK_SIZE,
					  (void *)NULL,
					  (UBaseType_t)1,
					  (StackType_t *)Task2Stack,
					  (TCB_t *)&Task2TCB
					  );
//	vListInsertEnd((List_t *)&pxReadyTasksLists[2], (ListItem_t *)&Task2TCB.xStateListItem);

	Task3_Handle = xTaskCreateStatic(
						  (TaskFunction_t)Task3_Entry,
						  (char *)"task3",
						  (uint32_t)TASK3_STACK_SIZE,
						  (void *)NULL,
						  (UBaseType_t)2,
						  (StackType_t *)Task3Stack,
						  (TCB_t *)&Task3TCB
						  );

						
	vTaskStartScheduler();

	while(1);
}

void delay (uint32_t count)
{
	for(; count!=0; count--);
}

portCHAR flag1;
portCHAR flag2;
portCHAR flag3;

void Task1_Entry( void *p_arg )
{
	while(1)
	{
	#if 0
		flag1 = 0;
		delay(1000);
		flag1 = 1;
		delay(1000);

//		taskYIELD();
	#else
		flag1 = 0;
		vTaskDelay(1);
		flag1 = 1;
		vTaskDelay(1);
	#endif
	}
}
void Task2_Entry( void *p_arg )
{
	while(1)
	{
	#if 0
		flag2 = 0;
		delay(1000);
		flag2 = 1;
		delay(1000);

//		taskYIELD();
	#else
		flag2 = 0;
		vTaskDelay(1);
		flag2 = 1;
		vTaskDelay(1);
	#endif
	}
}

void Task3_Entry( void *p_arg )
{
	while(1)
	{
		flag3 = 0;
		vTaskDelay(2);
		flag3 = 1;
		vTaskDelay(2);
	}
}

void prvIdleTask( void *p_arg )
{
	while(1)
	{
		
	}
}





