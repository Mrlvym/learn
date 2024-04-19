#include "math.h"
#include "app_rtc.h"
#include "hal_rtc.h"
#include "hal_debug_usart.h"
#include "hal_led.h"
#include "hal_key.h"

static RTC_DateTypeDef App_RTC_Date;
static RTC_TimeTypeDef App_RTC_Time;
static RTC_AlarmTypeDef App_RTC_Alarm_A;
static RTC_AlarmTypeDef App_RTC_Alarm_B;
static uint32_t App_RTC_Alarm_X;

static uint8_t App_DataTimeAlarm_Set_Flag;

char weekday_str[] = "WeekDay";
char data_str[] = "Data";

uint8_t App_Get_DataTimeAlarm_Set_Flag(void)
{
	return App_DataTimeAlarm_Set_Flag;
}

void App_Set_DataTimeAlarm_Set_Flag(uint8_t flag)
{
	App_DataTimeAlarm_Set_Flag = flag;
}

void App_DataTime_Alarm_Init(void)
{
	App_RTC_Date.RTC_Year = YEAR;
	App_RTC_Date.RTC_Month = MONTH;
	App_RTC_Date.RTC_Date = DATE;
	App_RTC_Date.RTC_WeekDay = WEEKDAY;

	App_RTC_Time.RTC_H12 = RTC_H12_AMorPM;
	App_RTC_Time.RTC_Hours = HOURS;
	App_RTC_Time.RTC_Minutes = MINUTES;
	App_RTC_Time.RTC_Seconds = SECONDS;

	App_RTC_Alarm_A.RTC_AlarmTime.RTC_H12 = RTC_H12_AMorPM;
	App_RTC_Alarm_A.RTC_AlarmTime.RTC_Hours = ALARM_HOURS;
	App_RTC_Alarm_A.RTC_AlarmTime.RTC_Minutes = ALARM_MINUTES;
	App_RTC_Alarm_A.RTC_AlarmTime.RTC_Seconds = ALARM_SECONDS;
	App_RTC_Alarm_A.RTC_AlarmMask = ALARM_MASK;
	App_RTC_Alarm_A.RTC_AlarmDateWeekDaySel = ALARM_DATE_WEEKDAY_SEL;
	App_RTC_Alarm_A.RTC_AlarmDateWeekDay = ALARM_DATE_WEEKDAY;

	App_RTC_Alarm_B.RTC_AlarmTime.RTC_H12 = RTC_H12_AMorPM;
	App_RTC_Alarm_B.RTC_AlarmTime.RTC_Hours = ALARM_HOURS;
	App_RTC_Alarm_B.RTC_AlarmTime.RTC_Minutes = ALARM_MINUTES;
	App_RTC_Alarm_B.RTC_AlarmTime.RTC_Seconds = ALARM_SECONDS;
	App_RTC_Alarm_B.RTC_AlarmMask = ALARM_MASK;
	App_RTC_Alarm_B.RTC_AlarmDateWeekDaySel = ALARM_DATE_WEEKDAY_SEL;
	App_RTC_Alarm_B.RTC_AlarmDateWeekDay = ALARM_DATE_WEEKDAY;

	App_RTC_Alarm_X = RTC_ALARM_X;

	App_DataTimeAlarm_Set_Flag = APP_SET_DataTimeAlarm_FLAG_RESET;
}

uint8_t App_Get_Val_From_Key(void)
{
	uint8_t ch;
	uint8_t get_str[2];
	uint8_t i = 0, j = 0;
	uint8_t val = 0;

	while(1)
	{
		ch = getchar();
		putchar(ch);
		if(ch != 0x0D || i == 2)
		{
			get_str[i++] = ch;
		}
		else
		{
			putchar('\r');
			putchar('\n');
			break;
		}
	}

	for(j = 0; j < i; j++)
	{
		val = val + ((get_str[j] - 0x30) * pow(10, (i-1-j)));
	}

	return val;
}

