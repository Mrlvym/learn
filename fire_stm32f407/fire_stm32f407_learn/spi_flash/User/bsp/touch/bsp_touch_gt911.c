#include "bsp_touch_gt911.h"
#include "bsp_touch_iic.h"
#include "bsp_delay.h"


uint8_t Bsp_Touch_Gt911_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(BSP_TOUCH_GT911_WR_CMD);   	//发送写命令 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}
		  
void Bsp_Touch_Gt911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(BSP_TOUCH_GT911_WR_CMD);   //发送写命令 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(BSP_TOUCH_GT911_RD_CMD);   //发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件    
} 

void Bsp_Touch_Gt911_Rst_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(BSP_TOUCH_GT911_RST_CLK, ENABLE);

	GPIO_InitStruct.GPIO_Pin = BSP_TOUCH_GT911_RST_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(BSP_TOUCH_GT911_RST_PORT, &GPIO_InitStruct);

	BSP_TOUCH_GT911_RST = 0;
}

void Bsp_Touch_Gt911_Int_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(BSP_TOUCH_GT911_INT_CLK, ENABLE);

	GPIO_InitStruct.GPIO_Pin = BSP_TOUCH_GT911_INT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(BSP_TOUCH_GT911_INT_PORT, &GPIO_InitStruct);

	BSP_TOUCH_GT911_INT = 0;		
}

void Bsp_Touch_Gt911_Int_Irq_Cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHB1PeriphClockCmd(BSP_TOUCH_GT911_INT_CLK, ENABLE);

	GPIO_InitStruct.GPIO_Pin = BSP_TOUCH_GT911_INT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(BSP_TOUCH_GT911_INT_PORT, &GPIO_InitStruct);

	SYSCFG_EXTILineConfig(BSP_TOUCH_GT911_INT_EXTI_PORTSOURCE, BSP_TOUCH_GT911_INT_EXTI_PINSOURCE);

	EXTI_InitStruct.EXTI_Line = BSP_TOUCH_GT911_INT_EXTI_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = BSP_TOUCH_GT911_INT_IRQCHANNEL;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = BSP_TOUCH_GT911_INT_PRE_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = BSP_TOUCH_GT911_INT_SUB_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}

void Bsp_Touch_Gt911_PowerOn(void)
{
	
#if (defined(GT911_WR_CMD_0X28))
	BSP_TOUCH_GT911_RST = 0;
	BSP_TOUCH_GT911_INT = 0;
	Delay_ms(5);
	BSP_TOUCH_GT911_INT = 1;
	Delay_ms(5);
	BSP_TOUCH_GT911_RST = 1;
	Delay_ms(10);
	BSP_TOUCH_GT911_INT = 0;
#elif (defined(GT911_WR_CMD_0XBA))
	BSP_TOUCH_GT911_RST = 0;
	BSP_TOUCH_GT911_INT = 0;
	Delay_ms(10);
	BSP_TOUCH_GT911_RST = 1;
	Delay_ms(10);
#endif

	Delay_ms(50);
}

//uint8_t Bsp_Touch_Gt911_ReadID(void)
//{
//	uint8_t ret = 0;
//	uint8_t gt911_id[5];
//
//	Bsp_Touch_Gt911_RD_Reg(0x8140, gt911_id, 4);
//
//	gt911_id[4] = 0;
//
//	if(gt911_id[0] == 'g' && gt911_id[1] == 't' && gt911_id[2] == '9' && gt911_id[3] == '1' && gt911_id[4] == '1' && gt911_id[0] == '0')
//	{
//		ret = 0;
//	}
//	else
//	{
//		ret = 1;
//		printf("gt911_id : %s \r\n", gt911_id);
//	}
//
//	
//	return ret;	
//}

