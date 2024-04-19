#include "ff.h"
#include "app_fatfs.h"
#include "hal_debug_usart.h"



FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"欢迎使用野火STM32 F407开发板 今天是个好日子，新建文件系统测试文件 123 \r\n";  


void App_Fatfs_Init(void)
{
	res_sd = f_mount(&fs, FATFS_SEL_FS, 1);
//	res_sd = f_mount(&fs, "4:", 1);

	if(res_sd == FR_NO_FILESYSTEM)
//	if(1)
	{
		Hal_Debug(PRINTF, "》SPI_FLASH / SD卡还没有文件系统，即将进行格式化...\r\n");
		/* 格式化 */
		res_sd=f_mkfs(FATFS_SEL_FS, 0, 0);							

		if(res_sd == FR_OK)
		{
			Hal_Debug(PRINTF, "》SPI_FLASH / SD卡已成功格式化文件系统。\r\n");
			/* 格式化后，先取消挂载 */
			res_sd = f_mount(NULL, FATFS_SEL_FS, 1);			
			/* 重新挂载	*/			
			res_sd = f_mount(&fs, FATFS_SEL_FS, 1);
		}
		else
		{
			Hal_Debug(PRINTF, "《《格式化失败。》》\r\n");
			while(1);
		}
	}
	else if(res_sd!=FR_OK)
	{
		Hal_Debug(PRINTF, "！！SPI_FLASH / SD卡挂载文件系统失败。(%d)\r\n",res_sd);
		Hal_Debug(PRINTF, "！！可能原因：SPI_FLASH / SD卡初始化不成功。\r\n");
		while(1);
	}
	else
	{
		Hal_Debug(PRINTF, "》文件系统挂载成功，可以进行读写测试\r\n");
	}
}

void App_Fatfs_RadeWrite_Test(void)
{
	/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
	Hal_Debug(PRINTF, "\r\n****** 即将进行文件写入测试... ******\r\n");	
	res_flash = f_open(&fnew, FATFS_TEST_F, FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		Hal_Debug(PRINTF, "》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
    /* 将指定存储区内容写入到文件内 */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
	    if(res_flash==FR_OK)
	    {
	      Hal_Debug(PRINTF, "》文件写入成功，写入字节数据：%d\n",fnum);
	      Hal_Debug(PRINTF, "》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
	    }
	    else
	    {
	      Hal_Debug(PRINTF, "！！文件写入失败：(%d)\n",res_flash);
	    }    
			/* 不再读写，关闭文件 */
	    f_close(&fnew);
	}
	else
	{	
		Hal_Debug(PRINTF, "！！打开/创建文件失败。\r\n");
	}
	
/*------------------- 文件系统测试：读测试 ------------------------------------*/
	Hal_Debug(PRINTF, "****** 即将进行文件读取测试... ******\r\n");
	res_flash = f_open(&fnew, FATFS_TEST_F, FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		Hal_Debug(PRINTF, "》打开文件成功。\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
	    if(res_flash==FR_OK)
	    {
	      Hal_Debug(PRINTF, "》文件读取成功,读到字节数据：%d\r\n",fnum);
	      Hal_Debug(PRINTF, "》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
	    }
	    else
	    {
	      Hal_Debug(PRINTF, "！！文件读取失败：(%d)\n",res_flash);
	    }		
	}
	else
	{
		Hal_Debug(PRINTF, "！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,FATFS_SEL_FS,1);
}





void App_Fatfs_Test_Task(void)
{
	App_Fatfs_Init();

	App_Fatfs_RadeWrite_Test();
}

