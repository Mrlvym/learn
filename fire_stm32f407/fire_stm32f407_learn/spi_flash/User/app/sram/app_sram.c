#include"app_sram.h"
#include"hal_sram.h"
#include "hal_debug_usart.h"

uint32_t app_testsram[250000] __attribute__((at(APP_Bank1_SRAM3_ADDR)));//测试用数组

void APP_Sram_Init(void)
{
	HAL_Sram_Init();
}

void APP_Sram_Memory_Test(void)
{
	
	uint32_t i = 0;
	uint8_t temp = 0;

	for(i = 0; i < APP_Bank1_SRAM3_SIZE; i += 4096)//每间隔4kb写入一个字节
	{
		HAL_Sram_WriteByte(APP_Bank1_SRAM3_ADDR+i, &temp, 1);
		temp++;
	}

	for(i = 0; i < APP_Bank1_SRAM3_SIZE; i += 4096)
	{
		HAL_Sram_ReadByte(APP_Bank1_SRAM3_ADDR+i, &temp, 1);

		if(temp < 0)
		{
			Hal_Debug(DEBUG_LEVEL, "Test Sram Memory error \r\n");
			break;
		}
		
		Hal_Debug(DEBUG_LEVEL, "Test Sram Memory is %d kb \r\n", ((temp+1)*4));
	}
}

uint8_t APP_Sram_Test(void)
{
	uint8_t test_byte1 = 0xAA, test_byte2 = 0x00;
	uint16_t test_short1 = 0xAABB, test_short2 = 0x0000;
	uint32_t test_word1 = 0xAABBCCDD, test_word2 = 0x00000000;

	APP_Sram_Memory_Test();

	HAL_Sram_WriteByte(APP_Bank1_SRAM3_ADDR, (uint8_t *)&test_byte1, 1);

	HAL_Sram_ReadByte(APP_Bank1_SRAM3_ADDR, (uint8_t *)&test_byte2, 1);

	if(test_byte1 == test_byte2)
	{
		Hal_Debug(DEBUG_LEVEL, "test sram readwrite byte success ... \r\n");
	}
	else
	{
		Hal_Debug(DEBUG_LEVEL, "test sram readwrite byte failed ... \r\n");

		return 1;
	}

	
	HAL_Sram_WriteShort(APP_Bank1_SRAM3_ADDR+10, (uint16_t *)&test_short1, 1);
	
	HAL_Sram_ReadShort(APP_Bank1_SRAM3_ADDR+10, (uint16_t *)&test_short2, 1);

	if(test_short1 == test_short2)
	{
		Hal_Debug(DEBUG_LEVEL, "test sram readwrite short success ... \r\n");
	}
	else
	{
		Hal_Debug(DEBUG_LEVEL, "test sram readwrite short failed ... \r\n");

		return 2;
	}

	HAL_Sram_WriteWord(APP_Bank1_SRAM3_ADDR+20, (uint32_t *)&test_word1, 1);
	
	HAL_Sram_ReadWord(APP_Bank1_SRAM3_ADDR+20, (uint32_t *)&test_word2, 1);

	if(test_word1 == test_word2)
	{
		Hal_Debug(DEBUG_LEVEL, "test sram readwrite word success ... \r\n");
	}
	else
	{
		Hal_Debug(DEBUG_LEVEL, "test sram readwrite word failed ... \r\n");

		return 3;
	}

	return 0;
}

void APP_Sram_Task(void)
{
	uint8_t ret = 0;

	APP_Sram_Init();

	ret = APP_Sram_Test();

	if(0 == ret)
	{
		Hal_Debug(DEBUG_LEVEL, "sram test is success ... \r\n");
	}
	else
	{
		Hal_Debug(DEBUG_LEVEL, "sram test is failed ... \r\n");
	}
}


