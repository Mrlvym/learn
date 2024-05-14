#include "freertos_iap.h"

#include "string.h"
#include "stdlib.h"

#include "hal_flash.h"
#include "hal_debug_usart.h"
#include "hal_iap.h"

#include "FreeRTOS.h"
#include "task.h"

#include "protocol.h"


static iap_info_t gApp_Iap_Info;

void App_Iap_Info_Init(void)
{
	memset(&gApp_Iap_Info, 0x00, sizeof(iap_info_t));
}

void App_Iap_Set_Start_Flag(uint8_t flag)
{
	gApp_Iap_Info.iap_start_flag = flag;
}

uint8_t App_Iap_Get_Start_Flag(void)
{
	return gApp_Iap_Info.iap_start_flag;
}

void App_Iap_Set_Stop_Flag(uint8_t flag)
{
	gApp_Iap_Info.iap_stop_flag = flag;
}

uint8_t App_Iap_Get_Stop_Flag(void)
{
	return gApp_Iap_Info.iap_stop_flag;
}

void App_Iap_Set_DataLen(uint32_t data_len)
{
	gApp_Iap_Info.data_len = data_len;
}

uint32_t App_Iap_Get_DataLen(void)
{
	return gApp_Iap_Info.data_len;
}

void App_Iap_Set_CheckSum(uint8_t check_sum)
{
	gApp_Iap_Info.check_sum = check_sum;
}

uint8_t App_Iap_Get_CheckSum(void)
{
	return gApp_Iap_Info.check_sum; 
}

void App_Iap_Set_pData(uint8_t *pData)
{
	gApp_Iap_Info.pData = pData;
}

uint8_t *App_Iap_Get_pData(void)
{
	return gApp_Iap_Info.pData;
}

void App_Iap_Deal_Usart_Buf(uint8_t *buf, uint8_t len)
{
	if(0xEF != buf[0] || 0xAA != buf[1]){
		return ;
	}	

	switch(buf[2]){
		case Iap_Start_Cmd:
			App_Iap_Start();
			break;
		case Iap_Stop_Cmd:
			App_Iap_Stop();
			break;
		case Iap_Len_Cmd:
			App_Iap_Len(buf);
			break;
		case Iap_CheckSum_Cmd:
			App_Iap_CheckSum(buf);
			break;
		case Iap_Data_Cmd:
			App_Iap_Data(buf);
			break;
		default:
			break;

	}
}

void App_Iap_Start(void)
{
	App_Iap_Set_Start_Flag(Iap_Flag_True);
}

void App_Iap_Stop(void)
{
	App_Iap_Set_Stop_Flag(Iap_Flag_True);	
}

void App_Iap_Len(uint8_t *buf)
{
	uint32_t data_len = 0;
	uint8_t *pData = NULL;

	data_len = (uint32_t)((buf[5] << 24) | (buf[6] << 16) | (buf[7] << 8) | buf[8]);

//	pData = (uint8_t *)malloc(sizeof(uint8_t) * data_len);	
	
	App_Iap_Set_DataLen(data_len);
	App_Iap_Set_pData(pData);
}

void App_Iap_CheckSum(uint8_t *buf)
{
	App_Iap_Set_CheckSum(buf[5]);
}

void App_Iap_Data(uint8_t *buf)
{
	static uint8_t data_frist_flag = 0;
	uint8_t *pData = NULL;
	uint16_t len = 0;
	static uint32_t cur_data_len = 0;
	uint32_t pCheckAppAddr = 0;

	len = (uint16_t)((buf[3]<<8) | buf[4]);

	if(data_frist_flag == 0)
	{	
		#if (defined(APP0_TEST))
			pCheckAppAddr = *(uint32_t *)&buf[9];
			if(APP1_START_ADDR != (pCheckAppAddr & APP1_START_ADDR)){//检查固件和地址是否匹配，不匹配也不能更新
				Hal_Debug(PRINTF, "App0 Check App Addr error ... \r\n");

				Hal_Debug(PRINTF, "App device iap reboot\r\n");
				Hal_NVIC_SystemReset();
			}
			Hal_Earse_Flash_Size(APP1_START_ADDR, App_Iap_Get_DataLen());
			
			App_Iap_Set_pData((uint8_t *)APP1_START_ADDR);
		#endif

		#if (defined(APP1_TEST))
			pCheckAppAddr = *(uint32_t *)&buf[9];
			if(APP0_START_ADDR != (pCheckAppAddr & APP0_START_ADDR)){//检查固件和地址是否匹配，不匹配也不能更新
				Hal_Debug(PRINTF, "App1 Check App Addr error ... \r\n");

				Hal_Debug(PRINTF, "App device iap reboot\r\n");
				Hal_NVIC_SystemReset();
			}
			Hal_Earse_Flash_Size(APP0_START_ADDR, App_Iap_Get_DataLen());

			App_Iap_Set_pData((uint8_t *)APP0_START_ADDR);
		#endif
		
		data_frist_flag = 1;
	}

	pData = App_Iap_Get_pData();

	Hal_Flash_Write_Len(((uint32_t)pData)+cur_data_len, buf+5, len);
	cur_data_len += len;
}

