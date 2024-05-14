#ifndef __HAL_DELAY_H__
#define __HAL_DELAY_H__
#include "stdint.h"

void Hal_Delay_Init(uint8_t sysclk);
void Hal_Delay_us(uint32_t us);
void Hal_Delay_ms(uint32_t ms);


#endif


