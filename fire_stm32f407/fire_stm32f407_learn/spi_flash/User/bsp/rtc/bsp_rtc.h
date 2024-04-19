#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__
#include "stdint.h"
#include "stm32f4xx.h"
#include "stdio.h"

// 异步分频因子
#define ASYNCHPREDIV 0X7F
// 同步分频因子
#define SYNCHPREDIV 0XFF

// 时间宏定义
#define RTC_H12_AMorPM RTC_H12_AM
#define HOURS 		1 	// 0~23
#define MINUTES 	1 	// 0~59
#define SECONDS 	1 	// 0~59

// 日期宏定义
#define WEEKDAY 1 	// 1~7
#define DATE 	1 	// 1~31
#define MONTH 	1 	// 1~12
#define YEAR 	1 	// 0~99

//闹钟相关宏定义
#define ALARM_HOURS 	1 	// 0~23
#define ALARM_MINUTES 	1 	// 0~59
#define ALARM_SECONDS 	3 	// 0~59

#define ALARM_MASK 				RTC_AlarmMask_None
#define ALARM_DATE_WEEKDAY_SEL 	RTC_AlarmDateWeekDaySel_WeekDay
#define ALARM_DATE_WEEKDAY 		RTC_Weekday_Monday
#define RTC_ALARM_X				RTC_Alarm_A


// 时间格式宏定义
#define RTC_Format_BINorBCD RTC_Format_BIN

// 备份域寄存器宏定义
#define RTC_BKP_DRX RTC_BKP_DR0

// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA 0X32F2

void Bsp_Rtc_CLK_Config(void);
void Bsp_Rtc_Set_DataTime(void);
void Bsp_Rtc_Set_CurDataTime(RTC_DateTypeDef* RTC_DateStruct, RTC_TimeTypeDef* RTC_TimeStruct);
void Bsp_Rtc_Init_Alarm_Time(void);
void Bsp_Rtc_Set_AlarmTime(uint32_t RTC_Alarm,  RTC_AlarmTypeDef* RTC_AlarmStruct);
void Bsp_Rtc_DataTime_Init(void);


#endif 

