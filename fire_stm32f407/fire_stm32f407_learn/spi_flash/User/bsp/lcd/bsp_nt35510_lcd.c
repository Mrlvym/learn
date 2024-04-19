#include "bsp_nt35510_lcd.h"

void BSP_LCD_BL_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(BSP_LCD_BL_GPIO_CLK, ENABLE);

	GPIO_InitStruct.GPIO_Pin = BSP_LCD_BL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType =GPIO_OType_PP ;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(BSP_LCD_BL_GPIO, &GPIO_InitStruct);

	GPIO_ResetBits(BSP_LCD_BL_GPIO, BSP_LCD_BL);
}

void BSP_LCD_BL_On(void)
{
	GPIO_SetBits(BSP_LCD_BL_GPIO, BSP_LCD_BL);
}

void BSP_LCD_BL_Off(void)
{
	GPIO_ResetBits(BSP_LCD_BL_GPIO, BSP_LCD_BL);
}

void BSP_LCD_8080_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(BSP_LCD_GPIO_CLK, ENABLE);

	//RS - > PF12
	GPIO_InitStruct.GPIO_Pin = BSP_LCD_RS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType =GPIO_OType_PP ;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BSP_LCD_RS_GPIO, &GPIO_InitStruct);

	//CS - > PG12
	GPIO_InitStruct.GPIO_Pin = BSP_LCD_CS;
	GPIO_Init(BSP_LCD_CS_GPIO, &GPIO_InitStruct);

	//WR - > PD5	
	//RD - > PD4
	GPIO_InitStruct.GPIO_Pin = BSP_LCD_WR | BSP_LCD_RD;
	GPIO_Init(BSP_LCD_WRD_GPIO, &GPIO_InitStruct);


	//D0 - > PD14	
	//D1 - > PD15 	
	//D2 - > PD0	
	//D3 - > PD1	
	GPIO_InitStruct.GPIO_Pin = BSP_LCD_D0 | BSP_LCD_D1 | BSP_LCD_D2 | BSP_LCD_D3;
	GPIO_Init(BSP_LCD_D_D, &GPIO_InitStruct);

	//D4 - > PE7	
	//D5 - > PE8	
	//D6 - > PE9	
	//D7 - > PE10  
	//D8 - > PE11  
	//D9 - > PE12  
	//D10 - > PE13  
	//D11 - > PE14  
	//D12 - > PE15
	GPIO_InitStruct.GPIO_Pin = BSP_LCD_D4 | BSP_LCD_D5 | BSP_LCD_D6 | BSP_LCD_D7 | BSP_LCD_D8 | BSP_LCD_D9 | BSP_LCD_D10 | BSP_LCD_D11 | BSP_LCD_D12;
	GPIO_Init(BSP_LCD_D_E, &GPIO_InitStruct);

	//D13 - > PD8	
	//D14 - > PD9	
	//D15 - > PD10
	GPIO_InitStruct.GPIO_Pin = BSP_LCD_D13 | BSP_LCD_D14 | BSP_LCD_D15;
	GPIO_Init(BSP_LCD_D_D, &GPIO_InitStruct);

	//RS - > PF12
	GPIO_PinAFConfig(BSP_LCD_RS_GPIO, BSP_LCD_RS_AF, GPIO_AF_FSMC);

	//CS - > PG12	
	GPIO_PinAFConfig(BSP_LCD_CS_GPIO, BSP_LCD_CS_AF, GPIO_AF_FSMC);
	
	//WR - > PD5	
	//RD - > PD4
	GPIO_PinAFConfig(BSP_LCD_WRD_GPIO, BSP_LCD_WR_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_WRD_GPIO, BSP_LCD_RD_AF, GPIO_AF_FSMC);

	//D0 - > PD14	
	//D1 - > PD15 	
	//D2 - > PD0	
	//D3 - > PD1
	GPIO_PinAFConfig(BSP_LCD_D_D, BSP_LCD_D0_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_D, BSP_LCD_D1_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_D, BSP_LCD_D2_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_D, BSP_LCD_D3_AF, GPIO_AF_FSMC);

	//D4 - > PE7	
	//D5 - > PE8	
	//D6 - > PE9	
	//D7 - > PE10  
	//D8 - > PE11  
	//D9 - > PE12  
	//D10 - > PE13  
	//D11 - > PE14  
	//D12 - > PE15
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D4_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D5_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D6_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D7_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D8_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D9_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D10_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D11_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_E, BSP_LCD_D12_AF, GPIO_AF_FSMC);

	//D13 - > PD8	
	//D14 - > PD9	
	//D15 - > PD10
	GPIO_PinAFConfig(BSP_LCD_D_D, BSP_LCD_D13_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_D, BSP_LCD_D14_AF, GPIO_AF_FSMC);
	GPIO_PinAFConfig(BSP_LCD_D_D, BSP_LCD_D15_AF, GPIO_AF_FSMC);
}