void App_Set_DataTime(void)
{
	uint8_t get_char = ' ';
	uint8_t temp_val = 0;

	Hal_Debug(INFO_LEVEL, "Change Data or Time (Y / N):");
	scanf("%c", &get_char);
	Hal_Debug(PRINTF, "%c\r\n", get_char);

	if(get_char == 'Y' || get_char == 'y')
	{
		Hal_Rtc_GetDate(RTC_Format_BINorBCD, &App_RTC_Date);
		Hal_Debug(INFO_LEVEL, "Current Data is : Y %d - M %d - D %d : W %d \r\n",\
								App_RTC_Date.RTC_Year, \
								App_RTC_Date.RTC_Month, \
								App_RTC_Date.RTC_Date, \
								App_RTC_Date.RTC_WeekDay);
		
		Hal_Debug(INFO_LEVEL, "Change Data (Y / N):");
		scanf("%c", &get_char);
		Hal_Debug(PRINTF, "%c\r\n", get_char);
		if(get_char == 'Y' || get_char == 'y')
		{
			Hal_Debug(INFO_LEVEL, "Change Data - Year (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Year : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Date.RTC_Year = temp_val;
//				Hal_Debug(INFO_LEVEL, "Change Data - year to %d \r\n", temp_val);
			}
			
			Hal_Debug(INFO_LEVEL, "Change Data - Month (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Month : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Date.RTC_Month = temp_val;
//				Hal_Debug(INFO_LEVEL, "Change Data - month to %d \r\n", temp_val);				
			}
			
			Hal_Debug(INFO_LEVEL, "Change Data - Data (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{		
				Hal_Debug(INFO_LEVEL, "Please Enter Data : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Date.RTC_Date = temp_val;
//				Hal_Debug(INFO_LEVEL, "Change Data - data to %d \r\n", temp_val);				
			}
			
			Hal_Debug(INFO_LEVEL, "Change Data - WeekDay (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{			
				Hal_Debug(INFO_LEVEL, "Please Enter WeedDay : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Date.RTC_WeekDay = temp_val;
//				Hal_Debug(INFO_LEVEL, "Change Data - weekday to %d \r\n", temp_val);				
			}

			Hal_Rtc_Set_CurDataTime(&App_RTC_Date, &App_RTC_Time);
		}

		Hal_Rtc_GetTime(RTC_Format_BINorBCD, &App_RTC_Time);
		Hal_Debug(INFO_LEVEL, "Current Time is : H %d : m %d : s %d \r\n",\
								App_RTC_Time.RTC_Hours, \
								App_RTC_Time.RTC_Minutes, \
								App_RTC_Time.RTC_Seconds); 			

		Hal_Debug(INFO_LEVEL, "Change Time (Y / N):");
		scanf("%c", &get_char);
		Hal_Debug(PRINTF, "%c\r\n", get_char);
		if(get_char == 'Y' || get_char == 'y')
		{
			Hal_Debug(INFO_LEVEL, "Change Time - Hours (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Hours : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Time.RTC_Hours = temp_val;
//				Hal_Debug(INFO_LEVEL, "Change Time - hours to %d \r\n", temp_val);								
			}

			Hal_Debug(INFO_LEVEL, "Change Time - Minutes (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Minutes : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Time.RTC_Minutes = temp_val;
//				Hal_Debug(INFO_LEVEL, "Change Time - minutes to %d \r\n", temp_val);								
			}
			
			Hal_Debug(INFO_LEVEL, "Change Time - Seconds (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Seconds : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Time.RTC_Seconds = temp_val;
//				Hal_Debug(INFO_LEVEL, "Change Time - seconds to %d \r\n", temp_val);								
			}

			Hal_Rtc_Set_CurDataTime(&App_RTC_Date, &App_RTC_Time);
		}
								
	}
}

void App_Set_Alarm(void)
{
	uint8_t get_char = ' ';
	uint8_t temp_val = 0;
	uint8_t *P_AlarmDateWeekDaySel;

	Hal_Debug(INFO_LEVEL, "Set Alarm Time (Y / N):");
	scanf("%c", &get_char);
	Hal_Debug(PRINTF, "%c\r\n", get_char);
	if(get_char == 'Y' || get_char == 'y')
	{
		Hal_Debug(INFO_LEVEL, "Set Alarm A or B (A / B):");
		scanf("%c", &get_char);
		Hal_Debug(PRINTF, "%c\r\n", get_char);
		if(get_char == 'A' || get_char == 'a')
		{
			App_RTC_Alarm_X = HAL_RTC_Alarm_A;
			Hal_RTC_GetAlarm(RTC_Format_BINorBCD, App_RTC_Alarm_X, &App_RTC_Alarm_A);

			if(App_RTC_Alarm_A.RTC_AlarmDateWeekDaySel == HAL_RTC_AlarmDateWeekDaySel_WeekDay)
			{
				P_AlarmDateWeekDaySel = (uint8_t *)weekday_str;
			}
			else if(App_RTC_Alarm_A.RTC_AlarmDateWeekDaySel == HAL_RTC_AlarmDateWeekDaySel_Date)
			{
				P_AlarmDateWeekDaySel = (uint8_t *)data_str;			
			}
			
			Hal_Debug(INFO_LEVEL, "Current RTC_Alarm_A Time : H : %d m : %d s : %d - %s : %d \r\n",\
									App_RTC_Alarm_A.RTC_AlarmTime.RTC_Hours,\
									App_RTC_Alarm_A.RTC_AlarmTime.RTC_Minutes,\
									App_RTC_Alarm_A.RTC_AlarmTime.RTC_Seconds,\
									P_AlarmDateWeekDaySel,\
									App_RTC_Alarm_A.RTC_AlarmDateWeekDay);

			Hal_Debug(INFO_LEVEL, "Select Alarm A WeekDay or Data (W / D):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'W' || get_char == 'w')
			{
				App_RTC_Alarm_A.RTC_AlarmDateWeekDaySel = HAL_RTC_AlarmDateWeekDaySel_WeekDay;
			}
			else if(get_char == 'D' || get_char == 'd')
			{
				App_RTC_Alarm_A.RTC_AlarmDateWeekDaySel = HAL_RTC_AlarmDateWeekDaySel_Date;				
			}

			Hal_Debug(INFO_LEVEL, "Set Alarm A WeekDay or Data (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Alarm DateWeekDay : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Alarm_A.RTC_AlarmDateWeekDay = temp_val;
//				Hal_Debug(INFO_LEVEL, "Set Alarm DateWeekDay to %d \r\n", temp_val);				
			}

			Hal_Debug(INFO_LEVEL, "Set Alarm A Hours (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Alarm Hours : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Alarm_A.RTC_AlarmTime.RTC_Hours = temp_val;
//				Hal_Debug(INFO_LEVEL, "Set Alarm Hours to %d \r\n", temp_val);				
			}
									
			Hal_Debug(INFO_LEVEL, "Set Alarm A Minutes (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Alarm Minutes : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Alarm_A.RTC_AlarmTime.RTC_Minutes = temp_val;
//				Hal_Debug(INFO_LEVEL, "Set Alarm Minutes to %d \r\n", temp_val);				
			}
			
			Hal_Debug(INFO_LEVEL, "Set Alarm A Seconds (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Alarm Seconds : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Alarm_A.RTC_AlarmTime.RTC_Seconds = temp_val;
//				Hal_Debug(INFO_LEVEL, "Set Alarm Seconds to %d \r\n", temp_val);				
			}

			Hal_Rtc_Set_AlarmTime(HAL_RTC_Alarm_A, &App_RTC_Alarm_A);			
		}
		else if(get_char == 'B' || get_char == 'b')
		{
			App_RTC_Alarm_X = HAL_RTC_Alarm_B;
			Hal_RTC_GetAlarm(RTC_Format_BINorBCD, App_RTC_Alarm_X, &App_RTC_Alarm_B);

			if(App_RTC_Alarm_B.RTC_AlarmDateWeekDaySel == HAL_RTC_AlarmDateWeekDaySel_WeekDay)
			{
				P_AlarmDateWeekDaySel = (uint8_t *)weekday_str;
			}
			else if(App_RTC_Alarm_B.RTC_AlarmDateWeekDaySel == HAL_RTC_AlarmDateWeekDaySel_Date)
			{
				P_AlarmDateWeekDaySel = (uint8_t *)data_str;			
			}
			
			Hal_Debug(INFO_LEVEL, "Current RTC_Alarm_B Time : H : %d m : %d s : %d - %s : %d \r\n",\
									App_RTC_Alarm_B.RTC_AlarmTime.RTC_Hours,\
									App_RTC_Alarm_B.RTC_AlarmTime.RTC_Minutes,\
									App_RTC_Alarm_B.RTC_AlarmTime.RTC_Seconds,\
									P_AlarmDateWeekDaySel,\
									App_RTC_Alarm_B.RTC_AlarmDateWeekDay);

			Hal_Debug(INFO_LEVEL, "Select Alarm B WeekDay or Data (W / D):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'W' || get_char == 'w')
			{
				App_RTC_Alarm_B.RTC_AlarmDateWeekDaySel = HAL_RTC_AlarmDateWeekDaySel_WeekDay;
			}
			else if(get_char == 'D' || get_char == 'd')
			{
				App_RTC_Alarm_B.RTC_AlarmDateWeekDaySel = HAL_RTC_AlarmDateWeekDaySel_Date;				
			}

			Hal_Debug(INFO_LEVEL, "Set Alarm B WeekDay or Data (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Alarm DateWeekDay : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Alarm_B.RTC_AlarmDateWeekDay = temp_val;
//				Hal_Debug(INFO_LEVEL, "Set Alarm DateWeekDay to %d \r\n", temp_val);				
			}

			Hal_Debug(INFO_LEVEL, "Set Alarm B Hours (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Alarm Hours : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Alarm_B.RTC_AlarmTime.RTC_Hours = temp_val;
//				Hal_Debug(INFO_LEVEL, "Set Alarm Hours to %d \r\n", temp_val);				
			}
									
			Hal_Debug(INFO_LEVEL, "Set Alarm B Minutes (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Alarm Minutes : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Alarm_B.RTC_AlarmTime.RTC_Minutes = temp_val;
//				Hal_Debug(INFO_LEVEL, "Set Alarm Minutes to %d \r\n", temp_val);				
			}
			
			Hal_Debug(INFO_LEVEL, "Set Alarm B Seconds (Y / N):");
			scanf("%c", &get_char);
			Hal_Debug(PRINTF, "%c\r\n", get_char);
			if(get_char == 'Y' || get_char == 'y')
			{
				Hal_Debug(INFO_LEVEL, "Please Enter Alarm Seconds : ");
				temp_val = App_Get_Val_From_Key();
				App_RTC_Alarm_B.RTC_AlarmTime.RTC_Seconds = temp_val;
//				Hal_Debug(INFO_LEVEL, "Set Alarm Seconds to %d \r\n", temp_val);				
			}

			Hal_Rtc_Set_AlarmTime(HAL_RTC_Alarm_B, &App_RTC_Alarm_B);
		}
		
	}

	
}

void App_Set_DataTimeAlarm(void)
{
	if(App_Get_DataTimeAlarm_Set_Flag() == APP_SET_DataTimeAlarm_FLAG_SET)
	{
		App_Set_DataTime();	

		App_Set_Alarm();
	}

	App_Set_DataTimeAlarm_Set_Flag(APP_SET_DataTimeAlarm_FLAG_RESET);
}


void App_Rtc_Show_DateTime(void)
{
	static uint8_t temp_sec;
	
	RTC_TimeTypeDef RTC_Time;
	RTC_DateTypeDef RTC_Date;
	float RTC_SubSec = 0.0f;
	Hal_Rtc_GetTime(RTC_Format_BINorBCD, &RTC_Time);
	
	Hal_Rtc_GetDate(RTC_Format_BINorBCD, &RTC_Date);
	
	RTC_SubSec = Hal_RTC_GetSubSecond();
	
	if(temp_sec != RTC_Time.RTC_Seconds)
	{
		//打印日期
		printf("The Date : Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d\r\n",  \
				RTC_Date.RTC_Year,	\
				RTC_Date.RTC_Month, \
				RTC_Date.RTC_Date, \
				RTC_Date.RTC_WeekDay);
		//打印时间		
		printf("The Time : %0.2d:%0.2d:%0.2d::%0.3f \r\n\r\n", \
				RTC_Time.RTC_Hours, 	\
				RTC_Time.RTC_Minutes,	\
				RTC_Time.RTC_Seconds,	\
				RTC_SubSec);	
	
		temp_sec = RTC_Time.RTC_Seconds;			
	}
}


void App_Rtc_DateTimeAlarm_Test_Task(void)
{
	Hal_Led_Init();

	Hal_Key_Init();	

	Hal_Rtc_DataTime_Init();

	App_DataTime_Alarm_Init();

	while(1)
	{
		App_Set_DataTimeAlarm();
	
		App_Rtc_Show_DateTime();	
	}
}


