#ifndef __BSP_IAP_H__
#define __BSP_IAP_H__
#include "sys.h"

typedef void (*Bsp_Boot_Jump_Func)(void);

void Bsp_Per_DeInit(void);

void Bsp_SysTick_DeInit(void);

void Bsp_Clear_AllIRQ_Pending(void);

void Bsp_Jump_2_App(uint32_t app_addr);


#endif

