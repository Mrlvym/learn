#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"
#include "stddef.h"


/* ���������ض��� */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	uint32_t
#define portBASE_TYPE	long

//�� �� �� �� �� �� �� �� �� �� �� �� �� ��
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
	#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#endif

#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1

	/* ������ȼ����� */
	#if( configMAX_PRIORITIES > 32 )
		#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
	#endif

	/* �������ȼ�����/������ȼ�λͼ����Ӧ��λ */
	#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
	#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )

	/*-----------------------------------------------------------*/

	#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31UL - ( uint32_t ) __clz( ( uxReadyPriorities ) ) )

#endif /* taskRECORD_READY_PRIORITY */


typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif

/* �жϿ���״̬�Ĵ�����0xe000ed04
 * Bit 28 PENDSVSET: PendSV ����λ
 */
#define portNVIC_INT_CTRL_REG		( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT		( 1UL << 28UL )

#define portSY_FULL_READ_WRITE		( 15 )

#define portYIELD()																\
{																				\
	/* ����PendSV�������������л� */								                \
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;								\
	__dsb( portSY_FULL_READ_WRITE );											\
	__isb( portSY_FULL_READ_WRITE );											\
}

/* ��������ֵ�Ĺ��жϺ���������Ƕ�ף��������ж�����ʹ�� */ 
#define portDISABLE_INTERRUPTS() vPortRaiseBASEPRI()

static void vPortRaiseBASEPRI( void )
{
	uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY; 

	__asm
	{
		msr basepri, ulNewBASEPRI 
		dsb
		isb
	}
}

/* ������ֵ�Ĺ��жϺ���������Ƕ�ף��������ж�����ʹ�� */
#define portSET_INTERRUPT_MASK_FROM_ISR() ulPortRaiseBASEPRI()
static uint32_t ulPortRaiseBASEPRI( void )
{
	uint32_t ulReturn, ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;

	__asm
	{
		mrs ulReturn, basepri
		msr basepri, ulNewBASEPRI 
		dsb
		isb
	}

	return ulReturn;
}

/* �����жϱ����Ŀ��жϺ��� */
#define portENABLE_INTERRUPTS() vPortSetBASEPRI( 0 )

/* ���жϱ����Ŀ��жϺ��� */
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) vPortSetBASEPRI(x)


static void vPortSetBASEPRI( uint32_t ulBASEPRI )
{
	__asm
	{
		msr basepri, ulBASEPRI 
	}
}
void vPortEnterCritical( void );
void vPortExitCritical( void );

//�����ٽ��
#define portENTER_CRITICAL()					vPortEnterCritical()
#define portEXIT_CRITICAL()						vPortExitCritical()



#endif /* PORTMACRO_H */
