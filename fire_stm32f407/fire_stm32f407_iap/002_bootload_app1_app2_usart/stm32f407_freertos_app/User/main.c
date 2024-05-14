/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ƶ����ˮ��
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
#include "freertos_led.h"


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	
	App_FreeRTOS_BSP_Init();

	App_FreeRTOS_SW_Init();
	
	App_FreeRTOS_Task_Init();
	
	while(1)
	{
		
	}
}


