#include "app_exit.h"

#include "hal_debug_usart.h"
#include "hal_key.h"


//extern RingBuffer read_rb;

void App_Debug_Usart_RXEN_IRQHandler(void)
{
//	uint8_t r_val = 0;

//	r_val = (uint8_t)Hal_Debug_Usart_RecData();

//	if(rb_can_write(&read_rb) > 0){
//		rb_write(&read_rb, (uint8_t *)&r_val, 1);
//	}
}

void App_Key0_IRQHandler(void)
{
		if(!Hal_Key0_Read_Bit())
		{
				Hal_Debug(PRINTF, "This is Key 0 ... \r\n");
		}
}

void App_Key2_IRQHandler(void)
{
		if(!Hal_Key2_Read_Bit())
		{
				Hal_Debug(PRINTF, "This is Key 2 ... \r\n");
		}
}

