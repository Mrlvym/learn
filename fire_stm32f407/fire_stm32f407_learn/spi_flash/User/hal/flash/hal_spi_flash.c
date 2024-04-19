#include "hal_spi_flash.h"
#include "bsp_spi_flash.h"
#include "hal_debug_usart.h"

void Hal_Spi_Flash_Init(void)
{
	Spi_Flash_Init();
}

void Hal_Spi_Flash_SendByte(uint8_t byte)
{
	Spi_Flash_SendByte(byte);
}

uint8_t Hal_Spi_Flash_ReadByte(void)
{
	return Spi_Flash_ReadByte();
}

void Hal_SPI_FLASH_CS_L(void)
{
	SPI_FLASH_CS_L();
}

void Hal_SPI_FLASH_CS_H(void)
{
	SPI_FLASH_CS_H();
}


void Hal_Spi_Flash_Write_Enable(void)
{
	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_WriteEnable);

	Hal_SPI_FLASH_CS_H();
}

void Hal_Spi_Flash_WaitForWriteEnd(void)
{
	
	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_ReadStatusReg);

	SPITimeout = SPIT_FLAG_TIMEOUT;
		
	do
	{
		if((SPITimeout--) == 0)
		{
			SPI_TIMEOUT_UserCallback(2);
			return;
		}	
	}
	while (((Spi_Flash_ReadByte()) & (WIP_Flag)) == SET);
	
	Hal_SPI_FLASH_CS_H();
}

void Hal_Spi_Flash_PowerDown(void)
{
	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_PowerDown);

	Hal_SPI_FLASH_CS_H();
}


void Hal_Spi_Flash_ReleasePowerDown(void)
{
	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_ReleasePowerDown);

	Hal_SPI_FLASH_CS_H();
}

/*地址要做到4kb对齐*/
void Hal_Spi_Flash_SectorErase(uint32_t sector_addr)
{
	Hal_Spi_Flash_Write_Enable();

	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_SectorErase);

	Hal_Spi_Flash_SendByte((sector_addr >> 16) & 0xff);
	
	Hal_Spi_Flash_SendByte((sector_addr >> 8) & 0xff);
	
	Hal_Spi_Flash_SendByte((sector_addr >> 0) & 0xff);

	Hal_SPI_FLASH_CS_H();

	Hal_Spi_Flash_WaitForWriteEnd();
		
}

void Hal_Spi_Flash_ChipErase(void)
{
	Hal_Spi_Flash_Write_Enable();

	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_ChipErase);

	Hal_SPI_FLASH_CS_H();

	Hal_Spi_Flash_WaitForWriteEnd();
}

uint32_t Hal_Spi_Flash_Read_DeviceID(void)
{
	uint8_t temp = 0;

	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_DeviceID);

	Hal_Spi_Flash_SendByte(Dummy_Byte);
	Hal_Spi_Flash_SendByte(Dummy_Byte);
	Hal_Spi_Flash_SendByte(Dummy_Byte);

	temp = Hal_Spi_Flash_ReadByte();

	Hal_SPI_FLASH_CS_H();

	return (uint32_t)temp;
}

uint32_t Hal_Spi_Flash_Read_JedecDeviceID(void)
{
	uint8_t temp0 = 0, temp1 = 0, temp2 = 0;

	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_JedecDeviceID);

	temp0 = Hal_Spi_Flash_ReadByte();
	temp1 = Hal_Spi_Flash_ReadByte();
	temp2 = Hal_Spi_Flash_ReadByte();

	Hal_SPI_FLASH_CS_H();

	return ((temp0 << 16) | (temp1 << 8) | (temp2));
}



void Hal_Spi_Flash_PageWrite(uint8_t *pBuff, uint32_t address, uint32_t buf_len)
{
	Hal_Spi_Flash_Write_Enable();

	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_PageProgram);

	Hal_Spi_Flash_SendByte((address >> 16) & 0xff);

	Hal_Spi_Flash_SendByte((address >> 8) & 0xff);

	Hal_Spi_Flash_SendByte((address >> 0) & 0xff);

	if(buf_len > SPI_FLASH_PerWritePageSize)
	{
		buf_len = SPI_FLASH_PerWritePageSize;
		Hal_Debug(ERROR_LEVEL, "buf len is to big ...");
	}

	while(buf_len--)
	{
		Hal_Spi_Flash_SendByte(*pBuff);
		pBuff++;
	}

	Hal_SPI_FLASH_CS_H();
	
	Hal_Spi_Flash_WaitForWriteEnd();
}

void Hal_Spi_Flash_BufferWrite(uint8_t *pBuff, uint32_t address, uint32_t buf_len)
{
	uint8_t AddrAlign = 0, PageNum = 0, OverLength = 0, Page_OverLength = 0, temp = 0;

	AddrAlign = address % SPI_FLASH_PageSize;

	PageNum = buf_len / SPI_FLASH_PageSize;

	OverLength = buf_len % SPI_FLASH_PageSize;

	Page_OverLength = SPI_FLASH_PageSize - (address % SPI_FLASH_PageSize);

	if(AddrAlign == 0)
	{
		if(PageNum == 0)
		{
			if(OverLength != 0)
			{
				Hal_Spi_Flash_PageWrite(pBuff, address, OverLength);
			}
		}
		else
		{
			while(PageNum--)
			{		
				Hal_Spi_Flash_PageWrite(pBuff, address, SPI_FLASH_PageSize);
				pBuff += SPI_FLASH_PageSize;
				address += SPI_FLASH_PageSize;
			}
			
			if(OverLength != 0)
			{
				Hal_Spi_Flash_PageWrite(pBuff, address, OverLength);
			}
		}
	}
	else
	{
		if(PageNum == 0)
		{
			if(OverLength != 0)
			{			
				if(Page_OverLength >= OverLength)
				{
					Hal_Spi_Flash_PageWrite(pBuff, address, OverLength);					
				}
				else
				{
					temp = OverLength - Page_OverLength;
					Hal_Spi_Flash_PageWrite(pBuff, address, Page_OverLength);
					pBuff+=Page_OverLength;
					address+=Page_OverLength;
					Hal_Spi_Flash_PageWrite(pBuff, address, temp);											
				}
			}						
		}
		else
		{	
			buf_len -= Page_OverLength;
			Hal_Spi_Flash_PageWrite(pBuff, address, Page_OverLength);
			pBuff+=Page_OverLength;
			address+=Page_OverLength;

			PageNum = buf_len / SPI_FLASH_PageSize;
			
			OverLength = buf_len % SPI_FLASH_PageSize;

			
			while(PageNum--)
			{		
				Hal_Spi_Flash_PageWrite(pBuff, address, SPI_FLASH_PageSize);
				pBuff += SPI_FLASH_PageSize;
				address += SPI_FLASH_PageSize;
			}
			
			if(OverLength != 0)
			{
				Hal_Spi_Flash_PageWrite(pBuff, address, OverLength);
			}
		}
	}
}

void Hal_Spi_Flash_BufferRead(uint8_t *pBuff, uint32_t address, uint32_t buf_len)
{
	Hal_SPI_FLASH_CS_L();

	Hal_Spi_Flash_SendByte(W25X_ReadData);

	Hal_Spi_Flash_SendByte((address >> 16) & 0xff);

	Hal_Spi_Flash_SendByte((address >> 8) & 0xff);

	Hal_Spi_Flash_SendByte((address >> 0) & 0xff);

	while(buf_len--)
	{
		*pBuff = Hal_Spi_Flash_ReadByte();
		pBuff++;
	}

	Hal_SPI_FLASH_CS_H();
		
}

