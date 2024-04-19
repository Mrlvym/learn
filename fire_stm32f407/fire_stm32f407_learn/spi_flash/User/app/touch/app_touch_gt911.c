#include "app_touch_gt911.h"
#include "hal_touch_gt911.h"
#include "hal_debug_usart.h"
#include "app_nt35510_lcd.h"
#include "app_touch_task.h"
#include "hal_delay.h"


app_touch_gt911_dev_t g_app_gt911_dev;


//const uint8_t APP_TOUCH_CFG_GT911[] =  {
uint8_t APP_TOUCH_CFG_GT911[] =  {
#if 1
	0x55,0xE0,0x01,0x20,0x03,0x05,0x00,0x10,0x01,0xC8,
	0x28,0x0F,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
	0x11,0x11,0x05,0x18,0x1A,0x1E,0x14,0x88,0x29,0x0A,
	0x52,0x50,0x40,0x04,0x00,0x00,0x00,0x1A,0x32,0x1C,
	0x46,0x09,0x00,0x0F,0x00,0x2A,0xFF,0x7F,0x19,0x50,
	0x32,0x3C,0x64,0x94,0xD5,0x02,0x07,0x00,0x00,0x04,
	0x9F,0x3F,0x00,0x90,0x46,0x00,0x84,0x4D,0x00,0x79,
	0x55,0x00,0x6D,0x5F,0x00,0x6D,0x00,0x00,0x00,0x00,
	0xF0,0x4A,0x3A,0xFF,0xFF,0x27,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x14,0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,
	0x04,0x02,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0x22,0x21,0x20,0x1F,0x1E,0x1D,0x1C,0x18,
	0x16,0x12,0x10,0x0F,0x08,0x06,0x04,0x02,0x00,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x17,0x01

#else
	0x00,		//0x8047		版本号
//	0x20,0x03,	//0x8048/8049	X坐标输出最大值480
//	0xE0,0x01,	//0x804a/804b	Y坐标输出最大值854
	0xE0,0x01,	//0x8048/8049	X坐标输出最大值480
	0x56,0x03,	//0x804a/804b	Y坐标输出最大值854
	0x01,	//0x804c	输出触点个数上限
	0x35,	//0x804d	软件降噪，下降沿触发
	0x00,	//0x804e	reserved
	0x02,	//0x804f	手指按下去抖动次数
	0x08,	//0x8050	原始坐标窗口滤波值	
	0x28,	//0x8051	大面积触点个数
	0x0A,	//0x8052	噪声消除值
	0x5A,	//0x8053	屏上触摸点从无到有的阈值
	0x46,	//0x8054	屏上触摸点从有到无的阈值
	0x03,	//0x8055	进低功耗时间	s
	0x05,	//0x8056	坐标上报率
	0x00,	//0x8057	X坐标输出门上限
	0x00,	//0x8058	Y坐标输出门上限
	0x00,0X00,	//0x8059-0x805a	reserved
	0x00,	//0x805b	reserved
	0x00,	//0x805c	reserved
	0x00,	//0x805d	划线过程中小filter设置
	0x18,	//0x805e	拉伸区间	1	系数
	0x1A,	//0x805f	拉伸区间	2	系数
	0x1E,	//0x8060	拉伸区间	3	系数
	0x14,	//0x8061	各拉伸区间基数
	0x8C,	//0x8062	、、
	0x28,	//0x8063	、、
	0x0C,	//0x8064	、、
	0x71,	//0x8065	驱动组A的驱动频率倍频系数
	0x73,	//0x8066	驱动组B的驱动频率倍频系数
	0xB2,	//0x8067	驱动组A、B的基频
	0x04,	//0x8068	
	0x00,	//0x8069	相邻两次驱动信号输出时间间隔
	0x00,	//0x806a	
	0x00,	//0x806b	、、
	0x02,	//0x806c	、、
	0x03,	//0x806d	原始值放大系数
	0x1D,	//0x806e	、、
	0x00,	//0x806f	reserved
	0x01,	//0x8070	、、
	0x00,0x00,	//reserved
	0x00,	//0x8073	、、
	0x00,0x00,0x00,0x00,0x00,0x00,	//0x8071 - 0x8079 reserved
	0x50,	//0x807a	跳频范围的起点频率
	0xA0,	//0x807b	跳频范围的终点频率
	0x94,	//0x807c	多次噪声检测后确定噪声量，1-63有效
	0xD5,	//0x807d	噪声检测超时时间
	0x02,	//0x807e	、、	
	0x07,	//0x807f	判别有干扰的门限
	0x00,0x00,	//0x8081	reserved
	0x04,	//0x8082	跳频检测区间频段1中心点基频（适用于驱动A、B）
	0xA4,	//0x8083
	0x55,	//0x8084	跳频检测区间频段1中心点倍频系数
	0x00,	//0x8085	跳频检测区间频段2中心点基频(驱动A、B在此基础上换算)
	0x91,	//0x8086	
	0x62,	//0x8087	跳频检测区间频段2中心点倍频系数
	0x00,	//0x8088	跳频检测区间频段3中心点基频（适用于驱动A、B）
	0x80,	//0x8089	
	0x71,	//0x808a	跳频检测区间频段3中心点倍频系数
	0x00,	//0x808b	跳频检测区间频段4中心点基频（适用于驱动A、B）
	0x71,	//0x808c	
	0x82,	//0x808d	跳频检测区间频段4中心点倍频系数
	0x00,	//0x808e	跳频检测区间频段5中心点基频（适用于驱动A、B）
	0x65,	//0x808f	
	0x95,	//0x8090	跳频检测区间频段5中心点倍频系数
	0x00,	0x65,		//reserved
	0x00,	//0x8093	key1位置	0：无按键
	0x00,	//0x8094	key2位置	0：无按键
	0x00,	//0x8095	key3位置	0：无按键
	0x00,	//0x8096	key4位置	0：无按键
	0x00,	//0x8097	reserved
	0x00,	//0x8098	reserved
	0x00,	//0x8099	reserved
	0x00,	//0x809a	reserved
	0x00,	//0x809b	reserved
	0x00,	//0x809c	reserved
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x809d-0x80b2	reserved
	0x00,	//0x80b3	合框距离
	0x00,	//0x80b4	
	0x00,0x00,	//0x80b6	reserved
	0x06,	//0x80b7
	0x08,	//0x80b8
	0x0A,	//0x80b9
	0x0C,	//0x80ba
	0x0E,	//0x80bb
	0x10,	//0x80bc
	0x12,	//0x80bd
	0x14,	//0x80be
	0x16,	//0x80bf
	0x18,	//0x80c0
	0x1A,	//0x80c1
	0x1C,	//0x80c2
	0xFF,	//0x80c3
	0xFF,	//0x80c4
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,	//0x80c5-0x80d4	reserved
	0x00,	//0x80d5
	0x02,	//0x80d6
	0x04,	//0x80d7
	0x06,	//0x80d8
	0x08,	//0x80d9
	0x0A,	//0x80da
	0x0C,	//0x80db
	0x0F,	//0x80dc
	0x10,	//0x80dd
	0x12,	//0x80de
	0x13,	//0x80df
	0x14,	//0x80e0
	0x16,	//0x80e1
	0x18,	//0x80e2
	0x1C,	//0x80e3
	0x1D,	//0x80e4
	0x1E,	//0x80e5
	0x1F,	//0x80e6
	0x20,	//0x80e7
	0x21,	//0x80e8
	0xFF,	//0x80e9
	0xFF,	//0x80ea
	0xFF,	//0x80eb
	0xFF,	//0x80ec
	0xFF,	//0x80ed
	0xFF,	//0x80ee
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,	//0x80ef-0x80fe	reserved
	0x0B,	//0x80ff	配置信息校验
	0x01	//0x8100	配置以更新标记

#endif
};