void System_ParamReadCheckUpdate(SystemParam_t *pData)
{
	uint32_t flash_update = 0;
 
	Hal_Flash_Read_Len((uint32_t)SYSPARAM_ADDR, (uint8_t *)pData, sizeof(SystemParam_t));

#if (defined(APP0_TEST))
	/* APP0的软件版本号与固件内部软件版本号不一致，更新版本号并写入Flash */
	if(pData->Application0_Version != APPLICATION0_VERSION)
	{
		flash_update = 1;
		Hal_Debug(PRINTF, "App Update APP0 Software Version : 0x%08X\r\n", APPLICATION0_VERSION);
		pData->Application0_Version = APPLICATION0_VERSION;
	}
 
	/* APP0 启动了，但是APP0的状态是错误标志.
	 * 可能是由于更新完成后，第一次启动标记的，
	 * 修正为APPLICATION_NORMAL，重新写入flash
	 * */
	if(pData->Application0_Status != APPLICATION_NORMAL)
	{
		flash_update = 1;
		Hal_Debug(PRINTF, "App Update APP0 Status, APPLICATION_NORMAL\r\n");
		pData->Application0_Status = APPLICATION_NORMAL;
		pData->Application1_Version = APPLICATION0_VERSION-1;
	}
#endif

#if (defined(APP1_TEST))
		/* APP1的软件版本号与固件内部软件版本号不一致，更新版本号并写入Flash */
		if(pData->Application1_Version != APPLICATION1_VERSION)
		{
			flash_update = 1;
			Hal_Debug(PRINTF, "App Update APP1 Software Version : 0x%08X\r\n", APPLICATION1_VERSION);
			pData->Application1_Version = APPLICATION1_VERSION;
		}
	 
		/* APP1 启动了，但是APP1的状态是错误标志.
		 * 可能是由于更新完成后，第一次启动标记的，
		 * 修正为APPLICATION_NORMAL，重新写入flash
		 * */
		if(pData->Application1_Status != APPLICATION_NORMAL)
		{
			flash_update = 1;
			Hal_Debug(PRINTF, "App Update APP1 Status, APPLICATION_NORMAL\r\n");
			pData->Application1_Status = APPLICATION_NORMAL;
			pData->Application0_Version = APPLICATION1_VERSION-1;
		}
#endif

 
	/* 如果flash_update 等于 1， 即flash需要更新主存储区和备份区的系统参数数据
	 * 更新数据后，需要重启
	 * 注意：更新数据时，必须擦除一个扇区后写入完成后，才能更新另一个区域数据，否则有概率丢失两个扇区数据
	 * */
	if(flash_update == 1)
	{
		pData->SystemParamCheckSum = App_SysParam_CheckSum((uint8_t *)pData, sizeof(SystemParam_t));

		App_Update_SysParam2Flash((uint32_t)SYSPARAM_ADDR, pData);
		App_Update_SysParam2Flash((uint32_t)SYSPARAM_BACKUP_ADDR, pData);

		Hal_Debug(PRINTF, "App device ParamReadCheck reboot\r\n");
		Hal_NVIC_SystemReset();
	}
}

