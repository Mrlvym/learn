#include "bsp_sram.h"

void BSP_Sram_GPIO_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;

	//open gpio、af、ahp3 clk
	RCC_AHB1PeriphClockCmd(FSMC_A_GPIO_CLK | FSMC_D_GPIO_CLK | FSMC_NBLx_GPIO_CLK | FSMC_NOWE_GPIO_CLK | FSMC_NE3_GPIO_CLK, ENABLE);

	//FSMC_A0 - A18
	GPIO_InitStruct.GPIO_Pin = FSMC_A0 | FSMC_A1 | FSMC_A2 | FSMC_A3 | FSMC_A4 | FSMC_A5 | FSMC_A6 | FSMC_A7 | FSMC_A8 | FSMC_A9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(FSMC_A_GPIO_F, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = FSMC_A10 | FSMC_A11 | FSMC_A12 | FSMC_A13 | FSMC_A14 | FSMC_A15;
	GPIO_Init(FSMC_A_GPIO_G, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = FSMC_A16 | FSMC_A17 | FSMC_A18;
	GPIO_Init(FSMC_A_GPIO_D, &GPIO_InitStruct);

	//FSMC_D0 - D15
	GPIO_InitStruct.GPIO_Pin = FSMC_D0 | FSMC_D1 | FSMC_D2 | FSMC_D3 | FSMC_D13 | FSMC_D14 | FSMC_D15;
	GPIO_Init(FSMC_D_GPIO_D, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = FSMC_D4 | FSMC_D5 | FSMC_D6 | FSMC_D7 | FSMC_D8 | FSMC_D9 | FSMC_D10 | FSMC_D11 | FSMC_D12;
	GPIO_Init(FSMC_D_GPIO_E, &GPIO_InitStruct);

	//FSMC_NBLx
	GPIO_InitStruct.GPIO_Pin = FSMC_NBL0 | FSMC_NBL1;
	GPIO_Init(FSMC_NBLx_GPIO_E, &GPIO_InitStruct);

	//FSMC_NOE / NWE
	GPIO_InitStruct.GPIO_Pin = FSMC_NOE | FSMC_NWE;
	GPIO_Init(FSMC_NOWE_GPIO_D, &GPIO_InitStruct);

	//FSMC_NE3
	GPIO_InitStruct.GPIO_Pin = FSMC_NE3;
	GPIO_Init(FSMC_NE3_GPIO_G, &GPIO_InitStruct);

	//FSMC_AF_A0 - A12
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A2, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A3, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A6, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A7, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_F, FSMC_AF_A9, GPIO_AF_FSMC);

	GPIO_PinAFConfig(FSMC_A_GPIO_G, FSMC_AF_A10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_G, FSMC_AF_A11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_G, FSMC_AF_A12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_G, FSMC_AF_A13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_G, FSMC_AF_A14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_G, FSMC_AF_A15, GPIO_AF_FSMC);

	GPIO_PinAFConfig(FSMC_A_GPIO_D, FSMC_AF_A16, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_D, FSMC_AF_A17, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_A_GPIO_D, FSMC_AF_A18, GPIO_AF_FSMC);

	//FSMC_AF_D0 - D15
	GPIO_PinAFConfig(FSMC_D_GPIO_D, FSMC_AF_D0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_D, FSMC_AF_D1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_D, FSMC_AF_D2, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_D, FSMC_AF_D3, GPIO_AF_FSMC);

	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D6, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D7, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_E, FSMC_AF_D12, GPIO_AF_FSMC);	
	
	GPIO_PinAFConfig(FSMC_D_GPIO_D, FSMC_AF_D13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_D, FSMC_AF_D14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_D_GPIO_D, FSMC_AF_D15, GPIO_AF_FSMC);

	//FSMC_AF_NBLx
	GPIO_PinAFConfig(FSMC_NBLx_GPIO_E, FSMC_AF_NBL0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_NBLx_GPIO_E, FSMC_AF_NBL1, GPIO_AF_FSMC);

	//FSMC_AF_NOE / NWE
	GPIO_PinAFConfig(FSMC_NOWE_GPIO_D, FSMC_AF_NOE, GPIO_AF_FSMC);
	GPIO_PinAFConfig(FSMC_NOWE_GPIO_D, FSMC_AF_NWE, GPIO_AF_FSMC);

	//FSMC_AF_NE3
	GPIO_PinAFConfig(FSMC_NE3_GPIO_G, FSMC_AF_NE3, GPIO_AF_FSMC);

}

