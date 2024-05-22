#include "hal_debug_usart.h"


void Hal_Debug_Usart_Config(void)
{
	Debug_USART_Config();

	Debug_USART_NVIC_Cfg();
}


uint16_t Hal_Debug_Usart_RecData(void)
{
	return Debug_USART_RecData();
}


void Hal_Debug_Usart_SendData(uint16_t data)
{
	Debug_USART_SendData(data);
}


