#include "app_led.h"
#include "hal_led.h"
#include "hal_delay.h"


void App_Led_Test_Task(void)
{
	
	Hal_Led_Init();
	
	while(1)
	{
//		Hal_Delay_ms(1000);
//		printf("111111111111 \r\n");
		Hal_Led_on(LED0);
		Hal_Delay_ms(1000);
		
		Hal_Led_off(LED0);
		Hal_Delay_ms(1000);

		Hal_Led_on(LED1);
		Hal_Delay_ms(1000);
		
		Hal_Led_off(LED1);
		Hal_Delay_ms(1000);
	
		Hal_Led_on(LED_ALL);
		Hal_Delay_ms(1000);
		
		Hal_Led_off(LED_ALL);
		Hal_Delay_ms(1000);
	}
}
