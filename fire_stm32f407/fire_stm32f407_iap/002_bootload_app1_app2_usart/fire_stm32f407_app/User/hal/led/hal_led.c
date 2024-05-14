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

static uint8_t turn_flag = 0;
void Hal_Led_turn(uint8_t LED)
{
	if(turn_flag == 0)
	{
		Hal_Led_on(LED);
		turn_flag = 1;
	}
	else if(turn_flag == 1)
	{
		Hal_Led_off(LED);
		turn_flag = 0;
	}
}

