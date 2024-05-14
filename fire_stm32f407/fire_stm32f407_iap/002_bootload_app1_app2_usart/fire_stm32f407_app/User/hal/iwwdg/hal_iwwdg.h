#ifndef __HAL_IWWDG_H__
#define __HAL_IWWDG_H__
#include "bsp_iwwdg.h"

void Hal_IWDG_Init(uint8_t pre, uint16_t count);

void Hal_IWDG_Feed(void);

uint8_t Hal_IWDG_GetRst_Flag(uint8_t flag_mask);

void Hal_IWDG_ClearRst_Flag(void);


void Hal_WWDG_Init(uint32_t pre, uint8_t count, uint8_t win_val);

void Hal_WWDG_Feed(uint8_t count);

void Hal_WWDG_NVIC_Config(void);

uint8_t Hal_WWDG_GetRst_Flag(uint8_t flag_mask);

void Hal_WWDG_ClearRst_Flag(void);

uint8_t Hal_WWDG_Get_Counter(void);

uint8_t Hal_WWDG_Get_WinVal(void);


#endif

