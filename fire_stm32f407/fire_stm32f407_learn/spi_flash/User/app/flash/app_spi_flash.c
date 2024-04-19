#include "app_spi_flash.h"
#include "hal_spi_flash.h"
#include "hal_debug_usart.h"
#include "bsp_spi_flash.h"

uint8_t test_Buf1[] = "this is w25128q test buf 111\r\n";
//uint8_t test_Buf2[countof(test_Buf1) - 1];
uint8_t test_Buf2[countof(test_Buf1)];

void App_Spi_Flash_Test_Task(void)
{
	uint8_t ret = 0;
	uint32_t Device_ID = 0, JedecDevice_ID = 0; 

	Hal_Spi_Flash_Init();
	
	Hal_Debug(DEBUG_LEVEL, "This is Spi Flash W25Q128 test ...\r\n");

	Device_ID = Hal_Spi_Flash_Read_DeviceID();

	JedecDevice_ID = Hal_Spi_Flash_Read_JedecDeviceID();

	Hal_Debug(DEBUG_LEVEL, "Device_ID = 0x%x, JedecDevice_ID = 0x%x \r\n", Device_ID, JedecDevice_ID);

	if(JedecDevice_ID != sFLASH_ID)
	{
		Hal_Spi_Flash_PowerDown();
		Hal_Debug(ERROR_LEVEL, "Read W25Q128 JedecDeviceID is error ... \r\n");
		return;
	}

	Hal_Debug(DEBUG_LEVEL, "Read W25Q128 JedecDeviceID is success ... \r\n");

	//先擦除想要写入的扇区
//	Hal_Spi_Flash_ChipErase();//整片擦除
	Hal_Spi_Flash_SectorErase(1 * 4096);

	Hal_Debug(INFO_LEVEL, "%s", test_Buf1);

	Hal_Spi_Flash_BufferWrite(test_Buf1, 1 * 4096, countof(test_Buf1) -1);
	Hal_Spi_Flash_BufferRead(test_Buf2, 1 * 4096, countof(test_Buf2) -1);

	Hal_Debug(INFO_LEVEL, "%s", test_Buf2);
	ret = App_Mem_CMP(test_Buf1, test_Buf2, countof(test_Buf1) -1);
	if(ret == true)
	{
		Hal_Debug(DEBUG_LEVEL, "W25Q128 test success ... \r\n");		
	}
	else
	{
		Hal_Debug(DEBUG_LEVEL, "W25Q128 test faid ... \r\n");				
	}
}

uint8_t App_Mem_CMP(uint8_t *pBuff1, uint8_t *pBuff2, uint32_t buf_len)
{
	uint8_t ret = true;

	while(buf_len--)
	{
		if(*pBuff1 != *pBuff2)
		{
			ret = false;
			break;
		}

		pBuff1++;
		pBuff2++;
	}
	
	return ret;
}


