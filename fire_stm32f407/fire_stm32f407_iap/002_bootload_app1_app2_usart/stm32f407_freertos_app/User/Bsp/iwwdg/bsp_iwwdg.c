#include "bsp_iwwdg.h"

//IWDG相关
void Bsp_IWDG_Init(uint8_t pre, uint16_t count)
{
	IWDG_WriteAccessCmd(BSP_IWDG_WriteAccess_Enable);

	IWDG_SetPrescaler(pre);

	IWDG_SetReload(count);

	IWDG_ReloadCounter();

	IWDG_Enable();
}

void Bsp_IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

uint8_t Bsp_IWDG_GetRst_Flag(uint8_t flag_mask)
{
	return RCC_GetFlagStatus(flag_mask);	
}

void Bsp_IWDG_ClearRst_Flag(void)
{
	RCC_ClearFlag();
}

//WWDG相关
void Bsp_WWDG_Init(uint32_t pre, uint8_t count, uint8_t win_val)
{
	//开启时钟pclk1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	

	WWDG_SetCounter(count);

	//分频器
	WWDG_SetPrescaler(pre);

	//上窗口值
	WWDG_SetWindowValue(win_val);

	WWDG_Enable(count);
	
	//提前唤醒中断配置
	Bsp_WWDG_NVIC_Config();

	WWDG_ClearFlag();

	WWDG_EnableIT();

}

void Bsp_WWDG_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);
}

void Bsp_WWDG_Feed(uint8_t count)
{
	WWDG_SetCounter(count);
}

uint8_t Bsp_WWDG_GetRst_Flag(uint8_t flag_mask)
{
	return RCC_GetFlagStatus(flag_mask);	
}

void Bsp_WWDG_ClearRst_Flag(void)
{
	RCC_ClearFlag();
}

uint8_t Bsp_WWDG_Get_Counter(void)
{
	return (uint8_t)(WWDG->CR & 0x7F);
}

uint8_t Bsp_WWDG_Get_WinVal(void)
{
	return (uint8_t)(WWDG->CFR & 0X7F);
}