uint8_t App_Touch_Gt911_Crc16(uint8_t *src_data, uint8_t len)
{
	uint8_t check_sum = 0, ret_crc = 0;
	uint8_t i;

	for (i = 0; i < len-2; i++)
    {
        check_sum += (src_data[i] & 0xFF);
    }
    ret_crc = (~(check_sum & 0xFF)) + 1; 	//checksum

	return ret_crc;
}


void App_Touch_Gt911_Soft_Reset(void)
{
    uint8_t buf[1];
	
    buf[0] = 0x02;
    Hal_Touch_Gt911_WR_Reg(APP_GT911_COMMAND_REG, (uint8_t *)buf, 1);
	Hal_Delay_ms(10);

    buf[0] = 0x00;
    Hal_Touch_Gt911_WR_Reg(APP_GT911_COMMAND_REG, (uint8_t *)buf, 1);
}


uint8_t App_Touch_Gt911_ReadID(void)
{
	uint8_t ret = 0;
//	uint8_t gt911_id[5];

	Hal_Touch_Gt911_RD_Reg(APP_GT911_PRODUCT_ID_REG, g_app_gt911_dev.gt911_id, 4);

	g_app_gt911_dev.gt911_id[4] = 0;

	Hal_Debug(INFO_LEVEL, "gt911_id : %s \r\n", g_app_gt911_dev.gt911_id);

	if(g_app_gt911_dev.gt911_id[0] == '9' && \
	   g_app_gt911_dev.gt911_id[1] == '1' && \
	   g_app_gt911_dev.gt911_id[2] == '1' )
	{
		ret = 0;
	}
	else
	{
		ret = 1;
	}
	
	return ret;
}

