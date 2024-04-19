#ifndef __APP_SPI_FLASH_H__
#define __APP_SPI_FLASH_H__
#include "stdint.h"

void App_Spi_Flash_Test_Task(void);

uint8_t App_Mem_CMP(uint8_t *pBuff1, uint8_t *pBuff2, uint32_t buf_len);

#endif


