#ifndef __APP_NT35510_LCD_H__
#define __APP_NT35510_LCD_H__	
#include "stdint.h"

//LCD��Ҫ������
typedef struct  
{										    
	uint16_t 	width;			//LCD ���
	uint16_t 	height;			//LCD �߶�
	uint16_t 	id;				//LCD ID
	uint8_t  	dir;			//���������������ƣ�0��������1��������	
	uint16_t	wramcmd;		//��ʼдgramָ��
	uint16_t	rramcmd;		//��ʼ��gramָ��
	uint16_t  	setxcmd;		//����x����ָ��
	uint16_t  	setycmd;		//����y����ָ�� 
}app_lcd_dev_t; 

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)



#define APP_LCD_DEF_DIR	APP_LCD_V //Ĭ����Ļ����

#define APP_LCD_H	1 //���� w 800 h 480	
#define APP_LCD_V	0 //���� w 480 h 800	

#define APP_LCD_SCAN_L2R_U2D	0	//������,���ϵ���
#define APP_LCD_SCAN_L2R_D2U	1	//������,���µ���
#define APP_LCD_SCAN_R2L_U2D	2	//���ҵ���,���ϵ���
#define APP_LCD_SCAN_R2L_D2U	3	//���ҵ���,���µ���
#define APP_LCD_SCAN_U2D_L2R	4	//���ϵ���,������
#define APP_LCD_SCAN_U2D_R2L	5	//���ϵ���,���ҵ���
#define APP_LCD_SCAN_D2U_L2R	6	//���µ���,������
#define APP_LCD_SCAN_D2U_R2L	7	//���µ���,���ҵ���

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

//����
//x1,y1:�������
//x2,y2:�յ�����  
void App_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void App_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void App_LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color);
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void App_LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void App_LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);
//���û������ɫ�ͱ�����ɫ
void App_LCD_Set_Point_Back_Color(uint16_t point_color, uint16_t back_color);
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void App_LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);
//m^n����
//����ֵ:m^n�η�.
uint32_t App_LCD_Pow(uint8_t m,uint8_t n);
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void App_LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void App_LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void App_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);



#endif

