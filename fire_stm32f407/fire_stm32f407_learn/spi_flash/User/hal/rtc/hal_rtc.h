#ifndef __HAL_RTC_H__
#define __HAL_RTC_H__
#include "stdint.h"
#include "stm32f4xx.h"
#include "bsp_rtc.h"

#define HAL_RTC_Alarm_A 	RTC_Alarm_A
#define HAL_RTC_Alarm_B 	RTC_Alarm_B

#define HAL_RTC_AlarmDateWeekDaySel_WeekDay 	RTC_AlarmDateWeekDaySel_WeekDay
#define HAL_RTC_AlarmDateWeekDaySel_Date		RTC_AlarmDateWeekDaySel_Date

void Hal_Rtc_DataTime_Init(void);
void Hal_Rtc_Set_CurDataTime(RTC_DateTypeDef* RTC_DateStruct, RTC_TimeTypeDef* RTC_TimeStruct);
void Hal_Rtc_Set_AlarmTime(uint32_t RTC_Alarm,  RTC_AlarmTypeDef* RTC_AlarmStruct);
void Hal_Rtc_GetDate(uint32_t RTC_Format, RTC_DateTypeDef* RTC_DateStruct);
void Hal_Rtc_GetTime(uint32_t RTC_Format, RTC_TimeTypeDef* RTC_TimeStruct);
float Hal_RTC_GetSubSecond(void);
void Hal_RTC_GetAlarm(uint32_t RTC_Format, uint32_t RTC_Alarm, RTC_AlarmTypeDef* RTC_AlarmStruct);



#endif

