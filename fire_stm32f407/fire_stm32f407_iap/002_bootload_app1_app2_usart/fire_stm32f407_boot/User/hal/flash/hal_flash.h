#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__
#include "bsp_flash.h"

#define HAL_FLASH_START_ADRESS		BSP_FLASH_START_ADRESS
#define HAL_FLASH_SIZE				BSP_FLASH_SIZE
#define HAL_FLASH_END_ADDRESS		BSP_FLASH_END_ADDRESS


int Hal_Flash_Read_Len(uint32_t addr,  uint8_t *buf, uint32_t size);

int Hal_Flash_Write_Len(uint32_t addr, uint8_t *buf, uint32_t size);

uint32_t Hal_Get_Flash_Sector(uint32_t addr);

int Hal_Earse_Flash_Size(uint32_t addr, uint32_t size);


#endif

