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

/* 定义 uxTopReadyPriority，在 task.c 中定义 */
static volatile UBaseType_t uxTopReadyPriority = tskIDLE_PRIORITY;

static volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;

static List_t xDelayedTaskList1;
static List_t xDelayedTaskList2;
static List_t * volatile pxDelayedTaskList;
static List_t * volatile pxOverflowDelayedTaskList;

static volatile TickType_t xNextTaskUnblockTime		= ( TickType_t ) 0U;
static volatile BaseType_t xNumOfOverflows 			= ( BaseType_t ) 0;

/* 查找最高优先级的就绪任务：通用方法 */
#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 0 )
	/* uxTopReadyPriority 存的是就绪任务的最高优先级 */
//	#define taskRECORD_READY_PRIORITY( uxPriority )
//	{
//		if( ( uxPriority ) > uxTopReadyPriority )
//		{
//			uxTopReadyPriority = uxPriority;
//		}
//	}
/* uxTopReadyPriority 存的是就绪任务的最高优先级 */
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
//		/* 寻找包含就绪任务的最高优先级的队列 */\
//		while(listLIST_IS_EMPTY(&pxReadyTasksLists[uxTopPriority]))
//		{
//			--uxTopPriority;
//		}
//		/* 获取优先级最高的就绪任务的 TCB，然后更新到 pxCurrentTCB */\
//		listGET_OWNER_OF_NEXT_ENTRY(pxCurrentTCB, &pxReadyTasksLists[uxTopPriority]);
//		/* 更新 uxTopReadyPriority */\
//		uxTopReadyPriority = uxTopPriority;
//	}
	
	#define taskSELECT_HIGHEST_PRIORITY_TASK()															\
	{																									\
		UBaseType_t uxTopPriority = uxTopReadyPriority;														\
																										\
		/* 寻找包含就绪任务的最高优先级的队列 */                                                          \
		while( listLIST_IS_EMPTY( &( pxReadyTasksLists[ uxTopPriority ] ) ) )							\
		{																								\
			--uxTopPriority;																			\
		}																								\
																										\
		/* 获取优先级最高的就绪任务的TCB，然后更新到pxCurrentTCB */							            \
		listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists[ uxTopPriority ] ) );			\
		/* 更新uxTopReadyPriority */                                                                    \
		uxTopReadyPriority = uxTopPriority;																\
	} /* taskSELECT_HIGHEST_PRIORITY_TASK */
#else
	#define taskRECORD_READY_PRIORITY( uxPriority )	portRECORD_READY_PRIORITY( uxPriority, uxTopReadyPriority )

	/*-----------------------------------------------------------*/

	#define taskSELECT_HIGHEST_PRIORITY_TASK()														    \
	{																								    \
	UBaseType_t uxTopPriority;																		    \
																									    \
		/* 寻找最高优先级 */								                            \
		portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );								    \
		/* 获取优先级最高的就绪任务的TCB，然后更新到pxCurrentTCB */                                       \
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

/* 将任务添加到就绪列表 */                                    
#define prvAddTaskToReadyList( pxTCB )																   \
	taskRECORD_READY_PRIORITY( ( pxTCB )->uxPriority );												   \
	vListInsertEnd( &( pxReadyTasksLists[ ( pxTCB )->uxPriority ] ), &( ( pxTCB )->xStateListItem ) ); \


#if 0
/*
*************************************************************************
*                               任务控制块
*************************************************************************
*/

/* 当前正在运行的任务的任务控制块指针，默认初始化为NULL */
TCB_t * volatile pxCurrentTCB = NULL;

/* 任务就绪列表 */
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];


static volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;


/*
*************************************************************************
*                               函数声明
*************************************************************************
*/

static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* 任务入口 */
									const char * const pcName,              /* 任务名称，字符串形式 */
									const uint32_t ulStackDepth,            /* 任务栈大小，单位为字 */
									void * const pvParameters,              /* 任务形参 */
									TaskHandle_t * const pxCreatedTask,     /* 任务句柄 */
									TCB_t *pxNewTCB );


/*
*************************************************************************
*                               宏定义
*************************************************************************
*/
                                    

                                   

