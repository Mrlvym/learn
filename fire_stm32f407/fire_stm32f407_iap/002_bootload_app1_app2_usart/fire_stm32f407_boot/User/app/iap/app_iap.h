#ifndef __APP_IAP_H__
#define __APP_IAP_H__
#include "stdint.h"

#define APP_BUF_SIZE		(40 * 1024)

#define COUNTDOWN_SECS		5

#define BOOT_START_ADDR		0x08000000
#define BOOT_FLASH_SIZE		0x4000	//16k

#define SYSPARAM_ADDR		0x08004000
#define SYSPARAM_SIZE		0x4000	//16k

#define SYSPARAM_BACKUP_ADDR	0x08008000
#define SYSPARAM_BACKUP_SIZE	0x4000	//16k

#define APP0_START_ADDR		0x0800C000
#define APP0_FLASH_SIZE		0x8000//32k

#define APP1_START_ADDR		0x08014000
#define APP1_FLASH_SIZE		0x8000//32k


typedef enum {
	APPLICATION_NORMAL = 0,		// APP能正常稳定运行
	APPLICATION_UPDATED,		// APP刚更新完成，等待测试启动
	APPLICATION_ERROR,			// APP错误，不能正常工作
}Application_Status_t;
 
typedef	__packed struct{
	uint32_t Device_id;					// 设备号
	uint32_t Hardware_Version;			// 硬件版本信息
	uint32_t Application0_Version;		// APP0软件版本
	uint32_t Application1_Version;		// APP1软件版本
	uint32_t Application0_Status;		// APP0的状态. @Application_Status_t
	uint32_t Application1_Status;		// APP1的状态. @Application_Status_t
//	uint8_t  Server_Address[64];		// 服务器的地址
//	uint32_t Server_Port;				// 服务器的端口号
//	uint8_t  Server_Key[4];				// 服务器的登录密钥，随时更新
	uint8_t SystemParamCheckSum;
}SystemParam_t;




void App_IAP_Bsp_Init(void);

void App_IAP_Task(void);

void App_IAP_Show_BootInfo(void);

void App_System_ParamReadCheck(SystemParam_t *pData);

uint32_t App_System_Sel_BootAddr(SystemParam_t *pData);

void App_Update_SysParam2Flash(uint32_t addr, SystemParam_t *pData);

uint8_t App_SysParam_CheckSum(uint8_t *buf, uint32_t len);

#endif

