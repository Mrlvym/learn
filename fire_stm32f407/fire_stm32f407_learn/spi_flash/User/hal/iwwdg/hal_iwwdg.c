#include "hal_iwwdg.h"
#include "hal_debug_usart.h"

//iwdg
void Hal_IWDG_Init(uint8_t pre, uint16_t count)
{
	hal_assert_param(HAL_IS_IWDG_PRE(pre));
	
	Bsp_IWDG_Init(pre, count);
}

void Hal_IWDG_Feed(void)
{
	Hal_Debug(INFO_LEVEL, "IWDGÎ¹¹· ... \r\n");

	Bsp_IWDG_Feed();
}

uint8_t Hal_IWDG_GetRst_Flag(uint8_t flag_mask)
{
	hal_assert_param(HAL_IS_RCC_FLAG(flag_mask));
	
	return Bsp_IWDG_GetRst_Flag(flag_mask);
}

void Hal_IWDG_ClearRst_Flag(void)
{
	Bsp_IWDG_ClearRst_Flag();
}

//wwdg
void Hal_WWDG_Init(uint32_t pre, uint8_t count, uint8_t win_val)
{
	hal_assert_param(HAL_IS_WWDG_PRE(pre));
	hal_assert_param(HAL_IS_COUNT_VAL(count));

	Bsp_WWDG_Init(pre, count, win_val);
}

void Hal_WWDG_Feed(uint8_t count)
{
	hal_assert_param(HAL_IS_COUNT_VAL(count));

	Bsp_WWDG_Feed(count);
}

uint8_t Hal_WWDG_GetRst_Flag(uint8_t flag_mask)
{
	hal_assert_param(HAL_IS_RCC_FLAG(flag_mask));
	
	return Bsp_WWDG_GetRst_Flag(flag_mask);
}


void Hal_WWDG_ClearRst_Flag(void)
{
	Bsp_WWDG_ClearRst_Flag();
}

uint8_t Hal_WWDG_Get_Counter(void)
{
	return Bsp_WWDG_Get_Counter();
}


uint8_t Hal_WWDG_Get_WinVal(void)
{
	return Bsp_WWDG_Get_WinVal();
}



