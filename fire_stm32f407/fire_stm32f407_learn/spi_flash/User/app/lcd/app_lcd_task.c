#include "app_lcd_task.h"
#include "app_nt35510_lcd.h"
#include "hal_debug_usart.h"
#include "hal_delay.h"

void App_LCD_Task_Init(void)
{
	uint16_t lcd_id = 0;
	app_lcd_dev_t app_task_lcd_dev;

	App_LCD_Init();

	lcd_id = App_LCD_ReadID();
	if(lcd_id != 0x5510)
		Hal_Debug(DEBUG_LEVEL, "LCD read id error : %x \r\n", lcd_id);

	//初始化nt35510
	App_LCD_NT35510_Init();

	//默认屏幕方向 和扫描方向
	App_LCD_DisplayDir_Set(APP_LCD_DEF_DIR);

	//设置屏幕窗口大小
	app_task_lcd_dev = App_Get_LCD_DevInfo();
	App_LCD_Set_Window(0, 0, app_task_lcd_dev.width, app_task_lcd_dev.height);

	//开启lcd背光
	App_LCD_BL_On();
	
	//清屏
	App_LCD_Clear(WHITE);
}

void App_LCD_ScreenFlush_Test(void)
{
	Hal_Debug(DEBUG_LEVEL, "LCD Screen Flush Test ... \r\n");

	App_LCD_Clear(WHITE);
	Hal_Delay_ms(1000);
	App_LCD_Clear(BLACK);
	Hal_Delay_ms(1000);
	App_LCD_Clear(BLUE);
	Hal_Delay_ms(1000);
	App_LCD_Clear(BRED);
	Hal_Delay_ms(1000);
	App_LCD_Clear(GRED);
	Hal_Delay_ms(1000);
	App_LCD_Clear(GBLUE);
	Hal_Delay_ms(1000);	
	App_LCD_Clear(RED);
	Hal_Delay_ms(1000);
	App_LCD_Clear(MAGENTA);
	Hal_Delay_ms(1000);
	App_LCD_Clear(GREEN);
	Hal_Delay_ms(1000);
	App_LCD_Clear(CYAN);
	Hal_Delay_ms(1000);
	App_LCD_Clear(YELLOW);
	Hal_Delay_ms(1000);
	App_LCD_Clear(BROWN);
	Hal_Delay_ms(1000);
	App_LCD_Clear(BRRED);
	Hal_Delay_ms(1000);
	App_LCD_Clear(BROWN);
	Hal_Delay_ms(1000);
	App_LCD_Clear(GRAY);
	Hal_Delay_ms(1000);
}

void App_LCD_DrawLine_Test(void)
{
	Hal_Debug(DEBUG_LEVEL, "LCD Draw Line Test ... \r\n");

	App_LCD_Clear(WHITE);
	Hal_Delay_ms(1000);
	
	App_LCD_DrawLine(40, 100, 440, 100, RED);
	Hal_Delay_ms(1000);
	App_LCD_DrawLine(40, 100, 440, 700, RED);
	Hal_Delay_ms(1000);
	App_LCD_DrawLine(440, 100, 40, 700, RED);
	Hal_Delay_ms(1000);
	App_LCD_DrawLine(40, 700, 440, 700, RED);
	Hal_Delay_ms(1000);

	App_LCD_DrawRectangle(80, 200, 400, 600, BLUE);
	Hal_Delay_ms(1000);
	App_LCD_DrawRectangle(120, 300, 360, 500, BLUE);
	Hal_Delay_ms(1000);
	
	App_LCD_Draw_Circle(240, 400, 200, GREEN);
	Hal_Delay_ms(1000);
	App_LCD_Draw_Circle(240, 400, 100, GREEN);
	Hal_Delay_ms(1000);

	App_LCD_Fill(100, 200, 380, 600, YELLOW);
	Hal_Delay_ms(1000);
}

void App_LCD_ShowString_Test(void)
{
	uint32_t i = 0;

	Hal_Debug(DEBUG_LEVEL, "LCD Show String Test ... \r\n");

	App_LCD_Clear(WHITE);
	Hal_Delay_ms(1000);

	App_LCD_Set_Point_Back_Color(RED, WHITE);

	App_LCD_ShowString(30,40,180,24,24,"Explorer STM32F4");	
	App_LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	App_LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK"); 
	App_LCD_ShowString(30,110,200,16,16,"Test Num :");		//显示LCD ID
	App_LCD_ShowString(30,130,200,12,12,"2023/11/11");

	for(i = 0; i < 20; i++)
	{
//		App_LCD_ShowNum(110,110, i, 2, 16);
		App_LCD_ShowxNum(120,110, i, 2, 16, 0x80);
		Hal_Delay_ms(1000);
	}

	
}

void App_LCD_ReadPoint_Test(void)
{
	uint16_t temp_color = 0;

	Hal_Debug(DEBUG_LEVEL, "LCD Read Point Test ... \r\n");

	App_LCD_DrawPoint(100, 100, GRAY);

	temp_color = App_LCD_ReadPont(100, 100);

	if(GRAY == temp_color)
	{
		Hal_Debug(DEBUG_LEVEL, "LCD Read Point Test success ... \r\n");
	}
	else
	{
		Hal_Debug(DEBUG_LEVEL, "LCD Read Point Test failed : %x ... \r\n", temp_color);
	}
}
void App_LCD_Task(void)
{
	App_LCD_Task_Init();

	App_LCD_ScreenFlush_Test();

	App_LCD_DrawLine_Test();

	App_LCD_ShowString_Test();

	App_LCD_ReadPoint_Test();
}


