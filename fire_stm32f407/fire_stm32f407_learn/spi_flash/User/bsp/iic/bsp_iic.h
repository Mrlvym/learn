#ifndef __BSP_IIC_H__
#define __BSP_IIC_H__
#include "stm32f4xx.h"

void Bsp_IIC_Gpio_Cfg(void);

void Bsp_IIC_Master_Cfg(uint16_t addr, uint16_t clk_speed);

void Bsp_IIC_Slave_Cfg(uint16_t addr, uint16_t clk_speed);

uint8_t Bsp_IIC_Master_BufferWrite(I2C_TypeDef* I2Cx, uint8_t *Buffer, uint32_t NumByteToWrite, uint8_t SlaveAddress);

uint8_t Bsp_IIC_Master_BufferRead(I2C_TypeDef* I2Cx, uint8_t *Buffer, uint32_t NumByteToRead, uint8_t SlaveAddress);


#endif

