#include "app_iap.h"

#include "string.h"

#include "hal_iap.h"
#include "hal_flash.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_delay.h"
#include "hal_debug_usart.h"


uint8_t App_Buf[APP_BUF_SIZE] = {0};

SystemParam_t sysParamDef = {
								0x01, //Device_id
								0x01, //Hardware_Version
								0x01, //Application0_Version
								0x00, //Application1_Version
								APPLICATION_NORMAL, //Application0_Status
								APPLICATION_ERROR,  //Application1_Status
								0x00
							};

void App_IAP_Bsp_Init(void)
{
	Hal_Led_Init();

	Hal_Key_Init();
}

void App_IAP_Task(void)
{
//	uint8_t i = 0;
	uint32_t bootAddrCur = 0;
	SystemParam_t sysParamCur;
	
	App_IAP_Bsp_Init();

	App_IAP_Show_BootInfo();
//	memset((void *)&sysParamCur, 0x55, sizeof(SystemParam_t));
//	Hal_Flash_Write_Len(SYSPARAM_ADDR, (uint8_t *)&sysParamDef, sizeof(SystemParam_t));
//	Hal_Flash_Write_Len(SYSPARAM_BACKUP_ADDR, (uint8_t *)&sysParamDef, sizeof(SystemParam_t));

	App_System_ParamReadCheck(&sysParamCur);

	bootAddrCur = App_System_Sel_BootAddr(&sysParamCur);

	Hal_Debug(PRINTF, "Boot App Start ...\r\n\r\n");
	Hal_Jump_2_App(bootAddrCur);
	
	while(1);
}

void App_IAP_Show_BootInfo(void)
{
	Hal_Debug(PRINTF, "Boot---------- Enter BootLoader ----------\r\n");
    Hal_Debug(PRINTF, "\r\n");
    Hal_Debug(PRINTF, "Boot======== flash pration table =========\r\n");
    Hal_Debug(PRINTF, "Boot| name     | offset     | size       |\r\n");
    Hal_Debug(PRINTF, "Boot--------------------------------------\r\n");
    Hal_Debug(PRINTF, "Boot| boot     | 0x%08X | 0x%08X |\r\n", BOOT_START_ADDR, BOOT_FLASH_SIZE);
    Hal_Debug(PRINTF, "Boot| app0     | 0x%08X | 0x%08X |\r\n", APP0_START_ADDR, APP0_FLASH_SIZE);
	Hal_Debug(PRINTF, "Boot| app1     | 0x%08X | 0x%08X |\r\n", APP1_START_ADDR, APP1_FLASH_SIZE);
    Hal_Debug(PRINTF, "Boot======================================\r\n");
    Hal_Debug(PRINTF, "\r\n");
}

void App_System_ParamReadCheck(SystemParam_t *pData)
{
//	uint32_t checkDataLen = 0;
	SystemParam_t sysParam, sysParamBackup;

	Hal_Flash_Read_Len((uint32_t)SYSPARAM_ADDR, (uint8_t *)&sysParam, sizeof(SystemParam_t));
	Hal_Flash_Read_Len((uint32_t)SYSPARAM_BACKUP_ADDR, (uint8_t *)&sysParamBackup, sizeof(SystemParam_t));

	if(sysParam.SystemParamCheckSum == App_SysParam_CheckSum((uint8_t *)&sysParam, sizeof(SystemParam_t))){
		/*
		 *  @ ϵͳ�������洢��������У����ȷ��
		 *  @ ��ʼУ��ϵͳ���������������ݣ�
		 *    1�����������������CRCУ����ȷ������жԱ�����һ���ԣ���һ������������д�뱸����
		 *    2�����������������CRCУ�������������ϵͳ������д�뱸������
		 * */
		Hal_Debug(PRINTF, "Boot System parameter main sector data checked OK\r\n");
		if(sysParamBackup.SystemParamCheckSum == App_SysParam_CheckSum((uint8_t *)&sysParamBackup, sizeof(SystemParam_t))){
			Hal_Debug(PRINTF, "Boot System parameter backup sector data checked OK\r\n");

			if(memcmp(&sysParam, &sysParamBackup, sizeof(SystemParam_t)) != 0){
				Hal_Debug(PRINTF, "Boot System parameter main sector and backup sector data are different, update backup sector data\r\n");
				memcpy(&sysParamBackup, &sysParam, sizeof(SystemParam_t));
				App_Update_SysParam2Flash(SYSPARAM_BACKUP_ADDR, (SystemParam_t *)&sysParamBackup);
			}

			else{
				Hal_Debug(PRINTF, "Boot System parameter main sector and backup sector data are the same\r\n");
			}
		}
		else{
			Hal_Debug(PRINTF, "Boot System parameter backup sector data checked Fail, update backup sector data\r\n");
			memcpy(&sysParamBackup, &sysParam, sizeof(SystemParam_t));
			App_Update_SysParam2Flash(SYSPARAM_BACKUP_ADDR, (SystemParam_t *)&sysParamBackup);
		}
	}
	else{
		/*
		 *  @ ϵͳ�������洢��������У�����
		 *  @ ��ʼУ��ϵͳ���������������ݣ�
		 *    1�����������������CRCУ����ȷ���򽫱���������д������
		 *    2�����������������CRCУ�������Ĭ��ϵͳ��������д����������ǿ������APP0��
		 * */
		Hal_Debug(PRINTF, "Boot System parameter main sector data checked Fail\r\n");

		if(sysParamBackup.SystemParamCheckSum == App_SysParam_CheckSum((uint8_t *)&sysParamBackup, sizeof(SystemParam_t))){
			Hal_Debug(PRINTF, "Boot System parameter backup sector data checked OK, update main sector data\r\n");

			memcpy(&sysParam, &sysParamBackup, sizeof(SystemParam_t));
			App_Update_SysParam2Flash(SYSPARAM_ADDR, (SystemParam_t *)&sysParam);
		}
		else{
			Hal_Debug(PRINTF, "Boot System parameter main sector and backup sector data checked Fail, Restore defaults\r\n");
			
			sysParamDef.SystemParamCheckSum = App_SysParam_CheckSum((uint8_t *)&sysParamDef, sizeof(SystemParam_t));
		
			memcpy(&sysParam, &sysParamDef, sizeof(SystemParam_t));
			memcpy(&sysParamBackup, &sysParamDef, sizeof(SystemParam_t));
			
			App_Update_SysParam2Flash(SYSPARAM_ADDR, (SystemParam_t *)&sysParam);
			App_Update_SysParam2Flash(SYSPARAM_BACKUP_ADDR, (SystemParam_t *)&sysParamBackup);
		}
	}

	memcpy(pData, &sysParam, sizeof(SystemParam_t));

	Hal_Debug(PRINTF, "Boot Hardware_Version = 0x%08lX\r\n", pData->Hardware_Version);
	Hal_Debug(PRINTF, "Boot Application0_Version = 0x%08lX\r\n", pData->Application0_Version);
	Hal_Debug(PRINTF, "Boot Application0_Status  = 0x%08lX\r\n", pData->Application0_Status);
	Hal_Debug(PRINTF, "Boot Application1_Version = 0x%08lX\r\n", pData->Application1_Version);
	Hal_Debug(PRINTF, "Boot Application1_Status  = 0x%08lX\r\n", pData->Application1_Status);
}

