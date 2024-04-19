#ifndef __HAL_TOUCH_GT911_H__
#define __HAL_TOUCH_GT911_H__
#include "sys.h"
 /*
  * Hal_Touch_Gt911_RD_Reg:��ȡgt911�Ĵ����ӿ�
  *	������
  *		reg:��ʼ�Ĵ�����ַ
  *		buf:���ݻ�������
  *		len:�����ݳ���
  *	����ֵ��
  *	  	void
  */ 
void Hal_Touch_Gt911_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

/*
  * Hal_Touch_Gt911_WR_Reg:д��gt911�Ĵ����ӿ�
  *	������
  *		reg:��ʼ�Ĵ�����ַ
  *		buf:���ݻ�������
  *		len:�����ݳ���
  *	����ֵ��
  *		0:ACK�ɹ�
  *		1:ACKʧ��
  */
uint8_t Hal_Touch_Gt911_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

 /*
 * Hal_Touch_Gt911_Int_Irq_Cfg:��ʼ��Gt911 Int ����Ϊ�ж�ģʽ
 * ������
 *	   void
 * ����ֵ��
 *	   void
 */
void Hal_Touch_Gt911_Int_Irq_Cfg(void);


/*
 * Hal_Touch_Gt911_Init: gt911��ʼ��
 * ������
 *	   void
 * ����ֵ��
 *	   void
 */
void Hal_Touch_Gt911_Init(void);

#endif

