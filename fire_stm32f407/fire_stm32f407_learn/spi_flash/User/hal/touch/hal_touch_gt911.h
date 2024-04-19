#ifndef __HAL_TOUCH_GT911_H__
#define __HAL_TOUCH_GT911_H__
#include "sys.h"
 /*
  * Hal_Touch_Gt911_RD_Reg:读取gt911寄存器接口
  *	参数：
  *		reg:起始寄存器地址
  *		buf:数据缓缓存区
  *		len:读数据长度
  *	返回值：
  *	  	void
  */ 
void Hal_Touch_Gt911_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

/*
  * Hal_Touch_Gt911_WR_Reg:写入gt911寄存器接口
  *	参数：
  *		reg:起始寄存器地址
  *		buf:数据缓缓存区
  *		len:读数据长度
  *	返回值：
  *		0:ACK成功
  *		1:ACK失败
  */
uint8_t Hal_Touch_Gt911_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

 /*
 * Hal_Touch_Gt911_Int_Irq_Cfg:初始化Gt911 Int 引脚为中断模式
 * 参数：
 *	   void
 * 返回值：
 *	   void
 */
void Hal_Touch_Gt911_Int_Irq_Cfg(void);


/*
 * Hal_Touch_Gt911_Init: gt911初始化
 * 参数：
 *	   void
 * 返回值：
 *	   void
 */
void Hal_Touch_Gt911_Init(void);

#endif