void App_Touch_Gt911_Set_XY_Point(uint16_t x, uint16_t y, uint8_t p_num)
{
	APP_TOUCH_CFG_GT911[1] = (uint8_t)(x&0xff);
	APP_TOUCH_CFG_GT911[2] = (uint8_t)((x>>8)&0xff);

	APP_TOUCH_CFG_GT911[3] = (uint8_t)(y&0xff);
	APP_TOUCH_CFG_GT911[4] = (uint8_t)((y>>8)&0xff);	

	APP_TOUCH_CFG_GT911[5] = (uint8_t)p_num;
}

uint8_t App_Touch_Gt911_Init(void)
{
	uint8_t ret = 0;
//	uint8_t lcd_dir = 0;

	Hal_Touch_Gt911_Init();
	
	ret = App_Touch_Gt911_ReadID();
	if(ret == 1){
		Hal_Debug(DEBUG_LEVEL, "gt911 read id err !\r\n");
	}

//	lcd_dir = App_LCD_DisplayDir_Get();
//
//	if(lcd_dir == APP_LCD_H)
//	{
//		g_app_gt911_dev.gt911_x = 800;
//		g_app_gt911_dev.gt911_y = 480;
//	}
//	else if(lcd_dir == APP_LCD_V)
//	{
		g_app_gt911_dev.gt911_x = 480;
		g_app_gt911_dev.gt911_y = 800;
//	}
	g_app_gt911_dev.gt911_p_num = APP_GT911_POINT_NUM;

	App_Touch_Gt911_Set_XY_Point(g_app_gt911_dev.gt911_x, g_app_gt911_dev.gt911_y, g_app_gt911_dev.gt911_p_num);

	App_Touch_Gt911_Cfg();

	App_Touch_Gt911_Soft_Reset();

//	Hal_Touch_Gt911_Int_Irq_Cfg();
	
	return ret;
}

uint8_t App_Touch_Gt911_Cfg(void)
{
	uint8_t temp[1];
	uint16_t cfg_num = 0;

	cfg_num = (sizeof(APP_TOUCH_CFG_GT911) / sizeof(APP_TOUCH_CFG_GT911[0]));

	Hal_Debug(DEBUG_LEVEL, "gt911 cfg num : %d \r\n", cfg_num);

	Hal_Touch_Gt911_RD_Reg(APP_GT911_CONFIG_REG, temp, 1);
	Hal_Debug(DEBUG_LEVEL, "gt911 cfg version : %x \r\n", temp[0]);
		
//	if(temp[0] > 0x60)//如果版本大于0x60
//		return 1;

	APP_TOUCH_CFG_GT911[0] = 0x00;

	APP_TOUCH_CFG_GT911[cfg_num - 2] = App_Touch_Gt911_Crc16(APP_TOUCH_CFG_GT911, cfg_num);

	APP_TOUCH_CFG_GT911[cfg_num - 1] = 0x01;

	Hal_Touch_Gt911_WR_Reg(APP_GT911_CONFIG_REG, APP_TOUCH_CFG_GT911, cfg_num);

	Hal_Delay_ms(200);

	return 0;
}


/**
  * @brief   用于处理或报告触屏检测到按下
  * @param
  *    @arg     id: 触摸顺序trackID
  *    @arg     x:  触摸的 x 坐标
  *    @arg     y:  触摸的 y 坐标
  *    @arg     w:  触摸的 大小
  * @retval 无
  */
/*用于记录连续触摸时(长按)的上一次触摸位置，负数值表示上一次无触摸按下*/
static int16_t pre_x[APP_GT911_POINT_NUM] ={-1,-1,-1,-1,-1};
static int16_t pre_y[APP_GT911_POINT_NUM] ={-1,-1,-1,-1,-1};

