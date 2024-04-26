#ifndef __APP_IAP_H__
#define __APP_IAP_H__

#define APP_BUF_SIZE		(40 * 1024)

#define COUNTDOWN_SECS		5

#define BOOT_START_ADDR		0x08000000
#define BOOT_FLASH_SIZE		0x4000	//16k
#define APP_START_ADDR		0x08004000
#define APP_FLASH_SIZE		0x3C000//240k



void App_IAP_Bsp_Init(void);

void App_IAP_Task(void);

void App_IAP_Show_BootInfo(void);


#endif

