#ifndef __HAL_IWWDG_H__
#define __HAL_IWWDG_H__
#include "stm32f4xx.h"
#include "bsp_iwwdg.h"


#define HAL_IWDG_Prescaler_4		BSP_IWDG_Prescaler_4
#define HAL_IWDG_Prescaler_8  	BSP_IWDG_Prescaler_8
#define HAL_IWDG_Prescaler_16 	BSP_IWDG_Prescaler_16
#define HAL_IWDG_Prescaler_32		BSP_IWDG_Prescaler_32
#define HAL_IWDG_Prescaler_64		BSP_IWDG_Prescaler_64
#define HAL_IWDG_Prescaler_128	BSP_IWDG_Prescaler_128
#define HAL_IWDG_Prescaler_256	BSP_IWDG_Prescaler_256

#define HAL_IS_IWDG_PRE(PRE)	((PRE == HAL_IWDG_Prescaler_4) || (PRE == HAL_IWDG_Prescaler_8) \
								 (PRE == HAL_IWDG_Prescaler_16) || (PRE == HAL_IWDG_Prescaler_32) \
								 (PRE == HAL_IWDG_Prescaler_64) || (PRE == HAL_IWDG_Prescaler_128) \
								 (PRE == HAL_IWDG_Prescaler_256))

#define HAL_RCC_FLAG_IWDGRST	BSP_RCC_FLAG_IWDGRST
#define HAL_RCC_FLAG_WWDGRST	BSP_RCC_FLAG_WWDGRST

#define HAL_IS_RCC_FLAG(FLAG)	((FLAG == HAL_RCC_FLAG_IWDGRST) || (FLAG == HAL_RCC_FLAG_WWDGRST))


#define HAL_WWDG_Prescaler_1	BSP_WWDG_Prescaler_1
#define HAL_WWDG_Prescaler_2	BSP_WWDG_Prescaler_2
#define HAL_WWDG_Prescaler_4	BSP_WWDG_Prescaler_4
#define HAL_WWDG_Prescaler_8	BSP_WWDG_Prescaler_8
#define HAL_IS_WWDG_PRE(PRE)	((PRE == HAL_WWDG_Prescaler_1) || (PRE == HAL_WWDG_Prescaler_2) \
								(PRE == HAL_WWDG_Prescaler_4) || (PRE == HAL_WWDG_Prescaler_8))

#define HAL_IS_COUNT_VAL(COUNT_VAL)	((COUNT_VAL >= 0x04) && (COUNT_VAL <= 0x7F))

void Hal_IWDG_Init(uint8_t pre, uint16_t count);

void Hal_IWDG_Feed(void);

uint8_t Hal_IWDG_GetRst_Flag(uint8_t flag_mask);

void Hal_IWDG_ClearRst_Flag(void);




void Hal_WWDG_Init(uint32_t pre, uint8_t count, uint8_t win_val);

void Hal_WWDG_Feed(uint8_t count);

uint8_t Hal_WWDG_GetRst_Flag(uint8_t flag_mask);

void Hal_WWDG_ClearRst_Flag(void);


uint8_t Hal_WWDG_Get_Counter(void);

uint8_t Hal_WWDG_Get_WinVal(void);

#endif


