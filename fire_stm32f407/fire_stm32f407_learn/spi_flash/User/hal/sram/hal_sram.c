#include "hal_sram.h"
#include "hal_debug_usart.h"


void HAL_Sram_Init(void)
{
	BSP_Sram_GPIO_Init();

	BSP_Sram_Timing_Init();
}

void HAL_Sram_ReadByte(uint32_t addr_byte, uint8_t *data_byte, uint32_t len)
{
	if(addr_byte < HAL_Bank1_SRAM3_ADDR || addr_byte > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE))
	{
		Hal_Debug(DEBUG_LEVEL, "addr_byte is illegality ... \r\n");
		return ;
	}

	if(len > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE - addr_byte))
	{
		Hal_Debug(DEBUG_LEVEL, "len is to big ... \r\n");
		return ;
	}
		
	BSP_Sram_ReadByte(addr_byte, data_byte, len);
}

void HAL_Sram_WriteByte(uint32_t addr_byte, uint8_t *data_byte, uint32_t len)
{
	if(addr_byte < HAL_Bank1_SRAM3_ADDR || addr_byte > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE))
	{
		Hal_Debug(DEBUG_LEVEL, "addr_byte is illegality ... \r\n");
		return ;
	}

	if(len > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE - addr_byte))
	{
		Hal_Debug(DEBUG_LEVEL, "len is to big ... \r\n");
		return ;
	}
		
	BSP_Sram_WriteByte(addr_byte, data_byte, len);
}

void HAL_Sram_ReadShort(uint32_t addr_short, uint16_t *data_short, uint32_t len)
{
	len *= 2;

	if(addr_short < HAL_Bank1_SRAM3_ADDR || addr_short > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE))
	{
		Hal_Debug(DEBUG_LEVEL, "addr_short is illegality ... \r\n");
		return ;
	}

	if(len > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE - addr_short))
	{
		Hal_Debug(DEBUG_LEVEL, "len is to big ... \r\n");
		return ;
	}

	BSP_Sram_ReadShort(addr_short, data_short, len);
}

void HAL_Sram_WriteShort(uint32_t addr_short, uint16_t *data_short, uint32_t len)
{
	len *= 2;

	if(addr_short < HAL_Bank1_SRAM3_ADDR || addr_short > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE))
	{
		Hal_Debug(DEBUG_LEVEL, "addr_short is illegality ... \r\n");
		return ;
	}

	if(len > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE - addr_short))
	{
		Hal_Debug(DEBUG_LEVEL, "len is to big ... \r\n");
		return ;
	}

	BSP_Sram_WriteShort(addr_short, data_short, len);
}

void HAL_Sram_ReadWord(uint32_t addr_word, uint32_t *data_word, uint32_t len)
{
	len *= 4;

	if(addr_word < HAL_Bank1_SRAM3_ADDR || addr_word > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE))
	{
		Hal_Debug(DEBUG_LEVEL, "addr_word is illegality ... \r\n");
		return ;
	}

	if(len > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE - addr_word))
	{
		Hal_Debug(DEBUG_LEVEL, "len is to big ... \r\n");
		return ;
	}

	BSP_Sram_ReadWord(addr_word, data_word, len);
}

void HAL_Sram_WriteWord(uint32_t addr_word, uint32_t *data_word, uint32_t len)
{
	len *= 4;

	if(addr_word < HAL_Bank1_SRAM3_ADDR || addr_word > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE))
	{
		Hal_Debug(DEBUG_LEVEL, "addr_word is illegality ... \r\n");
		return ;
	}

	if(len > (HAL_Bank1_SRAM3_ADDR + HAL_Bank1_SRAM3_SIZE - addr_word))
	{
		Hal_Debug(DEBUG_LEVEL, "len is to big ... \r\n");
		return ;
	}

	BSP_Sram_WriteWord(addr_word, data_word, len);
}




