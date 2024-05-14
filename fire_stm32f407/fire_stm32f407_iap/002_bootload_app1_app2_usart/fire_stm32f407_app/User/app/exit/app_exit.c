#include "app_exit.h"
#include "app_iap.h"

#include "hal_debug_usart.h"

#include "ringbuffer.h"


extern RingBuffer read_rb;

void App_Debug_Usart_RXEN_IRQHandler(void)
{
	uint8_t r_val = 0;

	r_val = (uint8_t)Hal_Debug_Usart_RecData();

	if(rb_can_write(&read_rb) > 0){
		rb_write(&read_rb, (uint8_t *)&r_val, 1);
	}
}


