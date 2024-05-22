#include "app_led.h"
#include "hal_led.h"
#include "hal_delay.h"
#include "hal_debug_usart.h"

#include "app_iap.h"

#include "protocol.h"


void App_Led_Test_Task(void)
{
	static uint32_t count = 0;
	SystemParam_t mSysParam;
	
#if (defined(APP0_TEST))
	Hal_Debug(PRINTF, "This is App0 LED Task ... \r\n");
#endif

#if (defined(APP1_TEST))
	Hal_Debug(PRINTF, "This is App1 LED Task ... \r\n");
#endif

	Hal_Led_Init();

	System_ParamReadCheckUpdate(&mSysParam);
	
	while(1)
	{
		mh_serial_messagehandle();
	
		SystemParam_IAP(&mSysParam);

		if(Iap_Flag_False == App_Iap_Get_Start_Flag()){	
			count++;
			if(count > 0x5ffff){
			#if (defined(APP0_TEST))
				Hal_Debug(PRINTF, "This is App0 22 ... \r\n");
			#endif
			
			#if (defined(APP1_TEST))
				Hal_Debug(PRINTF, "This is App1 22 ... \r\n");
			#endif
				count = 0;
			}
		}
		
//		Hal_Led_on(LED0);
//		Hal_Delay_ms(1000);
//		
//		Hal_Led_off(LED0);
//		Hal_Delay_ms(1000);
//
//		Hal_Led_on(LED1);
//		Hal_Delay_ms(1000);
//		
//		Hal_Led_off(LED1);
//		Hal_Delay_ms(1000);
//	
//		Hal_Led_on(LED_ALL);
//		Hal_Delay_ms(1000);
//		
//		Hal_Led_off(LED_ALL);
//		Hal_Delay_ms(1000);
	}
}
