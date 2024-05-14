#include "bsp_flash.h"
#include "sys.h"


void Bsp_Flash_Read_Len(uint32_t addr,  uint8_t *buf, uint32_t size)
{
	uint32_t i = 0;

	for(i = 0; i < size; i++, buf++, addr++)
	{
		*buf = (*(volatile uint8_t *)addr);
	}
}

int Bsp_Flash_Write_Len(uint32_t addr, uint8_t *buf, uint32_t size)
{
	int ret = 0;
	uint32_t i = 0;
	

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_BSY);

	for(i = 0; i < size; i++, buf++, addr++)
	{
		if(FLASH_COMPLETE == FLASH_ProgramByte(addr, *buf)){
			if(*buf != *(uint8_t *)addr){
				ret = -1;
				break;
			}
		}
	}

	FLASH_Lock();

	if(i == size)
	{
		ret = size;
	}

	return ret;
}

uint32_t Bsp_Get_Flash_Sector(uint32_t addr)
{
	if(ADDR_FLASH_SECTOR_0 <= addr && ADDR_FLASH_SECTOR_1 > addr){
		return FLASH_Sector_0;
	}
	else if(ADDR_FLASH_SECTOR_1 <= addr && ADDR_FLASH_SECTOR_2 > addr){
		return FLASH_Sector_1;
	}
	else if(ADDR_FLASH_SECTOR_2 <= addr && ADDR_FLASH_SECTOR_3 > addr){
		return FLASH_Sector_2;
	}
	else if(ADDR_FLASH_SECTOR_3 <= addr && ADDR_FLASH_SECTOR_4 > addr){
		return FLASH_Sector_3;
	}
	else if(ADDR_FLASH_SECTOR_4 <= addr && ADDR_FLASH_SECTOR_5 > addr){
		return FLASH_Sector_4;
	}
	else if(ADDR_FLASH_SECTOR_5 <= addr && ADDR_FLASH_SECTOR_6 > addr){
		return FLASH_Sector_5;
	}
	else if(ADDR_FLASH_SECTOR_6 <= addr && ADDR_FLASH_SECTOR_7 > addr){
		return FLASH_Sector_6;
	}
	else if(ADDR_FLASH_SECTOR_7 <= addr && ADDR_FLASH_SECTOR_8 > addr){
		return FLASH_Sector_7;
	}
	else if(ADDR_FLASH_SECTOR_8 <= addr && ADDR_FLASH_SECTOR_9 > addr){
		return FLASH_Sector_8;
	}
	else if(ADDR_FLASH_SECTOR_9 <= addr && ADDR_FLASH_SECTOR_10 > addr){
		return FLASH_Sector_9;
	}
	else if(ADDR_FLASH_SECTOR_10 <= addr && ADDR_FLASH_SECTOR_11 > addr){
		return FLASH_Sector_10;
	}
	else if(ADDR_FLASH_SECTOR_11 <= addr && ADDR_FLASH_SECTOR_End > addr){
		return FLASH_Sector_11;
	}
	
	return 0;
}

void Bsp_Earse_Flash_Size(uint32_t addr, uint32_t size)
{
	uint32_t i = 0;
	uint32_t cur_sector = 0;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_BSY);

	i = addr;
	while(i < (addr+size)){
		cur_sector = Bsp_Get_Flash_Sector(i);
		FLASH_EraseSector(cur_sector, VoltageRange_3);

		switch(cur_sector){
			case FLASH_Sector_0:
			case FLASH_Sector_1:
			case FLASH_Sector_2:
			case FLASH_Sector_3:
			case FLASH_Sector_4:
				i += FLASH_SECTOR_16K;
				break;
			case FLASH_Sector_5:
				i += FLASH_SECTOR_64K;
				break;
			case FLASH_Sector_6:
			case FLASH_Sector_7:
			case FLASH_Sector_8:
			case FLASH_Sector_9:
			case FLASH_Sector_10:
			case FLASH_Sector_11:
				i += FLASH_SECTOR_64K;
				break;
			default:
				break;
		}
	}

	FLASH_Lock();
}


