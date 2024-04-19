#ifndef __APP_SRAM_H__
#define __APP_SRAM_H__
#include "stdint.h"

#define APP_Bank1_SRAM3_ADDR	HAL_Bank1_SRAM3_ADDR
#define APP_Bank1_SRAM3_SIZE	HAL_Bank1_SRAM3_SIZE	

void APP_Sram_Init(void);

void APP_Sram_Memory_Test(void);

uint8_t APP_Sram_Test(void);
void APP_Sram_Task(void);



#endif

