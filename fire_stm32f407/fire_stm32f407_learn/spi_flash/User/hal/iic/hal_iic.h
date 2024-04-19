#ifndef __HAL_IIC_H__
#define __HAL_IIC_H__
#include "stm32f4xx.h"
#include "bsp_iic.h"


void Hal_IIC_Master_Init(uint8_t slave_addr, uint16_t clk_speed);

void Hal_IIC_Slave_Init(uint8_t slave_addr, uint16_t clk_speed);

uint8_t Hal_IIC_Master_BufferWrite(I2C_TypeDef* I2Cx, uint8_t *Buffer, uint32_t NumByteToWrite, uint8_t SlaveAddress);

uint8_t Hal_IIC_Master_BufferRead(I2C_TypeDef* I2Cx, uint8_t *Buffer, uint32_t NumByteToRead, uint8_t SlaveAddress);

#endif


