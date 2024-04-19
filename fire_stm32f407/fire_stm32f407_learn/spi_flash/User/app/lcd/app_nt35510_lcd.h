#ifndef __APP_NT35510_LCD_H__
#define __APP_NT35510_LCD_H__	
#include "stdint.h"

//LCD重要参数集
typedef struct  
{										    
	uint16_t 	width;			//LCD 宽度
	uint16_t 	height;			//LCD 高度
	uint16_t 	id;				//LCD ID
	uint8_t  	dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t	rramcmd;		//开始读gram指令
	uint16_t  	setxcmd;		//设置x坐标指令
	uint16_t  	setycmd;		//设置y坐标指令 
}app_lcd_dev_t; 

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



#define APP_LCD_DEF_DIR	APP_LCD_V //默认屏幕方向

#define APP_LCD_H	1 //横屏 w 800 h 480	
#define APP_LCD_V	0 //竖屏 w 480 h 800	

#define APP_LCD_SCAN_L2R_U2D	0	//从左到右,从上到下
#define APP_LCD_SCAN_L2R_D2U	1	//从左到右,从下到上
#define APP_LCD_SCAN_R2L_U2D	2	//从右到左,从上到下
#define APP_LCD_SCAN_R2L_D2U	3	//从右到左,从下到上
#define APP_LCD_SCAN_U2D_L2R	4	//从上到下,从左到右
#define APP_LCD_SCAN_U2D_R2L	5	//从上到下,从右到左
#define APP_LCD_SCAN_D2U_L2R	6	//从下到上,从左到右
#define APP_LCD_SCAN_D2U_R2L	7	//从下到上,从右到左

app_lcd_dev_t App_Get_LCD_DevInfo(void);
void App_Set_LCD_DevInfo(app_lcd_dev_t lcd_info);

void App_LCD_WR_Cmd(uint16_t cmd);
void App_LCD_WR_Data(uint16_t w_data);
uint16_t App_LCD_RD_Data(void);
void App_LCD_WriteReg(uint16_t lcd_w_reg, uint16_t lcd_reg_w_val);
uint16_t App_LCD_ReadReg(uint16_t lcd_r_reg);

void App_LCD_Init(void);
void App_LCD_BL_On(void);
void App_LCD_BL_Off(void);
uint32_t App_LCD_ReadID(void);
void App_LCD_NT35510_Init(void);

void App_LCD_Display_On(void);
void App_LCD_Display_Off(void);

void App_LCD_DisplayDir_Set(uint8_t dis_dir);
uint8_t App_LCD_DisplayDir_Get(void);
void App_LCD_Scan_Dir(uint8_t scan_dir);
void App_LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
uint8_t App_LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);

void App_LCD_W_Gram(void);
void App_LCD_R_Gram(void);

void App_LCD_Clear(uint16_t color);
void App_LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
uint16_t App_LCD_ReadPont(uint16_t x, uint16_t y);

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void App_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void App_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void App_LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color);
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void App_LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void App_LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);
//设置画点的颜色和背景颜色
void App_LCD_Set_Point_Back_Color(uint16_t point_color, uint16_t back_color);
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void App_LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);
//m^n函数
//返回值:m^n次方.
uint32_t App_LCD_Pow(uint8_t m,uint8_t n);
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void App_LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void App_LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void App_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);



#endif