/*
*************************************************************************
*                               静态任务创建函数
*************************************************************************
*/
#if( configSUPPORT_STATIC_ALLOCATION == 1 )

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           /* 任务入口 */
					            const char * const pcName,           /* 任务名称，字符串形式 */
					            const uint32_t ulStackDepth,         /* 任务栈大小，单位为字 */
					            void * const pvParameters,           /* 任务形参 */
					            StackType_t * const puxStackBuffer,  /* 任务栈起始地址 */
					            TCB_t * const pxTaskBuffer )         /* 任务控制块指针 */
{
	TCB_t *pxNewTCB;
	TaskHandle_t xReturn;

	if( ( pxTaskBuffer != NULL ) && ( puxStackBuffer != NULL ) )
	{		
		pxNewTCB = ( TCB_t * ) pxTaskBuffer; 
		pxNewTCB->pxStack = ( StackType_t * ) puxStackBuffer;

		/* 创建新的任务 */
		prvInitialiseNewTask( pxTaskCode,        /* 任务入口 */
                              pcName,            /* 任务名称，字符串形式 */
                              ulStackDepth,      /* 任务栈大小，单位为字 */ 
                              pvParameters,      /* 任务形参 */
                              &xReturn,          /* 任务句柄 */ 
                              pxNewTCB);         /* 任务栈起始地址 */      

	}
	else
	{
		xReturn = NULL;
	}

	/* 返回任务句柄，如果任务创建成功，此时xReturn应该指向任务控制块 */
    return xReturn;
}

#endif /* configSUPPORT_STATIC_ALLOCATION */


static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* 任务入口 */
									const char * const pcName,              /* 任务名称，字符串形式 */
									const uint32_t ulStackDepth,            /* 任务栈大小，单位为字 */
									void * const pvParameters,              /* 任务形参 */
									TaskHandle_t * const pxCreatedTask,     /* 任务句柄 */
									TCB_t *pxNewTCB )                       /* 任务控制块指针 */

{
	StackType_t *pxTopOfStack;
	UBaseType_t x;	
	
	/* 获取栈顶地址 */
	pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - ( uint32_t ) 1 );
	//pxTopOfStack = ( StackType_t * ) ( ( ( portPOINTER_SIZE_TYPE ) pxTopOfStack ) & ( ~( ( portPOINTER_SIZE_TYPE ) portBYTE_ALIGNMENT_MASK ) ) );
	/* 向下做8字节对齐 */
	pxTopOfStack = ( StackType_t * ) ( ( ( uint32_t ) pxTopOfStack ) & ( ~( ( uint32_t ) 0x0007 ) ) );	

	/* 将任务的名字存储在TCB中 */
	for( x = ( UBaseType_t ) 0; x < ( UBaseType_t ) configMAX_TASK_NAME_LEN; x++ )
	{
		pxNewTCB->pcTaskName[ x ] = pcName[ x ];

		if( pcName[ x ] == 0x00 )
		{
			break;
		}
	}
	/* 任务名字的长度不能超过configMAX_TASK_NAME_LEN */
	pxNewTCB->pcTaskName[ configMAX_TASK_NAME_LEN - 1 ] = '\0';

    /* 初始化TCB中的xStateListItem节点 */
    vListInitialiseItem( &( pxNewTCB->xStateListItem ) );
    /* 设置xStateListItem节点的拥有者 */
	listSET_LIST_ITEM_OWNER( &( pxNewTCB->xStateListItem ), pxNewTCB );
    
    
    /* 初始化任务栈 */
	pxNewTCB->pxTopOfStack = (StackType_t *)pxPortInitialiseStack( pxTopOfStack, pxTaskCode, pvParameters );   


	/* 让任务句柄指向任务控制块 */
    if( ( void * ) pxCreatedTask != NULL )
	{		
		*pxCreatedTask = ( TaskHandle_t ) pxNewTCB;
	}
}

/* 初始化任务相关的列表 */
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
    /* 手动指定第一个运行的任务 */
    pxCurrentTCB = &Task1TCB;
    
    /* 启动调度器 */
    if( xPortStartScheduler() != pdFALSE )
    {
        /* 调度器启动成功，则不会返回，即不会来到这里 */
    }
}

void vTaskSwitchContext( void )
{    
    /* 两个任务轮流切换 */
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

/* 任务就绪列表 */
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



static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* 任务入口 */
									const char * const pcName,              /* 任务名称，字符串形式 */
									const uint32_t ulStackDepth,            /* 任务栈大小，单位为字 */
									void * const pvParameters,              /* 任务形参 */
									UBaseType_t uxPriority,                 /* 任务优先级，数值越大，优先级越高 */
									TaskHandle_t * const pxCreatedTask,     /* 任务句柄 */
									TCB_t *pxNewTCB )                       /* 任务控制块指针 */
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

	/* 初始化优先级 */
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
//	/* 两个任务轮流切换 */
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

	/* 获取当前任务的 TCB */
	pxTCB = pxCurrentTCB; 

//	/* 设置延时时间 */
//	pxTCB->xTicksToDelay = xTicksToDelay;

