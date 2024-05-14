#include "app_exit.h"
#include "app_iap.h"

#include "hal_debug_usart.h"


extern uint8_t App_Buf[APP_BUF_SIZE];
void App_Debug_Usart_RXEN_IRQHandler(void)
{
	static uint32_t app_buf_count = 0;
	uint8_t r_val = 0;

	r_val = (uint8_t)Hal_Debug_Usart_RecData();

	App_Buf[app_buf_count] = r_val;

	app_buf_count++;
}


