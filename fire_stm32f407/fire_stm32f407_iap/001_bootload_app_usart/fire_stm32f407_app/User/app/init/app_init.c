#include "app_init.h"
#include "sys.h"
#include "hal_delay.h"
#include "hal_led.h"
#include "hal_debug_usart.h"


void App_HW_Init(void)
{
	RCC_ClocksTypeDef RCC_Clocks;	
	
	#define NVIC_VTOR_MASK       0x3FFFFF80
	#define APP_PART_ADDR        0x08004000
	SCB->VTOR = APP_PART_ADDR & NVIC_VTOR_MASK;	

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


