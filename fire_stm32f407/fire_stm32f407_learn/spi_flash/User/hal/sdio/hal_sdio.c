#include "hal_sdio.h"
#include "hal_debug_usart.h"

SD_Error Hal_SD_Init(void)
{
	return SD_Init();	
}
 
SD_Error Hal_SD_Erase(uint64_t startaddr, uint64_t endaddr)
{
	return SD_Erase(startaddr, endaddr);
}

SD_Error Hal_SD_ReadBlock(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize)
{
	return SD_ReadBlock(readbuff, ReadAddr, BlockSize);
}

SD_Error Hal_SD_ReadMultiBlocks(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	return SD_ReadMultiBlocks(readbuff, ReadAddr, BlockSize, NumberOfBlocks);
}

SD_Error Hal_SD_WriteBlock(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize)
{
	return SD_WriteBlock(writebuff, WriteAddr, BlockSize);
}

SD_Error Hal_SD_WriteMultiBlocks(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	return SD_WriteMultiBlocks(writebuff, WriteAddr, BlockSize, NumberOfBlocks);
}


SD_Error Hal_SD_WaitReadOperation(void)
{
	return SD_WaitReadOperation();
}


SD_Error Hal_SD_WaitWriteOperation(void)
{
	return SD_WaitWriteOperation();
}

void Hal_SD_WaitDMAOperation_Finish(void)
{
//	uint8_t ret;
//
//	ret = SD_GetStatus();
//	Hal_Debug(PRINTF, "%d\r\n", ret);
//	while(ret != SD_TRANSFER_OK)
//	{		
//		ret = SD_GetStatus();
//		Hal_Debug(PRINTF, "%d\r\n", ret);
//	}

	while(SD_GetStatus() != SD_TRANSFER_OK);
}

