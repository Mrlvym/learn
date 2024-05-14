 #include "hal_flash.h"
 #include "hal_debug_usart.h"

int Hal_Flash_Read_Len(uint32_t addr,  uint8_t *buf, uint32_t size)
{
	if(size < 1){
		Hal_Debug(INFO_LEVEL, "Err size < 1 ... \r\n");
		return -1;
	}
	
	if((addr+size) > HAL_FLASH_END_ADDRESS){
		Hal_Debug(INFO_LEVEL, "size is to large ... \r\n");		
		return -2;
	}
	
	Bsp_Flash_Read_Len(addr, buf, size);

	return 0;
}

int Hal_Flash_Write_Len(uint32_t addr, uint8_t *buf, uint32_t size)
{
	int ret = 0;
	
	if(size < 1){
		Hal_Debug(INFO_LEVEL, "Err size < 1 ... \r\n");
		return -1;
	}
	
	if((addr+size) > HAL_FLASH_END_ADDRESS){
		Hal_Debug(INFO_LEVEL, "size is to large ... \r\n");		
	}

	ret = Bsp_Flash_Write_Len(addr, buf, size);

	return ret;
}

uint32_t Hal_Get_Flash_Sector(uint32_t addr)
{
	return Bsp_Get_Flash_Sector(addr);
}

int Hal_Earse_Flash_Size(uint32_t addr, uint32_t size)
{
	if(size < 1){
		Hal_Debug(INFO_LEVEL, "Err size < 1 ... \r\n");
		return -1;
	}
	
	if((addr+size) > HAL_FLASH_END_ADDRESS){
		Hal_Debug(INFO_LEVEL, "size is to large ... \r\n");		
	}

	Bsp_Earse_Flash_Size(addr, size);

	return 0;
}


