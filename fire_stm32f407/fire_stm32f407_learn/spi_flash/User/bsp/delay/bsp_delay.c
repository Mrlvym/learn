#include "bsp_delay.h"

static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;

void Delay_Init(uint8_t SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK / 8;

	fac_ms = fac_us * 1000;
}

void Delay_us(uint32_t nus)
{
	uint32_t temp = 0;
	
	if(nus > (16777216 / 21) - 1)
	{
		printf("nus is too large ... \r\n");
	}

	SysTick->LOAD = nus * fac_us;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp = SysTick->CTRL;
	}
	while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD = 0x00;
}


void Delay_ms(uint32_t nms)
{
	while(nms--)
	{
		Delay_us(1000);
	}
}

