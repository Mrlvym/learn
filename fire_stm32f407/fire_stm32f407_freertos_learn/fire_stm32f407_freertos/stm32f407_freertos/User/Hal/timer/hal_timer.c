#include "hal_timer.h"

#include "bsp_timer.h"


void Hal_TIMx_Cfg(void)
{
	Bsp_TIMx_Init();

	Bsp_TIMx_NVIC_Cfg();
}
