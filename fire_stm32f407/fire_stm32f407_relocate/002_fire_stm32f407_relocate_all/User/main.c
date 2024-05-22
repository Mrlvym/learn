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
#include "app_led.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int a = 8;
int b;

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

	App_Led_Test_Task();
	b = 0;
	while(1)
	{
		a++;
		
		b++;
	}
}

/*********************************************END OF FILE**********************/

