/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��1.8.0�汾�⽨�Ĺ���ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
  * @brief  ������
  * @param  ��
  * @retval ��
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
		/* ��������main����֮ǰ�������ļ���statup_stm32f4xx.s�Ѿ�����
		* SystemInit()������ϵͳʱ�ӳ�ʼ����168MHZ
		* SystemInit()��system_stm32f4xx.c�ж���
		* ����û����޸�ϵͳʱ�ӣ������б�д�����޸�
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

