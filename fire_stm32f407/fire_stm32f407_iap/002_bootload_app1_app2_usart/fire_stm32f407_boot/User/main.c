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
  
#include "app_init.h"
#include "app_iap.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
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

