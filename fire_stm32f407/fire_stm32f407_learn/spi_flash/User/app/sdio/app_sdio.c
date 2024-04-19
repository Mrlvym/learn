#include "app_sdio.h"
#include "hal_debug_usart.h"
#include "hal_delay.h"
#include "string.h"

uint8_t App_Buf_Block_Tx[APP_BLOCK_SIZE], App_Buf_Block_Rx[APP_BLOCK_SIZE];
uint8_t App_Buf_MulBlock_Tx[APP_MUL_BUF_SIZE],App_Buf_MulBlock_Rx[APP_MUL_BUF_SIZE];
app_test_status_e	App_EraseStatus = APP_FAILED, App_TransferStatus1 = APP_FAILED, App_TransferStatus2 = APP_FAILED;
SD_Error App_Status = SD_OK;


void App_SD_Test_Task(void)
{
	App_Status = Hal_SD_Init();
	if(App_Status != SD_OK)
	{
		Hal_Debug(PRINTF, "SD 卡初始化失败，请确保已插入开发板，或更换一张SD卡测试 ... \r\n");
	}
	else
	{
		Hal_Debug(PRINTF, "SD 卡初始化成功 ... \r\n");
	}

	if(App_Status == SD_OK)
	{
		Hal_Debug(PRINTF, "SD 擦除测试 ... \r\n");
		App_SD_Erase_Test();

		Hal_Debug(PRINTF, "SD 单块读写测试 ... \r\n");
		App_SD_SingleBlock_Test();

		Hal_Debug(PRINTF, "SD 多块读写测试 ... \r\n");
		App_SD_MulBlock_Test();
	}
}

void App_SD_Erase_Test(void)
{
	App_EraseStatus = APP_FAILED;
	App_Status = Hal_SD_Erase(APP_SD_TEST_ADDR, (APP_SD_TEST_ADDR + (APP_NUMBER_OF_BLOCKS * APP_BLOCK_SIZE)));
	if(App_Status != SD_OK)
	{
		Hal_Debug(PRINTF, "SD Erase is failed ... \r\n");
	}

	if(App_Status == SD_OK)
	{
		App_Status = Hal_SD_ReadMultiBlocks(App_Buf_MulBlock_Rx, APP_SD_TEST_ADDR, APP_BLOCK_SIZE, APP_NUMBER_OF_BLOCKS);
		if(App_Status != SD_OK)
		{
			Hal_Debug(PRINTF, "SD ResdMulBlock is failed ... \r\n");
		}
		else
		{			
			App_Status = Hal_SD_WaitReadOperation();
			
			Hal_SD_WaitDMAOperation_Finish();
		}

		if(App_Status == SD_OK)
		{
			App_EraseStatus = eCompare_Buffer(App_Buf_MulBlock_Rx, APP_MUL_BUF_SIZE);

			if(App_EraseStatus == APP_PASS)
			{
				Hal_Debug(PRINTF, "SD Erase Test is Pass ... \r\n");				
			}
			else
			{
				Hal_Debug(PRINTF, "SD Erase Test is Failed ... \r\n");								
			}
		}		
	}
}

void App_SD_SingleBlock_Test(void)
{
	Fill_Buffer(App_Buf_Block_Tx, APP_BLOCK_SIZE);

	App_Status = Hal_SD_WriteBlock(App_Buf_Block_Tx, APP_SD_TEST_ADDR, APP_BLOCK_SIZE);

//	if(App_Status != SD_OK)
//	{	
//		Hal_Debug(PRINTF, "SD WriteBlock Failed ... \r\n");
//	}
	if(App_Status == SD_OK)
	{
		App_Status = Hal_SD_WaitWriteOperation();
		
		Hal_SD_WaitDMAOperation_Finish();		
	}

	if(App_Status == SD_OK)
	{
		memset(App_Buf_Block_Rx, 0, APP_BLOCK_SIZE);
		App_Status = Hal_SD_ReadBlock(App_Buf_Block_Rx, APP_SD_TEST_ADDR, APP_BLOCK_SIZE);
		
//		if(App_Status != SD_OK)
//		{	
//			Hal_Debug(PRINTF, "SD ReadBlock Failed ... \r\n");
//		}
		if(App_Status == SD_OK)
		{
			App_Status = Hal_SD_WaitReadOperation();
			
			Hal_SD_WaitDMAOperation_Finish();			
		}
		
		if(App_Status == SD_OK)
		{
			App_TransferStatus1 =  Compare_Buffer(App_Buf_Block_Tx, App_Buf_Block_Rx, APP_BLOCK_SIZE);

			if(App_TransferStatus1 == APP_PASS)
			{				
				Hal_Debug(PRINTF, "SD Block Test is Pass ... \r\n");				
			}
			else
			{
				Hal_Debug(PRINTF, "SD Block Test is Failed ... \r\n");								
			}
		}
		else
		{
			Hal_Debug(PRINTF, "SD ReadBlock Failed 1 ... \r\n");
		}
	}
	else
	{
		Hal_Debug(PRINTF, "SD WriteBlock Failed 2 ... \r\n");
	}
}

