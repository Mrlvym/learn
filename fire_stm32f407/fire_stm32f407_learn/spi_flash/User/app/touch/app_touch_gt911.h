#ifndef __APP_TOUCH_GT911_H__
#define __APP_TOUCH_GT911_H__
#include "stdint.h"

#define APP_GT911_READ_XY_REG 				0x814E	/* 坐标寄存器 */
#define APP_GT911_CLEARBUF_REG 				0x814E	/* 清除坐标寄存器 */
#define APP_GT911_READ_POINT1_REG			0x814F/* 坐标1寄存器 */
#define APP_GT911_READ_POINT2_REG			0x8157/* 坐标2寄存器 */
#define APP_GT911_READ_POINT3_REG			0x815F/* 坐标3寄存器 */
#define APP_GT911_READ_POINT4_REG			0x8167/* 坐标4寄存器 */
#define APP_GT911_READ_POINT5_REG			0x816F/* 坐标5寄存器 */
#define APP_GT911_CONFIG_REG				0x8047	/* 配置参数寄存器 */
#define APP_GT911_COMMAND_REG   			0x8040  /* 实时命令 */
#define APP_GT911_PRODUCT_ID_REG 			0x8140 /*productid*/
#define APP_GT911_VENDOR_ID_REG   			0x814A  /* 当前模组选项信息 */
#define APP_GT911_CONFIG_VERSION_REG   		0x8047  /* 配置文件版本号 */
#define APP_GT911_CONFIG_CHECKSUM_REG   	0x80FF  /* 配置文件校验码 */
#define APP_GT911_FIRMWARE_VERSION_REG   	0x8144  /* 固件版本号 */

#define APP_GT911_POINT_NUM		5

typedef struct{
	uint8_t 	gt911_id[5];
	
	uint16_t 	gt911_x;
	uint16_t 	gt911_y;
	
	uint8_t 	gt911_p_num;
	
}app_touch_gt911_dev_t;


 /*
  *App_Touch_Gt911_Crc16: 读取Crc16 计算
  *参数：
  * 	src_data : 计算的数据
  *		len		: 数据长度
  *返回值：
  * 	Crc16 结果
  */
uint8_t App_Touch_Gt911_Crc16(uint8_t *src_data, uint8_t len);

void App_Touch_Gt911_Soft_Reset(void);

void App_Touch_Gt911_Set_XY_Point(uint16_t x, uint16_t y, uint8_t p_num);

/*
 *App_Touch_Gt911_ReadID: 读取Gt911 ID
 *参数：
 *	   void
 *返回值：
 *	   0：读取成功
 *	   1：读取失败
 */
uint8_t App_Touch_Gt911_ReadID(void);

/*
 *App_Touch_Gt911_Cfg: 配置GT911
 *参数：
 *	   void
 *返回值：
 *	   0：cfg成功
 *	   1：cfg失败
 */
uint8_t App_Touch_Gt911_Cfg(void);


/*
 *App_Touch_Gt911_Init: 初始化GT911
 *参数：
 *	   void
 *返回值：
 *	   0：初始化成功
 *	   1：初始化失败
 */
uint8_t App_Touch_Gt911_Init(void);

void App_Touch_Gt911_Work_Func(void);


#endif

