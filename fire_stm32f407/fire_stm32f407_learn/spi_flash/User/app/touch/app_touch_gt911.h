#ifndef __APP_TOUCH_GT911_H__
#define __APP_TOUCH_GT911_H__
#include "stdint.h"

#define APP_GT911_READ_XY_REG 				0x814E	/* ����Ĵ��� */
#define APP_GT911_CLEARBUF_REG 				0x814E	/* �������Ĵ��� */
#define APP_GT911_READ_POINT1_REG			0x814F/* ����1�Ĵ��� */
#define APP_GT911_READ_POINT2_REG			0x8157/* ����2�Ĵ��� */
#define APP_GT911_READ_POINT3_REG			0x815F/* ����3�Ĵ��� */
#define APP_GT911_READ_POINT4_REG			0x8167/* ����4�Ĵ��� */
#define APP_GT911_READ_POINT5_REG			0x816F/* ����5�Ĵ��� */
#define APP_GT911_CONFIG_REG				0x8047	/* ���ò����Ĵ��� */
#define APP_GT911_COMMAND_REG   			0x8040  /* ʵʱ���� */
#define APP_GT911_PRODUCT_ID_REG 			0x8140 /*productid*/
#define APP_GT911_VENDOR_ID_REG   			0x814A  /* ��ǰģ��ѡ����Ϣ */
#define APP_GT911_CONFIG_VERSION_REG   		0x8047  /* �����ļ��汾�� */
#define APP_GT911_CONFIG_CHECKSUM_REG   	0x80FF  /* �����ļ�У���� */
#define APP_GT911_FIRMWARE_VERSION_REG   	0x8144  /* �̼��汾�� */

#define APP_GT911_POINT_NUM		5

typedef struct{
	uint8_t 	gt911_id[5];
	
	uint16_t 	gt911_x;
	uint16_t 	gt911_y;
	
	uint8_t 	gt911_p_num;
	
}app_touch_gt911_dev_t;


 /*
  *App_Touch_Gt911_Crc16: ��ȡCrc16 ����
  *������
  * 	src_data : ���������
  *		len		: ���ݳ���
  *����ֵ��
  * 	Crc16 ���
  */
uint8_t App_Touch_Gt911_Crc16(uint8_t *src_data, uint8_t len);

void App_Touch_Gt911_Soft_Reset(void);

void App_Touch_Gt911_Set_XY_Point(uint16_t x, uint16_t y, uint8_t p_num);

/*
 *App_Touch_Gt911_ReadID: ��ȡGt911 ID
 *������
 *	   void
 *����ֵ��
 *	   0����ȡ�ɹ�
 *	   1����ȡʧ��
 */
uint8_t App_Touch_Gt911_ReadID(void);

/*
 *App_Touch_Gt911_Cfg: ����GT911
 *������
 *	   void
 *����ֵ��
 *	   0��cfg�ɹ�
 *	   1��cfgʧ��
 */
uint8_t App_Touch_Gt911_Cfg(void);


/*
 *App_Touch_Gt911_Init: ��ʼ��GT911
 *������
 *	   void
 *����ֵ��
 *	   0����ʼ���ɹ�
 *	   1����ʼ��ʧ��
 */
uint8_t App_Touch_Gt911_Init(void);

void App_Touch_Gt911_Work_Func(void);


#endif

