#include "bsp_iic.h"



void Bsp_IIC_Gpio_Cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//iic-scl - PB8  iic-sda - PB9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
}

void Bsp_IIC_Master_Cfg(uint16_t addr, uint16_t clk_speed)
{
	I2C_InitTypeDef I2C_InitStruct;

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	I2C_DeInit(I2C1);

	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = addr;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_ClockSpeed = clk_speed;
	
	I2C_Init(I2C1, &I2C_InitStruct);

	I2C_Cmd(I2C1, ENABLE);
}

void Bsp_IIC_Slave_Cfg(uint16_t addr, uint16_t clk_speed)
{
	I2C_InitTypeDef I2C_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	I2C_DeInit(I2C1);

	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = addr;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_ClockSpeed = clk_speed;
	
	I2C_Init(I2C1, &I2C_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = I2C1_ER_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);

	I2C_ITConfig(I2C1, I2C_IT_BUF | I2C_IT_EVT | I2C_IT_ERR, ENABLE);
	
	I2C_Cmd(I2C1, ENABLE);
		
}

uint8_t Bsp_IIC_Master_BufferWrite(I2C_TypeDef* I2Cx, uint8_t *Buffer, uint32_t NumByteToWrite, uint8_t SlaveAddress)
{
	if(NumByteToWrite == 0)
		return 1;

	//start
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));//EV5

	//slave address
	I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//EV6

	//master send data
	while(NumByteToWrite--)
	{
		I2C_SendData(I2Cx, *Buffer);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING));//EV8
		Buffer++;
	}
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//EV8_2

	//stop
	I2C_GenerateSTOP(I2Cx, ENABLE);

	return 0;
}


uint8_t Bsp_IIC_Master_BufferRead(I2C_TypeDef* I2Cx, uint8_t *Buffer, uint32_t NumByteToRead, uint8_t SlaveAddress)
{
	if(NumByteToRead == 0)
		return 1;

	//等待总线不忙
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx, ENABLE);//释放总线

	//start
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));//EV5
	
	//slave address
	I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//EV6

	//start
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));//EV5
	
	//slave address
	I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//EV6

	//read data
	while(NumByteToRead)
	{
		//last data
		if(NumByteToRead == 1)
		{
			//NACK
			I2C_AcknowledgeConfig(I2Cx, DISABLE);			
			I2C_GenerateSTOP(I2Cx, ENABLE);
		}

		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));//EV7
		*Buffer = I2C_ReceiveData(I2Cx);
		Buffer++;
		NumByteToRead--;
	}

	return 0;
}


