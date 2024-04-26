#include "hal_led.h"
#include "bsp_led.h"

void Hal_Led_Init(void)
{
	Bsp_Led_Init();
}

void Hal_Led0_on(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);	
}

void Hal_Led0_off(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
}

void Hal_Led1_on(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_10);	
}

void Hal_Led1_off(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
}


void Hal_Led_on(uint8_t LED)
{
	switch(LED)
	{
		case LED0:
			Hal_Led0_on();
			break;
		case LED1:
			Hal_Led1_on();
			break;
		case LED_ALL:			
			Hal_Led0_on();
			Hal_Led1_on();
			break;
		default:
			break;
	}
}

void Hal_Led_off(uint8_t LED)
{
	switch(LED)
	{
		case LED0:
			Hal_Led0_off();
			break;
		case LED1:
			Hal_Led1_off();
			break;
		case LED_ALL:
			Hal_Led1_off();			
			Hal_Led0_off();
			break;
		default:
			break;
	}
}

void Hal_Led_turn(uint8_t LED)
{
	static uint8_t turn_flag1 = 0;
	static uint8_t turn_flag2 = 0;
	
	if(LED == LED0){		
		if(turn_flag1 == 0)
		{
			Hal_Led_on(LED);
			turn_flag1 = 1;
		}
		else if(turn_flag1 == 1)
		{
			Hal_Led_off(LED);
			turn_flag1 = 0;
		}
	}
	else if(LED == LED1){
		if(turn_flag2 == 0)
		{
			Hal_Led_on(LED);
			turn_flag2 = 1;
		}
		else if(turn_flag2 == 1)
		{
			Hal_Led_off(LED);
			turn_flag2 = 0;
		}
	}
	else if(LED == LED_ALL){
		if(turn_flag1 == 0)
		{
			Hal_Led_on(LED0);
			Hal_Led_on(LED1);
			turn_flag1 = 1;
		}
		else if(turn_flag1 == 1)
		{
			Hal_Led_off(LED0);
			Hal_Led_off(LED1);
			turn_flag1 = 0;
		}
	}
}

