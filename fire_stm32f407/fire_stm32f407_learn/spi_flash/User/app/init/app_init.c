#include "app_init.h"
#include "hal_delay.h"
#include "hal_led.h"
#include "hal_spi_flash.h"
#include "hal_debug_usart.h"
#include "hal_rtc.h"
#include "hal_key.h"


void App_HW_Init(void)
{
	RCC_ClocksTypeDef RCC_Clocks;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	Hal_Delay_Init(168);
	
	Hal_Debug_Usart_Config();
	
	RCC_GetClocksFreq(&RCC_Clocks);
	Hal_Debug(DEBUG_LEVEL, "sysclk : %d, hclk : %d, pclk1 : %d, pclk2 : %d \r\n", \
						  RCC_Clocks.SYSCLK_Frequency,\
						  RCC_Clocks.HCLK_Frequency,\
						  RCC_Clocks.PCLK1_Frequency,\
						  RCC_Clocks.PCLK2_Frequency);
}


void App_SW_Init(void)
{
	
}


