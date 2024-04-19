#ifndef __APP_FATFS_H__
#define __APP_FATFS_H__


//#define FATFS_SPI_FLASH		"3:"
#define FATFS_SDIO_SD		"4:"

#ifdef FATFS_SPI_FLASH
#define FATFS_SEL_FS	FATFS_SPI_FLASH
#define FATFS_TEST_F	"3:FatFs��д�����ļ�.txt"
#endif

#ifdef FATFS_SDIO_SD
#define FATFS_SEL_FS	FATFS_SDIO_SD
#define FATFS_TEST_F	"4:FatFs��д�����ļ�.txt"
#endif


void App_Fatfs_Init(void);

void App_Fatfs_RadeWrite_Test(void);

void App_Fatfs_Test_Task(void);


#endif 



