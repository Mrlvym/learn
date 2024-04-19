/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */
#include "bsp_spi_flash.h"
#include "hal_spi_flash.h"
#include "hal_delay.h"
#include "hal_sdio.h"
#include "string.h"


/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */
#define SPI_FLASH	3
#define SDIO_SD		4

extern  SD_CardInfo SDCardInfo;

/*-----------------------------------------------------------------------*/
/* ��ȡ�豸״̬                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* ������ */
)
{
	DSTATUS status = STA_NOINIT;
	
	switch (pdrv) {
		case SDIO_SD:	/* SD CARD */
			status &= ~STA_NOINIT;
			break;
    
		case SPI_FLASH:        /* SPI Flash */   
			if(sFLASH_ID == Hal_Spi_Flash_Read_JedecDeviceID())
			{
				status &= ~STA_NOINIT;			
			}
			else
			{
				status = STA_NOINIT;									
			}
			break;

		default:
			status = STA_NOINIT;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* �豸��ʼ��                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* ������ */
)
{
	DSTATUS status = STA_NOINIT;	
	switch (pdrv) {
		case SDIO_SD:	         /* SD CARD */
			if(Hal_SD_Init()==SD_OK)
			{
				status &= ~STA_NOINIT;
			}
			else 
			{
				status = STA_NOINIT;
			}
		
			break;
    
		case SPI_FLASH:    /* SPI Flash */ 
			Hal_Spi_Flash_Init();
	
			Hal_Delay_ms(10);

			status = disk_status(SPI_FLASH);
			
			break;
      
		default:
			status = STA_NOINIT;
	}
	return status;
}


/*-----------------------------------------------------------------------*/
/* ����������ȡ�������ݵ�ָ���洢��                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* �豸������(0..) */
	BYTE *buff,		/* ���ݻ����� */
	DWORD sector,	/* �����׵�ַ */
	UINT count		/* ��������(1..128) */
)
{
	DRESULT status = RES_PARERR;
	SD_Error SD_state = SD_OK;
	
	switch (pdrv) {
		case SDIO_SD:	/* SD CARD */						
		  if((DWORD)buff&3)
			{
				DRESULT res = RES_OK;
				DWORD scratch[SDIO_SD_BLOCK_SIZE / 4];

				while (count--) 
				{
					res = disk_read(SDIO_SD,(void *)scratch, sector++, 1);

					if (res != RES_OK) 
					{
						break;
					}
					memcpy(buff, scratch, SDIO_SD_BLOCK_SIZE);
					buff += SDIO_SD_BLOCK_SIZE;
		    }
		    return res;
			}
			
			SD_state=SD_ReadMultiBlocks(buff,sector*SDIO_SD_BLOCK_SIZE,SDIO_SD_BLOCK_SIZE,count);
		  if(SD_state==SD_OK)
			{
				/* Check if the Transfer is finished */
				SD_state=SD_WaitReadOperation();
				while(SD_GetStatus() != SD_TRANSFER_OK);
			}
			if(SD_state!=SD_OK)
				status = RES_PARERR;
		  else
			  status = RES_OK;	
			break;   
			
		case SPI_FLASH:

			sector += 1536;//ֻ����10MB����fatfs��ǰ6MB��1536 * SPI_FLASH_SECTOR_SIZE����������ʵ�飬spi_flash��16MB

			Hal_Spi_Flash_BufferRead(buff, sector * SPI_FLASH_SECTOR_SIZE, count * SPI_FLASH_SECTOR_SIZE);

			status = RES_OK;
			
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* д������������д��ָ�������ռ���                                      */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			  /* �豸������(0..) */
	const BYTE *buff,	/* ��д�����ݵĻ����� */
	DWORD sector,		  /* �����׵�ַ */
	UINT count			  /* ��������(1..128) */
)
{
	DRESULT status = RES_PARERR;
	SD_Error SD_state = SD_OK;
	uint32_t write_address;
	
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}

	switch (pdrv) {
		case SDIO_SD:	/* SD CARD */  
			if((DWORD)buff&3)
			{
				DRESULT res = RES_OK;
				DWORD scratch[SDIO_SD_BLOCK_SIZE / 4];

				while (count--) 
				{
					memcpy( scratch,buff,SDIO_SD_BLOCK_SIZE);
					res = disk_write(SDIO_SD,(void *)scratch, sector++, 1);
					if (res != RES_OK) 
					{
						break;
					}					
					buff += SDIO_SD_BLOCK_SIZE;
		    }
		    return res;
			}		
		
			SD_state=SD_WriteMultiBlocks((uint8_t *)buff,sector*SDIO_SD_BLOCK_SIZE,SDIO_SD_BLOCK_SIZE,count);
			if(SD_state==SD_OK)
			{
				/* Check if the Transfer is finished */
				SD_state=SD_WaitWriteOperation();

				/* Wait until end of DMA transfer */
				while(SD_GetStatus() != SD_TRANSFER_OK);			
			}
			if(SD_state!=SD_OK)
				status = RES_PARERR;
		  else
			  status = RES_OK;	
		break;

		case SPI_FLASH:
			sector += 1536;//ֻ����10MB����fatfs��ǰ6MB��1536 * SPI_FLASH_SECTOR_SIZE����������ʵ�飬spi_flash��16MB
		
			write_address = sector * SPI_FLASH_SECTOR_SIZE;

			Hal_Spi_Flash_SectorErase(write_address);

			Hal_Spi_Flash_BufferWrite((uint8_t *)buff, write_address, count * SPI_FLASH_SECTOR_SIZE);

			status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif


/*-----------------------------------------------------------------------*/
/* ��������                                                              */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* ������ */
	BYTE cmd,		  /* ����ָ�� */
	void *buff		/* д����߶�ȡ���ݵ�ַָ�� */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case SDIO_SD:	/* SD CARD */
			switch (cmd) 
			{
				// Get R/W sector size (WORD) 
				case GET_SECTOR_SIZE :    
					*(WORD * )buff = SDIO_SD_BLOCK_SIZE;
				break;
				// Get erase block size in unit of sector (DWORD)
				case GET_BLOCK_SIZE :      
					*(DWORD * )buff = 1;//SDCardInfo.CardBlockSize;
				break;

				case GET_SECTOR_COUNT:
					*(DWORD * )buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
					break;
				case CTRL_SYNC :
				break;
			}
			status = RES_OK;
			break;
    
		case SPI_FLASH:		

			switch(cmd)
			{
				/* ��������    */
				case GET_SECTOR_COUNT:
					*(WORD *)buff = (WORD)((10*1024*1024) / SPI_FLASH_SECTOR_SIZE); //10MB*1024*1024 / 4096
					break;
				/* ������С */
				case GET_SECTOR_SIZE:
					*(WORD *)buff = (WORD)SPI_FLASH_SECTOR_SIZE;
					break;
				/* ͬʱ������������ */
				case GET_BLOCK_SIZE:
					*(WORD *)buff = (WORD)1;
					break;
			}
			status = RES_OK;
			break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif


__weak DWORD get_fattime(void) {
	/* ���ص�ǰʱ��� */
	return	  ((DWORD)(2023 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}
