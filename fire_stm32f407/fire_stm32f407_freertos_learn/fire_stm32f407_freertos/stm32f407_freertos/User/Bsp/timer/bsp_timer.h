#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "stm32f4xx.h"

#define BASIC_TIM           		TIM6
#define BASIC_TIM_CLK       		RCC_APB1Periph_TIM6

#define BASIC_TIM_IRQn				TIM6_DAC_IRQn
#define BASIC_TIM_IRQHandler    	TIM6_DAC_IRQHandler


void Bsp_TIMx_NVIC_Cfg(void);

void Bsp_TIMx_Init(void);

#endif


