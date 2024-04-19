#include "hal_nt35510_lcd.h"
#include "bsp_nt35510_lcd.h"

void Hal_LCD_Init(void)
{
	BSP_LCD_BL_Init();

	BSP_LCD_8080_Gpio_Init();

	BSP_LCD_8080_Timing_Init();
}

void Hal_LCD_BL_On(void)
{
	BSP_LCD_BL_On();
}

void Hal_LCD_BL_Off(void)
{
	BSP_LCD_BL_Off();
}

void Hal_LCD_WR_Cmd(uint16_t cmd)
{
	BSP_LCD_WR_Cmd(cmd);
}

void Hal_LCD_WR_Data(uint16_t w_data)
{
	BSP_LCD_WR_Data(w_data);
}

uint16_t Hal_LCD_RD_Data(void)
{
	uint16_t r_data;

	r_data = BSP_LCD_RD_Data();

	return r_data;
}


void Hal_LCD_WriteReg(uint16_t lcd_w_reg, uint16_t lcd_reg_w_val)
{
	Hal_LCD_WR_Cmd(lcd_w_reg);

	Hal_LCD_WR_Data(lcd_reg_w_val);
}

uint16_t Hal_LCD_ReadReg(uint16_t lcd_r_reg)
{
	uint16_t lcd_r_data;

	Hal_LCD_WR_Cmd(lcd_r_reg);

	lcd_r_data = Hal_LCD_RD_Data();

	return lcd_r_data;
}

