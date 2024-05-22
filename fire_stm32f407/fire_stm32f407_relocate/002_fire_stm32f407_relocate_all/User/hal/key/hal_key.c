#include "hal_key.h"


void Hal_Key_Init(void)
{
	Bsp_Key_Init();
}

void Hal_Key_Exit_Init(void)
{
	Bsp_Key_Exit_Init();
}

uint8_t Hal_Key0_Read_Bit(void)
{
	return Bsp_Key0_Read_Bit();
}

uint8_t Hal_Key2_Read_Bit(void)
{
	return Bsp_Key2_Read_Bit();
}

uint8_t Hal_Key_Scan(uint8_t KEYx, uint8_t mode)
{
	uint8_t  ret = 0;
	uint32_t i = 0;

	if(KEYx == HAL_KEY0){
		if(!Hal_Key0_Read_Bit()){
			for(i = 0; i < 40000; i++){};
			if(!Hal_Key0_Read_Bit()){
				ret = 1;

				if(mode == HAL_KEY_ONCE){
					while(!Hal_Key0_Read_Bit());
				}
			}		
		}
	}
	else if(KEYx == HAL_KEY2){
		if(!Hal_Key2_Read_Bit()){
			for(i = 0; i < 40000; i++){};
			if(!Hal_Key2_Read_Bit()){
				ret = 1;

				if(mode == HAL_KEY_ONCE){
					while(!Hal_Key2_Read_Bit());
				}
			}		
		}
	}

	return ret;
}


