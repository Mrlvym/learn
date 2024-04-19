#include "bsp_rtc.h"

void Bsp_Rtc_CLK_Config(void)
{
	RTC_InitTypeDef RTC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	PWR_BackupAccessCmd(ENABLE);

	RCC_LSEConfig(RCC_LSE_ON);

	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	RCC_RTCCLKCmd(ENABLE);	

	RTC_WaitForSynchro();

	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
	RTC_InitStruct.RTC_AsynchPrediv = ASYNCHPREDIV;
	RTC_InitStruct.RTC_SynchPrediv = SYNCHPREDIV;
	if(RTC_Init(&RTC_InitStruct) == ERROR)
	{
		printf("RTC CLK Init is error \r\n");
	}
	
}

void Bsp_Rtc_Set_DataTime(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;

	RTC_TimeStruct.RTC_H12 = RTC_H12_AMorPM; //选24小时格式，此项无需配置
	RTC_TimeStruct.RTC_Hours = HOURS;
	RTC_TimeStruct.RTC_Minutes = MINUTES;
	RTC_TimeStruct.RTC_Seconds = SECONDS;

	RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStruct);

	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);	

	RTC_DateStruct.RTC_Year = YEAR;
	RTC_DateStruct.RTC_Month = MONTH;
	RTC_DateStruct.RTC_WeekDay = WEEKDAY;
	RTC_DateStruct.RTC_Date = DATE;

	RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStruct);	

	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA); 
}

void Bsp_Rtc_Set_CurDataTime(RTC_DateTypeDef* RTC_DateStruct, RTC_TimeTypeDef* RTC_TimeStruct)
{
	RTC_SetTime(RTC_Format_BINorBCD, RTC_TimeStruct);
	RTC_SetDate(RTC_Format_BINorBCD, RTC_DateStruct);	
}

void Bsp_Rtc_Init_Alarm_Time(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RTC_AlarmTypeDef RTC_AlarmStruct;

	EXTI_InitStruct.EXTI_Line = EXTI_Line17;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);

	RTC_AlarmCmd(RTC_ALARM_X, DISABLE);
	
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_AMorPM;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = ALARM_HOURS;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = ALARM_MINUTES;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = ALARM_SECONDS;
	RTC_AlarmStruct.RTC_AlarmMask = ALARM_MASK;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = ALARM_DATE_WEEKDAY_SEL;
	RTC_AlarmStruct.RTC_AlarmDateWeekDay = ALARM_DATE_WEEKDAY;

	RTC_SetAlarm(RTC_Format_BINorBCD, RTC_ALARM_X, &RTC_AlarmStruct);

	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	RTC_AlarmCmd(RTC_ALARM_X, ENABLE);

	RTC_ClearFlag(RTC_FLAG_ALRAF);

	EXTI_ClearITPendingBit(EXTI_Line17);	
			
}

void Bsp_Rtc_Set_AlarmTime(uint32_t RTC_Alarm,  RTC_AlarmTypeDef* RTC_AlarmStruct)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	EXTI_InitStruct.EXTI_Line = EXTI_Line17;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);

	RTC_AlarmCmd(RTC_Alarm, DISABLE);
	
	RTC_SetAlarm(RTC_Format_BINorBCD, RTC_Alarm, RTC_AlarmStruct);

	if(RTC_Alarm == RTC_Alarm_A)
	{	
		RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	}
	else if(RTC_Alarm == RTC_Alarm_B)
	{
		RTC_ITConfig(RTC_IT_ALRB, ENABLE);		
	}

	RTC_AlarmCmd(RTC_Alarm, ENABLE);

	if(RTC_Alarm == RTC_Alarm_A)
	{	
		RTC_ClearFlag(RTC_FLAG_ALRAF);
	}
	else if(RTC_Alarm == RTC_Alarm_B)
	{
		RTC_ClearFlag(RTC_FLAG_ALRBF);		
	}

	EXTI_ClearITPendingBit(EXTI_Line17);	
}

void Bsp_Rtc_DataTime_Init(void)
{
	Bsp_Rtc_CLK_Config();

	if(RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		Bsp_Rtc_Set_DataTime();
		Bsp_Rtc_Init_Alarm_Time();
	}
	else
	{
		if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			printf("\r\n 发生电源复位....\r\n");
		}

		if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n 发生外部复位....\r\n");			
		}

		printf("\r\n 不需要重新配置 RTC....\r\n");

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		
		PWR_BackupAccessCmd(ENABLE);

		RTC_WaitForSynchro();
		
	}

	printf("\r\n RTC 配置完成....\r\n");
		
}

