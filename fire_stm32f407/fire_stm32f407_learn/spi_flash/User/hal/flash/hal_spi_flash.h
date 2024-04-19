#ifndef __HAL_SPI_FLASH_H__
#define __HAL_SPI_FLASH_H__
#include "stdint.h"

//SECTOR size is 4096
#define SPI_FLASH_SECTOR_SIZE	4096
#define SDIO_SD_BLOCK_SIZE		512

void Hal_Spi_Flash_Init(void);
void Hal_Spi_Flash_SendByte(uint8_t byte);
uint8_t Hal_Spi_Flash_ReadByte(void);
void Hal_SPI_FLASH_CS_L(void);
void Hal_SPI_FLASH_CS_H(void);
void Hal_Spi_Flash_Write_Enable(void);
void Hal_Spi_Flash_WaitForWriteEnd(void);
void Hal_Spi_Flash_PowerDown(void);
void Hal_Spi_Flash_ReleasePowerDown(void);
void Hal_Spi_Flash_SectorErase(uint32_t sector_addr);
void Hal_Spi_Flash_ChipErase(void);
uint32_t Hal_Spi_Flash_Read_DeviceID(void);
uint32_t Hal_Spi_Flash_Read_JedecDeviceID(void);
void Hal_Spi_Flash_PageWrite(uint8_t *pBuff, uint32_t address, uint32_t buf_len);
void Hal_Spi_Flash_BufferWrite(uint8_t *pBuff, uint32_t address, uint32_t buf_len);
void Hal_Spi_Flash_BufferRead(uint8_t *pBuff, uint32_t address, uint32_t buf_len);




#endif

