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
  
#include "app_init.h"
#include "app_iap.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */

int main(void)
{

	/* add your code here ^_^. */
	App_HW_Init();

	App_SW_Init();

	App_IAP_Task();

	while(1)
	{

	}
}

/*********************************************END OF FILE**********************/

