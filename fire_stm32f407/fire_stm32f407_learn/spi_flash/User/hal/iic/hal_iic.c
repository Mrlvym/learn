#include "hal_iic.h"


void Hal_IIC_Master_Init(uint8_t slave_addr, uint16_t clk_speed)
{
	Bsp_IIC_Gpio_Cfg();	
	Bsp_IIC_Master_Cfg(slave_addr, clk_speed);
}

void Hal_IIC_Slave_Init(uint8_t slave_addr, uint16_t clk_speed)
{
	Bsp_IIC_Gpio_Cfg();	
	Bsp_IIC_Slave_Cfg(slave_addr, clk_speed);	
}

uint8_t Hal_IIC_Master_BufferWrite(I2C_TypeDef* I2Cx, uint8_t *Buffer, uint32_t NumByteToWrite, uint8_t SlaveAddress)
{
	return Bsp_IIC_Master_BufferWrite(I2Cx, Buffer, NumByteToWrite, SlaveAddress);
}

uint8_t Hal_IIC_Master_BufferRead(I2C_TypeDef* I2Cx, uint8_t *Buffer, uint32_t NumByteToRead, uint8_t SlaveAddress)
{
	return Bsp_IIC_Master_BufferRead(I2Cx, Buffer, NumByteToRead, SlaveAddress);
}


