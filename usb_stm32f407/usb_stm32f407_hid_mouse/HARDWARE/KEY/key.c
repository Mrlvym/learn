#include "key.h"
#include "delay.h" 
#include "sys.h"
#include "led.h"

#include "usbd_hid_mouse.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//����
	if(WK_UP==1)	 
	{
//		BEEP=!BEEP; //��������ת 
		Set_Key_Sta(KEY_UP);
	}	
	else
	{
		Set_Key_Sta(KEY_NUL);
	}
	 EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
}	
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//����
	if(KEY2==0)	  
	{				 
//   LED0=!LED0; 
		Set_Key_Sta(KEY_LEFT);
	}	
	else
	{
		Set_Key_Sta(KEY_NUL);
	}
	 EXTI_ClearITPendingBit(EXTI_Line2);//���LINE2�ϵ��жϱ�־λ 
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	delay_ms(10);	//����
	if(KEY1==0)	 
	{
//		LED1=!LED1;
		Set_Key_Sta(KEY_DOWN);
	}	
	else
	{
		Set_Key_Sta(KEY_NUL);
	}
	 EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
}
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{
	delay_ms(10);	//����
	if(KEY0==0)	 
	{				 
//		LED0=!LED0;	
//		LED1=!LED1;	
		Set_Key_Sta(KEY_RIGHT);
	}		 
	else
	{
		Set_Key_Sta(KEY_NUL);
	}
	 EXTI_ClearITPendingBit(EXTI_Line4);//���LINE4�ϵ��жϱ�־λ  
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line6) == SET)
	{
		delay_ms(10);	//����
		
		if(KEY_CLICK_L == 0)
		{
			Set_Key_Click_Sta(KEY_CLICK_LEFT);
		}
		else
		{
			Set_Key_Click_Sta(KEY_NUL);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line6);//���LINE6�ϵ��жϱ�־λ  
	}
	else if(EXTI_GetFlagStatus(EXTI_Line7) == SET)
	{
		delay_ms(10);	//����
		
		if(KEY_CLICK_R == 0)
		{
			Set_Key_Click_Sta(KEY_CLICK_RIGHT);
		}
		else
		{
			Set_Key_Click_Sta(KEY_NUL);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line7);//���LINE7�ϵ��жϱ�־λ  
	}
	else if(EXTI_GetFlagStatus(EXTI_Line8) == SET)
	{
		delay_ms(10);	//����
		
		if(KEY_CLICK_M == 0)
		{
			Set_Key_Click_Sta(KEY_CLICK_MIDDLE);
		}
		else
		{
			Set_Key_Click_Sta(KEY_NUL);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line8);//���LINE8�ϵ��жϱ�־λ  
	}
	else if(EXTI_GetFlagStatus(EXTI_Line9) == SET)
	{
		delay_ms(10);	//����
		
		if(KEY_CLICK_RL == 0)
		{
			Set_Key_Click_Sta(KEY_CLICK_ROLL);
		}
		else
		{
			Set_Key_Click_Sta(KEY_NUL);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line9);//���LINE9�ϵ��жϱ�־λ  
	}
	
}
	   
//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //������Ӧ��IO�ڳ�ʼ��
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 ���ӵ��ж���3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 ���ӵ��ж���4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 ���ӵ��ж���0

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);//PB6 ���ӵ��ж���6
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource7);//PB7 ���ӵ��ж���7
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);//PB8 ���ӵ��ж���8
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);//PB9 ���ӵ��ж���9
	
  /* ����EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
	/* ����EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����

  /* ����EXTI_Line6,7,8,9 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//�ⲿ�ж�4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����

  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	   
}

//������ʼ������
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
 
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3;
		else if(WK_UP==1)return 4;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}




















