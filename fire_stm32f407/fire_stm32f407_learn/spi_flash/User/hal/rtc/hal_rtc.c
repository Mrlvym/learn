#include "hal_rtc.h"
#include "hal_debug_usart.h"


void Hal_Rtc_DataTime_Init(void)
{
	Bsp_Rtc_DataTime_Init();	
}

void Hal_Rtc_Set_CurDataTime(RTC_DateTypeDef* RTC_DateStruct, RTC_TimeTypeDef* RTC_TimeStruct)
{
	Bsp_Rtc_Set_CurDataTime(RTC_DateStruct, RTC_TimeStruct);	
}

void Hal_Rtc_Set_AlarmTime(uint32_t RTC_Alarm,  RTC_AlarmTypeDef* RTC_AlarmStruct)
{
	Bsp_Rtc_Set_AlarmTime(RTC_Alarm, RTC_AlarmStruct);	
}


void Hal_Rtc_GetDate(uint32_t RTC_Format, RTC_DateTypeDef* RTC_DateStruct)
{
	RTC_GetDate(RTC_Format, RTC_DateStruct);
}

void Hal_Rtc_GetTime(uint32_t RTC_Format, RTC_TimeTypeDef* RTC_TimeStruct)
{
	RTC_GetTime(RTC_Format, RTC_TimeStruct);
}

float Hal_RTC_GetSubSecond(void)
{
	uint32_t tmp_sub_sec = 0;
	float tmp_sub_sec_f = 0.0f;
	
	tmp_sub_sec = RTC_GetSubSecond();

	tmp_sub_sec_f = (float)((float)(SYNCHPREDIV - tmp_sub_sec) / (float)SYNCHPREDIV);

	return tmp_sub_sec_f;
}

void Hal_RTC_GetAlarm(uint32_t RTC_Format, uint32_t RTC_Alarm, RTC_AlarmTypeDef* RTC_AlarmStruct)
{
	RTC_GetAlarm(RTC_Format, RTC_Alarm, RTC_AlarmStruct);
}



