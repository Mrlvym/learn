#include "ff.h"
#include "app_fatfs.h"
#include "hal_debug_usart.h"



FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32 F407������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ� 123 \r\n";  


void App_Fatfs_Init(void)
{
	res_sd = f_mount(&fs, FATFS_SEL_FS, 1);
//	res_sd = f_mount(&fs, "4:", 1);

	if(res_sd == FR_NO_FILESYSTEM)
//	if(1)
	{
		Hal_Debug(PRINTF, "��SPI_FLASH / SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
		/* ��ʽ�� */
		res_sd=f_mkfs(FATFS_SEL_FS, 0, 0);							

		if(res_sd == FR_OK)
		{
			Hal_Debug(PRINTF, "��SPI_FLASH / SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
			/* ��ʽ������ȡ������ */
			res_sd = f_mount(NULL, FATFS_SEL_FS, 1);			
			/* ���¹���	*/			
			res_sd = f_mount(&fs, FATFS_SEL_FS, 1);
		}
		else
		{
			Hal_Debug(PRINTF, "������ʽ��ʧ�ܡ�����\r\n");
			while(1);
		}
	}
	else if(res_sd!=FR_OK)
	{
		Hal_Debug(PRINTF, "����SPI_FLASH / SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
		Hal_Debug(PRINTF, "��������ԭ��SPI_FLASH / SD����ʼ�����ɹ���\r\n");
		while(1);
	}
	else
	{
		Hal_Debug(PRINTF, "���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
	}
}

void App_Fatfs_RadeWrite_Test(void)
{
	/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
	/* ���ļ�������ļ��������򴴽��� */
	Hal_Debug(PRINTF, "\r\n****** ���������ļ�д�����... ******\r\n");	
	res_flash = f_open(&fnew, FATFS_TEST_F, FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		Hal_Debug(PRINTF, "����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
    /* ��ָ���洢������д�뵽�ļ��� */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
	    if(res_flash==FR_OK)
	    {
	      Hal_Debug(PRINTF, "���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
	      Hal_Debug(PRINTF, "�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
	    }
	    else
	    {
	      Hal_Debug(PRINTF, "�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
	    }    
			/* ���ٶ�д���ر��ļ� */
	    f_close(&fnew);
	}
	else
	{	
		Hal_Debug(PRINTF, "������/�����ļ�ʧ�ܡ�\r\n");
	}
	
/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
	Hal_Debug(PRINTF, "****** ���������ļ���ȡ����... ******\r\n");
	res_flash = f_open(&fnew, FATFS_TEST_F, FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		Hal_Debug(PRINTF, "�����ļ��ɹ���\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
	    if(res_flash==FR_OK)
	    {
	      Hal_Debug(PRINTF, "���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
	      Hal_Debug(PRINTF, "����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
	    }
	    else
	    {
	      Hal_Debug(PRINTF, "�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
	    }		
	}
	else
	{
		Hal_Debug(PRINTF, "�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,FATFS_SEL_FS,1);
}





void App_Fatfs_Test_Task(void)
{
	App_Fatfs_Init();

	App_Fatfs_RadeWrite_Test();
}

