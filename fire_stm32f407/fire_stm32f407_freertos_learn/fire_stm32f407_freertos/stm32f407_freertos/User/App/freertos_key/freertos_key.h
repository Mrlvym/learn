#ifndef __FREERTOS_KEY_H__
#define __FREERTOS_KEY_H__
#include "FreeRTOS.h"

//#define FREERTOS_USED_KEY_EXIT_IRQ

#ifdef FREERTOS_USED_KEY_EXIT_IRQ
#define FreeRTOS_Key0_IRQHandler	EXTI4_IRQHandler
#define FreeRTOS_Key2_IRQHandler	EXTI2_IRQHandler
#endif	


void FreeRTOS_Key_Init(void);

void FreeRTOS_Key_Task(void);

void Scan_Key_Sta_Exit(void);

void Scan_Key_Sta(void);

void Key_Sta_Add(void);

portCHAR Get_Key_Sta(void);

void Set_Key_Sta(portCHAR sta);


#endif


