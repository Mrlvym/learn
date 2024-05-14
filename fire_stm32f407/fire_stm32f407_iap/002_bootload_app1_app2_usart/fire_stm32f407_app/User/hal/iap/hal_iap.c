#include "hal_iap.h"
#include "bsp_iap.h"

void Hal_NVIC_SystemReset(void)
{
	Bsp_NVIC_SystemReset();
}

