#include "app_iap.h"

#include "hal_iap.h"
#include "hal_flash.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_delay.h"
#include "hal_debug_usart.h"


uint8_t App_Buf[APP_BUF_SIZE] = {0};

void App_IAP_Bsp_Init(void)
{
	Hal_Led_Init();

	Hal_Key_Init();
}


void App_IAP_Task(void)
{
	uint8_t i = 0;
	
	App_IAP_Bsp_Init();

	if(!Hal_Key0_Read_Bit()){

		App_IAP_Show_BootInfo();
				
		Hal_Debug(PRINTF, "Please release Key0 before the countdown ends  ...\r\n");
		for(i = COUNTDOWN_SECS; i > 0; i--)
		{
			Hal_Debug(PRINTF, "Countdown is %d s ... \r\n", i);
			Hal_Delay_ms(1000);
			if(Hal_Key0_Read_Bit()){
				break;
			}
		}
		
		if(i != 0)//倒计时结束前松开key0
		{
			memset(App_Buf, 0, APP_BUF_SIZE);
			Hal_Debug(PRINTF, "Please send App_File through  Usart and enter Key0 to update App_File ...\r\n");

			while(Hal_Key0_Read_Bit());
			
			Hal_Debug(PRINTF, "Erase App ...\r\n");
			Hal_Earse_Flash_Size(APP_START_ADDR, APP_BUF_SIZE);
			Hal_Debug(PRINTF, "Erase Done addr(0x%08x), size(0x%08x) ...\r\n", APP_START_ADDR, APP_BUF_SIZE);

			Hal_Debug(PRINTF, "Write App ...\r\n");
			Hal_Flash_Write_Len(APP_START_ADDR, App_Buf, APP_BUF_SIZE);
			Hal_Debug(PRINTF, "Write Done addr(0x%08x), size(0x%08x) ...\r\n", APP_START_ADDR, APP_BUF_SIZE);
		}
	
		
	}
	
	Hal_Debug(PRINTF, "Boot App Start ...\r\n\r\n");
	Hal_Jump_2_App(APP_START_ADDR);
	
	while(1);
}

void App_IAP_Show_BootInfo(void)
{
	Hal_Debug(PRINTF, "---------- Enter BootLoader ----------\r\n");
    Hal_Debug(PRINTF, "\r\n");
    Hal_Debug(PRINTF, "======== flash pration table =========\r\n");
    Hal_Debug(PRINTF, "| name     | offset     | size       |\r\n");
    Hal_Debug(PRINTF, "--------------------------------------\r\n");
    Hal_Debug(PRINTF, "| boot     | 0x%08X | 0x%08X |\r\n", BOOT_START_ADDR, BOOT_FLASH_SIZE);
    Hal_Debug(PRINTF, "| app      | 0x%08X | 0x%08X |\r\n", APP_START_ADDR, APP_FLASH_SIZE);
    Hal_Debug(PRINTF, "======================================\r\n");
    Hal_Debug(PRINTF, "\r\n");
}