uint32_t App_System_Sel_BootAddr(SystemParam_t *pData)
{
	uint32_t rBootAddr = 0;

	if(pData->Application0_Version >= pData->Application1_Version){
		if(APPLICATION_NORMAL == pData->Application0_Status){
			/* �������� */
			Hal_Debug(PRINTF, "Boot APP0 APPLICATION_NORMAL. Run the APP0.\r\n");

			rBootAddr = (uint32_t)APP0_START_ADDR;
		}
		else if(APPLICATION_UPDATED == pData->Application0_Status){
			/*
			 * ��IAP���µĳ���̼�����APP��status�ĳ� �����־λ @APPLICATION_ERROR��
			 * 1����APP��ת������������޸ĳ�����������־ @APPLICATION_NORMAL��
			 * 2����APP��ת����ʧ�ܣ��ܷɣ�����ô���Ź��Ḵλ���������APP�Ǵ����־����������
			 * 3����հ汾��Ϣ��������������
			 * 4������ϵͳ������д��flash
			 * */
			Hal_Debug(PRINTF, "Boot APP0 APPLICATION_UPDATED. Clear APP0 system parameters, Run the APP0.\r\n");

			pData->Application0_Version = 0;
			pData->Application0_Status = APPLICATION_ERROR;
			App_Update_SysParam2Flash(SYSPARAM_ADDR, pData);
			App_Update_SysParam2Flash(SYSPARAM_BACKUP_ADDR, pData);
			rBootAddr = (uint32_t)APP0_START_ADDR;
		}
		else{
			/* ����APP0��Ϣ����ǿ������APP1 */
			printf("APP0 APPLICATION_ERROR. Forced the APP1 to run.\r\n");
			rBootAddr = (uint32_t)APP1_START_ADDR;
		}
	}
	else{
		if(APPLICATION_NORMAL == pData->Application1_Status){
			/* �������� */
			Hal_Debug(PRINTF, "Boot APP1 APPLICATION_NORMAL. Run the APP1.\r\n");

			rBootAddr = (uint32_t)APP1_START_ADDR;
		}
		else if(APPLICATION_UPDATED == pData->Application1_Status){
			/*
			 * ��IAP���µĳ���̼�����APP��status�ĳ� �����־λ @APPLICATION_ERROR��
			 * 1����APP��ת������������޸ĳ�����������־ @APPLICATION_NORMAL��
			 * 2����APP��ת����ʧ�ܣ��ܷɣ�����ô���Ź��Ḵλ���������APP�Ǵ����־����������
			 * 3����հ汾��Ϣ��������������
			 * 4������ϵͳ������д��flash
			 * */
			Hal_Debug(PRINTF, "Boot APP1 APPLICATION_UPDATED. Clear APP1 system parameters, Run the APP1.\r\n");

			pData->Application1_Version = 0;
			pData->Application1_Status = APPLICATION_ERROR;
			App_Update_SysParam2Flash(SYSPARAM_ADDR, pData);
			App_Update_SysParam2Flash(SYSPARAM_BACKUP_ADDR, pData);
			rBootAddr = (uint32_t)APP1_START_ADDR;
		}
		else{
			/* ����APP1��Ϣ����ǿ������APP0 */
			Hal_Debug(PRINTF, "Boot APP1 APPLICATION_ERROR. Forced the APP0 to run.\r\n");
			rBootAddr = (uint32_t)APP0_START_ADDR;
		}
	}

	return rBootAddr;
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