void BSP_LCD_8080_Timing_Init(void)
{
	FSMC_NORSRAMTimingInitTypeDef ReadWrite_Timing;
	FSMC_NORSRAMTimingInitTypeDef Write_Timing;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStruct;

	RCC_AHB3PeriphClockCmd(BSP_LCD_TIMING_CLK, ENABLE);

	//时序
//	ReadWrite_Timing.FSMC_CLKDivision = 0;							/* 时钟分频因子 ,1-0xF ，若控制异步存储器，本参数无效 */
//	ReadWrite_Timing.FSMC_AccessMode = FSMC_AccessMode_A;			/* 设置访问模式 */
//	ReadWrite_Timing.FSMC_AddressSetupTime = 16;						/* 地址建立时间， 0-0xF 个 HCLK 周期 */
//	ReadWrite_Timing.FSMC_AddressHoldTime = 0;						/* 地址保持时间， 0-0xF 个 HCLK 周期 */
//	ReadWrite_Timing.FSMC_DataSetupTime = 60;						/* 数据建立时间， 0-0xF 个 HCLK 周期 */
//	ReadWrite_Timing.FSMC_DataLatency = 0;							/* 数据延迟时间，若控制异步存储器，本参数无效 */
//	ReadWrite_Timing.FSMC_BusTurnAroundDuration = 0;				/* 总线转换周期 ,0-0xF 个 HCLK 周期，在 NOR FLASH */
	ReadWrite_Timing.FSMC_CLKDivision = 0;							/* 时钟分频因子 ,1-0xF ，若控制异步存储器，本参数无效 */
	ReadWrite_Timing.FSMC_AccessMode = FSMC_AccessMode_B;			/* 设置访问模式 */
	ReadWrite_Timing.FSMC_AddressSetupTime = 4;						/* 地址建立时间， 0-0xF 个 HCLK 周期 */
	ReadWrite_Timing.FSMC_AddressHoldTime = 0;						/* 地址保持时间， 0-0xF 个 HCLK 周期 */
	ReadWrite_Timing.FSMC_DataSetupTime = 8;						/* 数据建立时间， 0-0xF 个 HCLK 周期 */
	ReadWrite_Timing.FSMC_DataLatency = 0;							/* 数据延迟时间，若控制异步存储器，本参数无效 */
	ReadWrite_Timing.FSMC_BusTurnAroundDuration = 0;				/* 总线转换周期 ,0-0xF 个 HCLK 周期，在 NOR FLASH */


//	Write_Timing.FSMC_CLKDivision = 0;							/* 时钟分频因子 ,1-0xF ，若控制异步存储器，本参数无效 */
//	Write_Timing.FSMC_AccessMode = FSMC_AccessMode_A;			/* 设置访问模式 */
//	Write_Timing.FSMC_AddressSetupTime = 9;						/* 地址建立时间， 0-0xF 个 HCLK 周期 */
//	Write_Timing.FSMC_AddressHoldTime = 0;						/* 地址保持时间， 0-0xF 个 HCLK 周期 */
//	Write_Timing.FSMC_DataSetupTime = 8;						/* 数据建立时间， 0-0xF 个 HCLK 周期 */
//	Write_Timing.FSMC_DataLatency = 0;							/* 数据延迟时间，若控制异步存储器，本参数无效 */
//	Write_Timing.FSMC_BusTurnAroundDuration = 0;				/* 总线转换周期 ,0-0xF 个 HCLK 周期，在 NOR FLASH */
	Write_Timing.FSMC_CLKDivision = 0;							/* 时钟分频因子 ,1-0xF ，若控制异步存储器，本参数无效 */
	Write_Timing.FSMC_AccessMode = FSMC_AccessMode_A;			/* 设置访问模式 */
	Write_Timing.FSMC_AddressSetupTime = 0;						/* 地址建立时间， 0-0xF 个 HCLK 周期 */
	Write_Timing.FSMC_AddressHoldTime = 0;						/* 地址保持时间， 0-0xF 个 HCLK 周期 */
	Write_Timing.FSMC_DataSetupTime = 0;						/* 数据建立时间， 0-0xF 个 HCLK 周期 */
	Write_Timing.FSMC_DataLatency = 0;							/* 数据延迟时间，若控制异步存储器，本参数无效 */
	Write_Timing.FSMC_BusTurnAroundDuration = 0;				/* 总线转换周期 ,0-0xF 个 HCLK 周期，在 NOR FLASH */

	FSMC_NORSRAMInitStruct.FSMC_Bank = FSMC_Bank1_NORSRAM4;								/* 设置要控制的 Bank 区域 */
	FSMC_NORSRAMInitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 			/* 设置地址总线与数据总线是否复用 */
//	FSMC_NORSRAMInitStruct.FSMC_MemoryType = FSMC_MemoryType_SRAM; 						/* 设置存储器的类型 */
	FSMC_NORSRAMInitStruct.FSMC_MemoryType = FSMC_MemoryType_NOR; 						/* 设置存储器的类型 */
	FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; 			/* 设置存储器的数据宽度 */
	FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; 		/* 设置是否支持突发访问模式，只支持同步类型的存储器      */
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable; 		/* 设置是否使能在同步传输时的等待信号， */
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low; 		/* 设置等待信号的极性 */
	FSMC_NORSRAMInitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable; 						/* 设置是否支持对齐的突发模式 */
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; /* 配置等待信号在等待前有效还是等待期间有效 */
	FSMC_NORSRAMInitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable; 			/* 设置是否写使能 */
	FSMC_NORSRAMInitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 					/* 设置是否使能等待状态插入 */
	FSMC_NORSRAMInitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 				/* 设置是否使能扩展模式 */
	FSMC_NORSRAMInitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 					/* 设置是否使能写突发操作 */
	/* 当不使用扩展模式时，本参数用于配置读写时序，否则用于配置读时序 */
	FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct = &ReadWrite_Timing;
	FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct = &ReadWrite_Timing;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1 

}



void BSP_LCD_WR_Cmd(uint16_t cmd)
{
	BSP_LCD_CMD = cmd;
}

void BSP_LCD_WR_Data(uint16_t w_data)
{
	BSP_LCD_DATA = w_data;	
}

uint16_t BSP_LCD_RD_Data(void)
{
	uint16_t r_data;

	r_data = BSP_LCD_DATA & 0xffff;

	return r_data;
}


