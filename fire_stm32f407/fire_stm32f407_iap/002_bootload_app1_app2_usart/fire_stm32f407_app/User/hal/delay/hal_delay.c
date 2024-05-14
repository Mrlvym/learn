#include "hal_delay.h"
#include "bsp_delay.h"

void Hal_Delay_Init(uint8_t sysclk)
{
	Delay_Init(sysclk);
}

void Hal_Delay_us(uint32_t us)
{
	Delay_us(us);
}

void Hal_Delay_ms(uint32_t ms)
{
	Delay_ms(ms);	
}

