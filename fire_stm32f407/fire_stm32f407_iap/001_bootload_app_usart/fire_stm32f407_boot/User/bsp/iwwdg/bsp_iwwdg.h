#ifndef __BSP_IWWDG_H__
#define __BSP_IWWDG_H__
#include "stm32f4xx.h"

#define BSP_IWDG_WriteAccess_Enable		IWDG_WriteAccess_Enable
#define BSP_IWDG_WriteAccess_Disable	IWDG_WriteAccess_Disable

#define BSP_IWDG_Prescaler_4		IWDG_Prescaler_4
#define BSP_IWDG_Prescaler_8  	IWDG_Prescaler_8
#define BSP_IWDG_Prescaler_16 	IWDG_Prescaler_16
#define BSP_IWDG_Prescaler_32 	IWDG_Prescaler_32
#define BSP_IWDG_Prescaler_64 	IWDG_Prescaler_64
#define BSP_IWDG_Prescaler_128	IWDG_Prescaler_128
#define BSP_IWDG_Prescaler_256	IWDG_Prescaler_256

#define BSP_RCC_FLAG_IWDGRST	RCC_FLAG_IWDGRST
#define BSP_RCC_FLAG_WWDGRST	RCC_FLAG_WWDGRST

#define BSP_WWDG_Prescaler_1	WWDG_Prescaler_1
#define BSP_WWDG_Prescaler_2	WWDG_Prescaler_2
#define BSP_WWDG_Prescaler_4	WWDG_Prescaler_4
#define BSP_WWDG_Prescaler_8	WWDG_Prescaler_8


void Bsp_IWDG_Init(uint8_t pre, uint16_t count);

void Bsp_IWDG_Feed(void);

uint8_t Bsp_IWDG_GetRst_Flag(uint8_t flag_mask);

void Bsp_IWDG_ClearRst_Flag(void);


void Bsp_WWDG_Init(uint32_t pre, uint8_t count, uint8_t win_val);

void Bsp_WWDG_Feed(uint8_t count);

void Bsp_WWDG_NVIC_Config(void);

uint8_t Bsp_WWDG_GetRst_Flag(uint8_t flag_mask);

void Bsp_WWDG_ClearRst_Flag(void);

uint8_t Bsp_WWDG_Get_Counter(void);

uint8_t Bsp_WWDG_Get_WinVal(void);


#endif

