#ifndef __BSP_TOUCH_GT911_H__
#define __BSP_TOUCH_GT911_H__
#include "sys.h"


//I2C读写命令
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

//IO操作函数	 
#define BSP_TOUCH_GT911_RST    		PCout(13)	//GT911复位引脚
#define BSP_TOUCH_GT911_INT    		PBout(1)	//GT911中断引脚	


 /*
  * Bsp_Touch_Gt911_RD_Reg:读取gt911寄存器接口
  *	参数：
  *		reg:起始寄存器地址
  *		buf:数据缓缓存区
  *		len:读数据长度
  *	返回值：
  *	  	void
  */ 
void Bsp_Touch_Gt911_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

/*
  * Bsp_Touch_Gt911_WR_Reg:写入gt911寄存器接口
  *	参数：
  *		reg:起始寄存器地址
  *		buf:数据缓缓存区
  *		len:读数据长度
  *	返回值：
  *		0:ACK成功
  *		1:ACK失败
  */
uint8_t Bsp_Touch_Gt911_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

 /*
  *Bsp_Touch_Gt911_Rst_Pin_Init: Gt911 Rst引脚初始化
  *参数：
  *		void
  *返回值：
  *		void
  */
void Bsp_Touch_Gt911_Rst_Pin_Init(void);
 
 /*
  *Bsp_Touch_Gt911_Int_Pin_Init: Gt911 Int引脚初始化
  *参数：
  * 	void
  *返回值：
  * 	void
  */
 void Bsp_Touch_Gt911_Int_Pin_Init(void);
 
 /*
 * Bsp_Touch_Gt911_PowerOn:Gt911上电时序
 * 参数：
 *	   void
 * 返回值：
 *	   void
 */
 void Bsp_Touch_Gt911_PowerOn(void);


 /*
 * Bsp_Touch_Gt911_Int_Irq_Cfg:初始化Gt911 Int 引脚为中断模式
 * 参数：
 *	   void
 * 返回值：
 *	   void
 */
void Bsp_Touch_Gt911_Int_Irq_Cfg(void);

 /*
  *Bsp_Touch_Gt911_ReadID: 读取Gt911 ID
  *参数：
  * 	void
  *返回值：
  * 	0：读取成功
  * 	1：读取失败
  */
 uint8_t Bsp_Touch_Gt911_ReadID(void);


#endif

