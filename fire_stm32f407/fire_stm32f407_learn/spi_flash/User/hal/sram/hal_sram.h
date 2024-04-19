#ifndef __HAL_SRAM_H__
#define __HAL_SRAM_H__
#include "stdint.h"
#include "bsp_sram.h"


#define HAL_Bank1_SRAM3_ADDR	Bank1_SRAM3_ADDR
#define HAL_Bank1_SRAM3_SIZE	Bank1_SRAM3_SIZE

void HAL_Sram_Init(void);

void HAL_Sram_ReadByte(uint32_t addr_byte, uint8_t *data_byte, uint32_t len);
void HAL_Sram_WriteByte(uint32_t addr_byte, uint8_t *data_byte, uint32_t len);

void HAL_Sram_ReadShort(uint32_t addr_short, uint16_t *data_short, uint32_t len);
void HAL_Sram_WriteShort(uint32_t addr_short, uint16_t *data_short, uint32_t len);

void HAL_Sram_ReadWord(uint32_t addr_word, uint32_t *data_word, uint32_t len);
void HAL_Sram_WriteWord(uint32_t addr_word, uint32_t *data_word, uint32_t len);


#endif

