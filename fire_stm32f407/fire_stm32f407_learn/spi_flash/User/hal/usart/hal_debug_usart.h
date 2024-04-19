#ifndef __HAL_DEBUG_USART_H__
#define __HAL_DEBUG_USART_H__
#include "bsp_debug_usart.h"

#define true	1
#define false	0

#define NULL	0

#define hal_assert_param	assert_param 

#define countof(a)      (sizeof(a) / sizeof(*(a)))

#define HAL_RESET	BSP_RESET
#define HAL_SET		BSP_SET

#define PRINTF			0xff

#define DEBUG_LEV		2 

#define INFO_LEVEL		2 
#define DEBUG_LEVEL		1
#define ERROR_LEVEL		0

#define Hal_Debug(level, format, ...) \
	do { \
		if(level <= DEBUG_LEV) { \
			switch(level) { \
				case ERROR_LEVEL: \
					printf("Err: <%s, %d>: " format, __func__, __LINE__, ##__VA_ARGS__); \
					break; \
				case DEBUG_LEVEL: \
					printf("Debug: <%s, %d>: " format, __func__, __LINE__, ##__VA_ARGS__); \
					break; \
				case INFO_LEVEL: \
					printf("Info: <%s, %d>: " format, __func__, __LINE__, ##__VA_ARGS__); \
                    break; \
			} \
		} \
		else if(level == PRINTF){\
			printf(format, ##__VA_ARGS__);\
		}\
	} while(0) \



void Hal_Debug_Usart_Config(void);

#endif


