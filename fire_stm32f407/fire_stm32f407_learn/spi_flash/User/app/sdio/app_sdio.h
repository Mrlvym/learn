#ifndef __APP_SDIO_H__
#define __APP_SDIO_H__
#include "stm32f4xx.h"
#include "hal_sdio.h"


typedef enum	{APP_FAILED = 0, APP_PASS = !APP_FAILED} app_test_status_e;

#define APP_SD_TEST_ADDR		0x00

#define APP_BLOCK_SIZE			512
#define APP_NUMBER_OF_BLOCKS	10
#define APP_MUL_BUF_SIZE		(APP_BLOCK_SIZE * APP_NUMBER_OF_BLOCKS)

void App_SD_Test_Task(void);
void App_SD_Erase_Test(void);
void App_SD_SingleBlock_Test(void);
void App_SD_MulBlock_Test(void);

void Fill_Buffer(uint8_t *buf, uint32_t buf_len);
app_test_status_e Compare_Buffer(uint8_t *buf1, uint8_t *buf2, uint32_t buf_len);
app_test_status_e eCompare_Buffer(uint8_t *buf, uint32_t buf_len);

#endif 


