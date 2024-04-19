#ifndef __HAL_SDIO_H__
#define __HAL_SDIO_H__
#include "stm32f4xx.h"
#include "bsp_sdio.h"


SD_Error Hal_SD_Init(void);
SD_Error Hal_SD_Erase(uint64_t startaddr, uint64_t endaddr);
SD_Error Hal_SD_ReadBlock(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize);
SD_Error Hal_SD_ReadMultiBlocks(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
SD_Error Hal_SD_WriteBlock(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize);
SD_Error Hal_SD_WriteMultiBlocks(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
SD_Error Hal_SD_WaitReadOperation(void);
SD_Error Hal_SD_WaitWriteOperation(void);
void Hal_SD_WaitDMAOperation_Finish(void);



#endif 


