#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__
#include "stdint.h"

#define BSP_FLASH_START_ADRESS		((uint32_t)0x08000000)
#define BSP_FLASH_SIZE				(256*1024)
#define BSP_FLASH_END_ADDRESS		((uint32_t)(BSP_FLASH_START_ADRESS + BSP_FLASH_SIZE))

#define FLASH_SECTOR_16K		0x4000
#define FLASH_SECTOR_64K		0x10000
#define FLASH_SECTOR_128K		0x20000

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */
#define ADDR_FLASH_SECTOR_End   ((uint32_t)0x08100000) /* Base @ of Sector End */


void Bsp_Flash_Read_Len(uint32_t addr,  uint8_t *buf, uint32_t size);


int Bsp_Flash_Write_Len(uint32_t addr, uint8_t *buf, uint32_t size);


uint32_t Bsp_Get_Flash_Sector(uint32_t addr);

void Bsp_Earse_Flash_Size(uint32_t addr, uint32_t size);


#endif

