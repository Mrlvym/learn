#ifndef __BSP_TOUCH_GT911_H__
#define __BSP_TOUCH_GT911_H__
#include "sys.h"


//I2C��д����
//#define GT911_WR_CMD_0X28
#define GT911_WR_CMD_0XBA

#if (defined(GT911_WR_CMD_0X28))
#define BSP_TOUCH_GT911_WR_CMD	0x28
#define BSP_TOUCH_GT911_RD_CMD	0x29
#elif (defined(GT911_WR_CMD_0XBA))
#define BSP_TOUCH_GT911_WR_CMD	0xBA
#define BSP_TOUCH_GT911_RD_CMD	0xBB
#endif

#define	BSP_TOUCH_GT911_RST_CLK		RCC_AHB1Periph_GPIOC
#define	BSP_TOUCH_GT911_INT_CLK		RCC_AHB1Periph_GPIOB

#define	BSP_TOUCH_GT911_RST_PORT	GPIOC
#define	BSP_TOUCH_GT911_INT_PORT	GPIOB

#define	BSP_TOUCH_GT911_RST_PIN		GPIO_Pin_13
#define	BSP_TOUCH_GT911_INT_PIN		GPIO_Pin_1

#define BSP_TOUCH_GT911_INT_EXTI_PORTSOURCE	EXTI_PortSourceGPIOB
#define BSP_TOUCH_GT911_INT_EXTI_PINSOURCE  EXTI_PinSource1
#define BSP_TOUCH_GT911_INT_EXTI_LINE		EXTI_Line1
#define BSP_TOUCH_GT911_INT_IRQCHANNEL		EXTI1_IRQn
#define BSP_TOUCH_GT911_INT_PRE_PRIORITY	0x02
#define BSP_TOUCH_GT911_INT_SUB_PRIORITY	0x02

//IO��������	 
#define BSP_TOUCH_GT911_RST    		PCout(13)	//GT911��λ����
#define BSP_TOUCH_GT911_INT    		PBout(1)	//GT911�ж�����	


 /*
  * Bsp_Touch_Gt911_RD_Reg:��ȡgt911�Ĵ����ӿ�
  *	������
  *		reg:��ʼ�Ĵ�����ַ
  *		buf:���ݻ�������
  *		len:�����ݳ���
  *	����ֵ��
  *	  	void
  */ 
void Bsp_Touch_Gt911_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

/*
  * Bsp_Touch_Gt911_WR_Reg:д��gt911�Ĵ����ӿ�
  *	������
  *		reg:��ʼ�Ĵ�����ַ
  *		buf:���ݻ�������
  *		len:�����ݳ���
  *	����ֵ��
  *		0:ACK�ɹ�
  *		1:ACKʧ��
  */
uint8_t Bsp_Touch_Gt911_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

 /*
  *Bsp_Touch_Gt911_Rst_Pin_Init: Gt911 Rst���ų�ʼ��
  *������
  *		void
  *����ֵ��
  *		void
  */
void Bsp_Touch_Gt911_Rst_Pin_Init(void);
 
 /*
  *Bsp_Touch_Gt911_Int_Pin_Init: Gt911 Int���ų�ʼ��
  *������
  * 	void
  *����ֵ��
  * 	void
  */
 void Bsp_Touch_Gt911_Int_Pin_Init(void);
 
 /*
 * Bsp_Touch_Gt911_PowerOn:Gt911�ϵ�ʱ��
 * ������
 *	   void
 * ����ֵ��
 *	   void
 */
 void Bsp_Touch_Gt911_PowerOn(void);


 /*
 * Bsp_Touch_Gt911_Int_Irq_Cfg:��ʼ��Gt911 Int ����Ϊ�ж�ģʽ
 * ������
 *	   void
 * ����ֵ��
 *	   void
 */
void Bsp_Touch_Gt911_Int_Irq_Cfg(void);

 /*
  *Bsp_Touch_Gt911_ReadID: ��ȡGt911 ID
  *������
  * 	void
  *����ֵ��
  * 	0����ȡ�ɹ�
  * 	1����ȡʧ��
  */
 uint8_t Bsp_Touch_Gt911_ReadID(void);


#endif

