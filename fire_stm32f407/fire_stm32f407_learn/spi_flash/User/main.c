/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   用1.8.0版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "app_init.h"
#include "app_led.h"
#include "app_spi_flash.h"
#include "app_rtc.h"
#include "app_iwwdg.h"
#include "app_sdio.h"
#include "app_fatfs.h"
#include "app_sram.h"
#include "app_lcd_task.h"
#include "app_touch_task.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */

void MCO_Test(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);
	RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_2);	
}

int main(void)
{
		/* 程序来到main函数之前，启动文件：statup_stm32f4xx.s已经调用
		* SystemInit()函数把系统时钟初始化成168MHZ
		* SystemInit()在system_stm32f4xx.c中定义
		* 如果用户想修改系统时钟，可自行编写程序修改
		*/
	/* add your code here ^_^. */
	App_HW_Init();

	App_SW_Init();

//	App_Rtc_DateTimeAlarm_Test_Task();
//	App_IWDG_Test_Task();
//	App_WWDG_Test_Task();
//	App_SD_Test_Task();
//	App_Fatfs_Test_Task();
//	APP_Sram_Task();
//	App_LCD_Task();

	App_Touch_Task();

	while(1)
	{

	}
}

/*********************************************END OF FILE**********************/

