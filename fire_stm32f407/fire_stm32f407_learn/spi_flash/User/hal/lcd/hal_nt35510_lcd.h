#ifndef __HAL_NT35510_LCD_H__
#define __HAL_NT35510_LCD_H__
#include "stdint.h"

void Hal_LCD_Init(void);

void Hal_LCD_BL_On(void);
void Hal_LCD_BL_Off(void);

void Hal_LCD_WR_Cmd(uint16_t cmd);
void Hal_LCD_WR_Data(uint16_t w_data);
uint16_t Hal_LCD_RD_Data(void);

void Hal_LCD_WriteReg(uint16_t lcd_w_reg, uint16_t lcd_reg_w_val);
uint16_t Hal_LCD_ReadReg(uint16_t lcd_r_reg);

#endif

