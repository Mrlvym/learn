#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__
#include "stm32f4xx.h"

void Bsp_Key_Init(void);
void Bsp_Key_Exit_Init(void);
uint8_t Bsp_Key0_Read_Bit(void);
uint8_t Bsp_Key2_Read_Bit(void);


#endif