void App_SD_MulBlock_Test(void)
{
	Fill_Buffer(App_Buf_MulBlock_Tx, APP_MUL_BUF_SIZE);

	App_Status = Hal_SD_WriteMultiBlocks(App_Buf_MulBlock_Tx, APP_SD_TEST_ADDR, APP_BLOCK_SIZE, APP_NUMBER_OF_BLOCKS);

//	if(App_Status != SD_OK)
//	{	
//		Hal_Debug(PRINTF, "SD WriteMulBlock Failed ... \r\n");
//	}
	if(App_Status == SD_OK)
	{
		App_Status = Hal_SD_WaitWriteOperation();
		
		Hal_SD_WaitDMAOperation_Finish();		
	}	

	if(App_Status == SD_OK)
	{
		memset(App_Buf_MulBlock_Rx, 0, APP_MUL_BUF_SIZE);
		App_Status = Hal_SD_ReadMultiBlocks(App_Buf_MulBlock_Rx, APP_SD_TEST_ADDR, APP_BLOCK_SIZE, APP_NUMBER_OF_BLOCKS);
		
//		if(App_Status != SD_OK)
//		{	
//			Hal_Debug(PRINTF, "SD ReadMulBlock Failed ... \r\n");
//		}
		if(App_Status == SD_OK)
		{
			App_Status = Hal_SD_WaitReadOperation();
			
			Hal_SD_WaitDMAOperation_Finish();			
		}	

		if(App_Status == SD_OK)
		{
			App_TransferStatus2 =  Compare_Buffer(App_Buf_MulBlock_Tx, App_Buf_MulBlock_Rx, APP_MUL_BUF_SIZE);

			if(App_TransferStatus2 == APP_PASS)
			{				
				Hal_Debug(PRINTF, "SD Mul Block Test is Pass ... \r\n");				
			}
			else
			{
				Hal_Debug(PRINTF, "SD Mul Block Test is Failed ... \r\n");								
			}
		}
		else
		{
			Hal_Debug(PRINTF, "SD ReadMulBlock Failed 1 ... \r\n");			
		}
	}
	else
	{
		Hal_Debug(PRINTF, "SD WriteMulBlock Failed 2 ... \r\n");
	}
	
}

void Fill_Buffer(uint8_t *buf, uint32_t buf_len)
{
	memset(buf, 0, buf_len);

	while(buf_len >0)
	{
		*buf = buf_len;

		buf++;
		buf_len--;
	}
}

app_test_status_e Compare_Buffer(uint8_t *buf1, uint8_t *buf2, uint32_t buf_len)
{
	app_test_status_e ret = APP_PASS;	
	
	while(buf_len > 0)
	{
		if(*buf1 != *buf2)
		{	
			ret = APP_FAILED;
			break;
		}

		buf_len--;
		buf1++;
		buf2++;
	}

	return ret;
}

app_test_status_e eCompare_Buffer(uint8_t *buf, uint32_t buf_len)
{
	app_test_status_e ret = APP_PASS;	

	while(buf_len > 0)
	{
		if(*buf != 0xFF && *buf != 0x00)
		{
			ret = APP_FAILED;
			break;
		}
		buf_len--;
		buf++;
	}

	return ret;
}



