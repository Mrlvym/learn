#include "hal_touch_gt911.h"
#include "bsp_touch_gt911.h"
#include "bsp_touch_iic.h"


void Hal_Touch_Gt911_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
	Bsp_Touch_Gt911_RD_Reg(reg, buf, len);
}

uint8_t Hal_Touch_Gt911_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
	return Bsp_Touch_Gt911_WR_Reg(reg, buf, len);
}

void Hal_Touch_Gt911_Init(void)
{
	CT_IIC_Init();

	Bsp_Touch_Gt911_Rst_Pin_Init();

	Bsp_Touch_Gt911_Int_Pin_Init();

	Bsp_Touch_Gt911_PowerOn();
}

void Hal_Touch_Gt911_Int_Irq_Cfg(void)
{
	Bsp_Touch_Gt911_Int_Irq_Cfg();
}



