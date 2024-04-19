#include "bsp_spi_flash.h"
#include "stdio.h"

__IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;


void Spi_Flash_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOG, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);	

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;//PG7
	GPIO_Init(GPIOG, &GPIO_InitStruct);//初始化
	GPIO_SetBits(GPIOG,GPIO_Pin_7);//PG7输出1,防止NRF干扰SPI FLASH的通信 

	SPI_FLASH_CS_H();

	SPI_I2S_DeInit(SPI1);
	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI1, &SPI_InitStruct);

	SPI_Cmd(SPI1, ENABLE);
}


uint8_t Spi_Flash_SendByte(uint8_t byte)
{
	uint8_t rec_byte;

	SPITimeout = SPIT_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(0);
		}
	}
	SPI_I2S_SendData(SPI1, byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(1);
		}
	}

	rec_byte = SPI_I2S_ReceiveData(SPI1);

	return rec_byte;
}

uint8_t Spi_Flash_ReadByte(void)
{
	return Spi_Flash_SendByte(Dummy_Byte);
}


/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  printf("SPI 等待超时!errorCode = %d \r\n",errorCode);
  return 0;
}



