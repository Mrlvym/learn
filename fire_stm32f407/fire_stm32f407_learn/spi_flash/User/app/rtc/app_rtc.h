#ifndef __APP_RTC_H__
#define __APP_RTC_H__
#include "stdint.h"

typedef enum
{	
	APP_SET_DataTimeAlarm_FLAG_RESET = 0,
	APP_SET_DataTimeAlarm_FLAG_SET = 1	
}App_DataTimeAlarm_Set_e;
	

uint8_t App_Get_DataTimeAlarm_Set_Flag(void);
void App_Set_DataTimeAlarm_Set_Flag(uint8_t flag);


void App_DataTime_Alarm_Init(void);

void App_Set_DataTimeAlarm(void);

void App_Rtc_DateTimeAlarm_Test_Task(void);


#endif 