//	taskRESET_READY_PRIORITY(pxTCB->uxPriority);

	prvAddCurrentTaskToDelayedList(xTicksToDelay);


	/* 任务切换 */
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

	//溢出 ，切换延时列表
	if(xConstTickCount == (TickType_t)0U)
	{
		//切换延时列表
		List_t *pxTemp;
		pxTemp = pxDelayedTaskList;
		pxDelayedTaskList = pxOverflowDelayedTaskList;
		pxOverflowDelayedTaskList = pxTemp;
		xNumOfOverflows++;//溢出次数计数
		//初始化新列表xNextTaskUnblockTime值
		prvResetNextTaskUnblockTime();
	}

	//有延时任务到时
	if(xConstTickCount >= xNextTaskUnblockTime)
	{
	
		while(1)
		{
			if(listLIST_IS_EMPTY(pxDelayedTaskList) != pdFALSE)//延时列表为空，初始化为portMAX_DELAY
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
				uxListRemove(&(pxTCB->xStateListItem));//从延时列表删除

				//添加到就绪列表
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

	/* 如果xConstTickCount溢出，则切换延时列表 */
	if( xConstTickCount == ( TickType_t ) 0U )
	{
		taskSWITCH_DELAYED_LISTS();
	}

	/* 最近的延时任务延时到期 */
	if( xConstTickCount >= xNextTaskUnblockTime )
	{
		for( ;; )
		{
			if( listLIST_IS_EMPTY( pxDelayedTaskList ) != pdFALSE )
			{
				/* 延时列表为空，设置xNextTaskUnblockTime为可能的最大值 */
				xNextTaskUnblockTime = portMAX_DELAY;
				break;
			}
			else /* 延时列表不为空 */
			{
				pxTCB = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxDelayedTaskList );
				xItemValue = listGET_LIST_ITEM_VALUE( &( pxTCB->xStateListItem ) );

				/* 直到将延时列表中所有延时到期的任务移除才跳出for循环 */
                if( xConstTickCount < xItemValue )
				{
					xNextTaskUnblockTime = xItemValue;
					break;
				}

				/* 将任务从延时列表移除，消除等待状态 */
				( void ) uxListRemove( &( pxTCB->xStateListItem ) );

				/* 将解除等待的任务添加到就绪列表 */
				prvAddTaskToReadyList( pxTCB );
			}
		}
	}/* xConstTickCount >= xNextTaskUnblockTime */
    
    /* 任务切换 */
    portYIELD();
}
#endif
static void prvResetNextTaskUnblockTime( void )
{
	TCB_t *pxTCB;

	if(listLIST_IS_EMPTY(pxDelayedTaskList) != pdFALSE)//延时列表为空，初始化为portMAX_DELAY
	{
		xNextTaskUnblockTime = portMAX_DELAY;
	}
	else//不为空，初始化为延时列表的第一个元素的pxTCB->xStateListItem.xItemValue
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
	
	/* 获取系统时基计数器 xTickCount 的值 */
	const TickType_t xConstTickCount = xTickCount;

	/* 将任务从就绪列表中移除 */ 
	if ( uxListRemove( &( pxCurrentTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
	{
		/* 将任务在优先级位图中对应的位清除 */
		portRESET_READY_PRIORITY(pxCurrentTCB->uxPriority, uxTopReadyPriority);
	}

	/* 计算任务延时到期时，系统时基计数器 xTickCount 的值是多少 */
	xTimeToWake = xConstTickCount + xTicksToWait;

	listSET_LIST_ITEM_VALUE(&(pxCurrentTCB->xStateListItem), xTimeToWake);

	//溢出
	if(xTimeToWake < xConstTickCount)
	{
		vListInsert(pxOverflowDelayedTaskList, &(pxCurrentTCB->xStateListItem));
	}
	else//未溢出
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
    
    /* 获取系统时基计数器xTickCount的值 */
    const TickType_t xConstTickCount = xTickCount;

    /* 将任务从就绪列表中移除 */
	if( uxListRemove( &( pxCurrentTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
	{
		/* 将任务在优先级位图中对应的位清除 */
        portRESET_READY_PRIORITY( pxCurrentTCB->uxPriority, uxTopReadyPriority );
	}

    /* 计算延时到期时，系统时基计数器xTickCount的值是多少 */
    xTimeToWake = xConstTickCount + xTicksToWait;

    /* 将延时到期的值设置为节点的排序值 */
    listSET_LIST_ITEM_VALUE( &( pxCurrentTCB->xStateListItem ), xTimeToWake );

    /* 溢出 */
    if( xTimeToWake < xConstTickCount )
    {
        vListInsert( pxOverflowDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );
    }
    else /* 没有溢出 */
    {

        vListInsert( pxDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );

        /* 更新下一个任务解锁时刻变量xNextTaskUnblockTime的值 */
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