static void App_Touch_Gt911_Down(int32_t id,int32_t x,int32_t y,int32_t w)
{
	/*取x、y初始值大于屏幕像素值*/
    Hal_Debug(DEBUG_LEVEL, "ID:%d, X:%d, Y:%d, W:%d \r\n", id, x, y, w);

	
    /* 处理触摸按钮，用于触摸画板 */
    Touch_Button_Down(x,y); 
	

    /*处理描绘轨迹，用于触摸画板 */
    Draw_Trail(pre_x[id],pre_y[id],x,y,&brush);
	
	/************************************/
	/*在此处添加自己的触摸点按下时处理过程即可*/
	/* (x,y) 即为最新的触摸点 *************/
	/************************************/

	/*prex,prey数组存储上一次触摸的位置，id为轨迹编号(多点触控时有多轨迹)*/
	pre_x[id] = x; pre_y[id] =y;
	
}


/**
  * @brief   用于处理或报告触屏释放
  * @param 释放点的id号
  * @retval 无
  */
static void App_Touch_Gt911_Up( int32_t id)
{
	

    /*处理触摸释放,用于触摸画板*/
    Touch_Button_Up(pre_x[id],pre_y[id]);

	/*****************************************/
	/*在此处添加自己的触摸点释放时的处理过程即可*/
	/* pre_x[id],pre_y[id] 即为最新的释放点 ****/
	/*******************************************/	
	/***id为轨迹编号(多点触控时有多轨迹)********/
	
	
    /*触笔释放，把pre xy 重置为负*/
	  pre_x[id] = -1;
	  pre_y[id] = -1;		
  
    Hal_Debug(DEBUG_LEVEL, "Touch id[%2d] release!\r\n", id);

}


void App_Touch_Gt911_Work_Func(void)
{
	uint8_t clear_buf[1];
	uint8_t point_data[1+APP_GT911_POINT_NUM*8];
	uint8_t  touch_num = 0;
    uint8_t  finger = 0;
	static uint16_t pre_touch = 0;
    static uint8_t pre_id[APP_GT911_POINT_NUM] = {0};

	uint8_t* coor_data = NULL;
    int32_t input_x = 0;
    int32_t input_y = 0;
    int32_t input_w = 0;
    uint8_t id = 0;

	uint8_t i = 0, j = 0;

//	uint8_t lcd_dir = 0;

	Hal_Touch_Gt911_RD_Reg(APP_GT911_READ_XY_REG, point_data, 1);
	finger = point_data[0];

	if(finger ==  0x00)
		return ;

	if(finger&0x80 == 0x00)
		goto clear_point_info;

	touch_num = finger&0x0f;
	if(touch_num > APP_GT911_POINT_NUM)
		goto clear_point_info;

	Hal_Touch_Gt911_RD_Reg(APP_GT911_READ_XY_REG, point_data, touch_num*8+1);

	if(pre_touch > touch_num)
	{
		for(i = 0; i < pre_touch; i++)
		{
			for(j = 0; j < touch_num; j++)
			{
				coor_data = (uint8_t *)&point_data[j*8+1];
				id = coor_data[0] & 0x0F;
				if(id == pre_id[i])
					break;

				if(j >= touch_num-1)//释放已经松开的
				{
					App_Touch_Gt911_Up( pre_id[i]);
				}
			}
		}
	}

	if(touch_num)
	{
		for(i = 0; i < touch_num; i++)
		{
			coor_data = (uint8_t *)&point_data[i*8+1];
			
			id = coor_data[0] & 0x0F;						//track id
			pre_id[i] = id;

			input_x  = coor_data[1] | (coor_data[2] << 8);	//x坐标
			input_y  = coor_data[3] | (coor_data[4] << 8);	//y坐标
			input_w  = coor_data[5] | (coor_data[6] << 8);	//size

//			lcd_dir = App_LCD_DisplayDir_Get();
//		
//			if(lcd_dir == APP_LCD_H)
//			{
//				
//			}
//			else if(lcd_dir == APP_LCD_H)
//			{
//				
//			}

			App_Touch_Gt911_Down( id, input_x, input_y, input_w);//数据处理
		}
	}
	else if(pre_touch)
	{
		for(i = 0; i < pre_touch; i++)
		{
			App_Touch_Gt911_Up( pre_id[i]);
		}
	}

	pre_touch = touch_num;

clear_point_info:
	clear_buf[0] = 0;
	Hal_Touch_Gt911_WR_Reg(APP_GT911_CLEARBUF_REG, clear_buf, 1);
}


void App_Touch_Gt911_Int_CallBack(void)
{
//	Hal_Debug(PRINTF, "%s \r\n", __func__);
}