void SystemParam_IAP(SystemParam_t *pData)
{
//	uint8_t *pAppData = NULL;
	uint32_t data_len = 0;
//	uint8_t check_sum = 0, check_sum1 = 0;
//
//	uint32_t *pCheckAppAddr = NULL;

	if(Iap_Flag_True == App_Iap_Get_Stop_Flag()){//更新固件接收完毕
//		pAppData = App_Iap_Get_pData();
		data_len = App_Iap_Get_DataLen();
//		check_sum = App_Iap_Get_CheckSum();
//		
//		check_sum1 = App_Iap_Data_CheckSum(pAppData, data_len);
//		if(check_sum == check_sum1){//检查校验和

		#if (defined(APP0_TEST))

//			pCheckAppAddr = (uint32_t *)pAppData;
//			if(APP1_START_ADDR != (pCheckAppAddr[1] & APP1_START_ADDR)){//检查固件和地址是否匹配，不匹配也不能更新
//				Hal_Debug(PRINTF, "App0 Check App Addr error ... \r\n");
//
//				//释放存固件的内存
//				free(pAppData);
//				App_Iap_Set_pData((uint8_t *)NULL);
//
//				Hal_Debug(PRINTF, "App device iap reboot\r\n");
//				Hal_NVIC_SystemReset();
//			}
//		
//			Hal_Earse_Flash_Size(APP1_START_ADDR, data_len);
//			Hal_Flash_Write_Len(APP1_START_ADDR, pAppData, data_len);

			Hal_Debug(PRINTF, "App0 Programming Completed Successfully! %ddBtye\r\n", data_len);
			Hal_Debug(PRINTF, "App0 Update system parameters\r\n");
			
			pData->Application1_Version = 0xFFFFFFFF;
			pData->Application1_Status  = APPLICATION_UPDATED;
		#endif

		#if (defined(APP1_TEST))

//			pCheckAppAddr = (uint32_t *)pAppData;
//			if(APP0_START_ADDR != (pCheckAppAddr[1] & APP0_START_ADDR)){//检查固件和地址是否匹配，不匹配也不能更新
//				Hal_Debug(PRINTF, "App1 Check App Addr error ... \r\n");
//
//				//释放存固件的内存
//				free(pAppData);
//				App_Iap_Set_pData((uint8_t *)NULL);
//
//				Hal_Debug(PRINTF, "App device iap reboot\r\n");
//				Hal_NVIC_SystemReset();
//			}
//			
//			Hal_Earse_Flash_Size(APP0_START_ADDR, data_len);
//			Hal_Flash_Write_Len(APP0_START_ADDR, pAppData, data_len);

			Hal_Debug(PRINTF, "App1 Programming Completed Successfully! %ddBtye\r\n", data_len);
			Hal_Debug(PRINTF, "App1 Update system parameters\r\n");
			
			pData->Application0_Version = 0xFFFFFFFF;
			pData->Application0_Status  = APPLICATION_UPDATED;
		#endif
			
			pData->SystemParamCheckSum = App_SysParam_CheckSum((uint8_t *)pData, sizeof(SystemParam_t));
			
			App_Update_SysParam2Flash((uint32_t)SYSPARAM_ADDR, pData);
			App_Update_SysParam2Flash((uint32_t)SYSPARAM_BACKUP_ADDR, pData);

//			//释放存固件的内存
//			free(pAppData);
//			App_Iap_Set_pData((uint8_t *)NULL);

			Hal_Debug(PRINTF, "App device iap reboot\r\n");
			Hal_NVIC_SystemReset();
//		}
//		else
//		{
//			//释放存固件的内存
//			free(pAppData);
//			App_Iap_Set_pData((uint8_t *)NULL);
//		}
	}
}

/* IAP  任务句柄 */
static TaskHandle_t IAP_Task_Handle = NULL;


static void IAP_Task(void* pvParameters);/* IAP_Task  任务实现 */


void FreeRTOS_IAP_Task(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(	(TaskFunction_t) IAP_Task,
							(const char * )"IAP_Task",
							(uint16_t) 128,
							(void * )NULL,
							(UBaseType_t )3,
							(TaskHandle_t * )&IAP_Task_Handle);

	if(pdPASS == xReturn)
		Hal_Debug(INFO_LEVEL, "IAP_Task Creat success ! \r\n");
}

static void IAP_Task(void* pvParameters)/* IAP_Task  任务实现 */
{
	SystemParam_t sysparam;

	System_ParamReadCheckUpdate(&sysparam);

	while(1)
	{
		mh_serial_messagehandle();

		SystemParam_IAP(&sysparam);
		
		vTaskDelay(1);
	}
}



void App_Update_SysParam2Flash(uint32_t addr, SystemParam_t *pData)
{
	Hal_Earse_Flash_Size(addr, sizeof(SystemParam_t));

	Hal_Flash_Write_Len(addr, (uint8_t *)pData, sizeof(SystemParam_t));
}


uint8_t App_SysParam_CheckSum(uint8_t *buf, uint32_t len)
{
	uint32_t i = 0;
	uint8_t checkSum = 0;

	for(i = 0; i < len-1; i++)
	{
		checkSum += buf[i];
	}

	return checkSum;
}

uint8_t App_Iap_Data_CheckSum(uint8_t *buf, uint32_t len)
{
	uint32_t i = 0;
	uint8_t checkSum = 0;

	for(i = 0; i < len-1; i++)
	{
		checkSum += buf[i];
	}

	return checkSum;
}

