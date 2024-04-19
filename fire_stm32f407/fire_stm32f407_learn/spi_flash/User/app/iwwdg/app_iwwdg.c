#include "app_iwwdg.h"
#include "hal_iwwdg.h"
#include "hal_debug_usart.h"
#include "hal_delay.h"
#include "hal_key.h"
#include "hal_led.h"

void App_IWDG_Test_Task(void)
{	
	Hal_Key_Init();

	if(Hal_IWDG_GetRst_Flag(HAL_RCC_FLAG_IWDGRST) == HAL_SET)
	{
		Hal_Debug(INFO_LEVEL, "发生了独立看门狗复位 ... \r\n");
		Hal_IWDG_ClearRst_Flag();
	}
	else
	{
		Hal_Debug(INFO_LEVEL, "正常复位 ... \r\n");
	}

	Hal_IWDG_Init(HAL_IWDG_Prescaler_64, 500);
	
	while(true)
	{
//		Hal_Debug(INFO_LEVEL, "This is main ... \r\n");
//		Hal_Delay_ms(500);
	}
	
}

void App_WWDG_Test_Task(void)
{
	uint8_t temp_win_val = 0;
	uint8_t temp_count_val = 0;

	Hal_Led_Init();

	if(Hal_WWDG_GetRst_Flag(HAL_RCC_FLAG_WWDGRST) == HAL_SET)
	{
		Hal_Debug(INFO_LEVEL, "发生了窗口看门狗复位 ... \r\n");
	}
	else
	{
		Hal_Debug(INFO_LEVEL, "正常复位 ... \r\n");
	}

	Hal_WWDG_Init(HAL_WWDG_Prescaler_8, 0x7F, 0x6F);

	temp_win_val = Hal_WWDG_Get_WinVal();

	while(true)
	{
		temp_count_val = Hal_WWDG_Get_Counter();
		if(temp_count_val < temp_win_val)
		{
//			Hal_WWDG_Feed(0x7F);			
		}
		else 
		{
//			Hal_Debug(INFO_LEVEL, "temp_win_val : %x, temp_count_val : %x ...\r\n", temp_win_val, temp_count_val);						
		}
	}
}

