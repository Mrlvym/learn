#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__
#include "stdio.h"
#include "stm32f4xx.h"

void Delay_Init(uint8_t SYSCLK);
void Delay_us(uint32_t nus);
void Delay_ms(uint32_t nms);


#endif