void BSP_Sram_Timing_Init(void)
{
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTimingStruct;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStruct;

	RCC_AHB3PeriphClockCmd(FSMC_CLK, ENABLE);
	
	//时序
	FSMC_ReadWriteTimingStruct.FSMC_CLKDivision = 0;							/* 时钟分频因子 ,1-0xF ，若控制异步存储器，本参数无效 */
	FSMC_ReadWriteTimingStruct.FSMC_AccessMode = FSMC_AccessMode_A;				/* 设置访问模式 */
	FSMC_ReadWriteTimingStruct.FSMC_AddressSetupTime = 0;						/* 地址建立时间， 0-0xF 个 HCLK 周期 */
	FSMC_ReadWriteTimingStruct.FSMC_AddressHoldTime = 0;						/* 地址保持时间， 0-0xF 个 HCLK 周期 */
	FSMC_ReadWriteTimingStruct.FSMC_DataSetupTime = 8;							/* 数据建立时间， 0-0xF 个 HCLK 周期 */
	FSMC_ReadWriteTimingStruct.FSMC_DataLatency = 0;							/* 数据延迟时间，若控制异步存储器，本参数无效 */
	FSMC_ReadWriteTimingStruct.FSMC_BusTurnAroundDuration = 0;					/* 总线转换周期 ,0-0xF 个 HCLK 周期，在 NOR FLASH */

	FSMC_NORSRAMInitStruct.FSMC_Bank = FSMC_Bank1_NORSRAM3;								/* 设置要控制的 Bank 区域 */
	FSMC_NORSRAMInitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 			/* 设置地址总线与数据总线是否复用 */
	FSMC_NORSRAMInitStruct.FSMC_MemoryType = FSMC_MemoryType_SRAM; 						/* 设置存储器的类型 */
	FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; 			/* 设置存储器的数据宽度 */
	FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; 		/* 设置是否支持突发访问模式，只支持同步类型的存储器      */
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable; 		/* 设置是否使能在同步传输时的等待信号， */
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low; 		/* 设置等待信号的极性 */
	FSMC_NORSRAMInitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable; 						/* 设置是否支持对齐的突发模式 */
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; /* 配置等待信号在等待前有效还是等待期间有效 */
	FSMC_NORSRAMInitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable; 			/* 设置是否写使能 */
	FSMC_NORSRAMInitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 					/* 设置是否使能等待状态插入 */
	FSMC_NORSRAMInitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; 				/* 设置是否使能扩展模式 */
	FSMC_NORSRAMInitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 					/* 设置是否使能写突发操作 */
	/* 当不使用扩展模式时，本参数用于配置读写时序，否则用于配置读时序 */
	FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTimingStruct;
	FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct = &FSMC_ReadWriteTimingStruct;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  // 使能BANK1区域3
}

void BSP_Sram_ReadByte(uint32_t addr_byte, uint8_t *data_byte, uint32_t len)
{
	uint32_t i = 0;

	for(i = 0; i < len; i++)
	{
		*(data_byte + i) = *(volatile uint8_t *)(addr_byte + i);
	}
}

void BSP_Sram_WriteByte(uint32_t addr_byte, uint8_t *data_byte, uint32_t len)
{
	uint32_t i = 0;

	for(i = 0; i < len; i++)
	{
		*(volatile uint8_t *)(addr_byte + i) = *(data_byte + i);
	}
}

void BSP_Sram_ReadShort(uint32_t addr_short, uint16_t *data_short, uint32_t len)
{
	uint32_t i = 0;

	for(i = 0; i < len; i+=2)
	{
		*(data_short + i) = *(volatile uint16_t *)(addr_short + i);
	}
}


void BSP_Sram_WriteShort(uint32_t addr_short, uint16_t *data_short, uint32_t len)
{
	uint32_t i = 0;

	for(i = 0; i < len; i+=2)
	{
		*(volatile uint16_t *)(addr_short + i) = *(data_short + i);
	}
}

void BSP_Sram_ReadWord(uint32_t addr_word, uint32_t *data_word, uint32_t len)
{
	uint32_t i = 0;

	for(i = 0; i < len; i+=4)
	{
		*(data_word + i) = *(volatile uint32_t *)(addr_word + i);
	}
}

void BSP_Sram_WriteWord(uint32_t addr_word, uint32_t *data_word, uint32_t len)
{
	uint32_t i = 0;

	for(i = 0; i < len; i+=4)
	{
		*(volatile uint32_t *)(addr_word + i) = *(data_word + i);
	}
}



