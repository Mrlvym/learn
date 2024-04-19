#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__
#include "bsp_key.h"

#define HAL_KEY0	1
#define HAL_KEY2	2

#define HAL_KEY_ONCE		1
#define HAL_KEY_CONTINUE	2

void Hal_Key_Init(void);

void Hal_Key_Exit_Init(void);

uint8_t Hal_Key0_Read_Bit(void);

uint8_t Hal_Key2_Read_Bit(void);

uint8_t Hal_Key_Scan(uint8_t KEYx, uint8_t mode);

#endif

