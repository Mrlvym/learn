#include "hal_iap.h"
#include "bsp_iap.h"


void Hal_Jump_2_App(uint32_t app_addr)
{
	Bsp_Jump_2_App(app_addr);
}



