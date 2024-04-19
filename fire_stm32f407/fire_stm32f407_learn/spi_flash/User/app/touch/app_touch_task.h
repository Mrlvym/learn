#ifndef __APP_TOUCH_TASK_H__
#define __APP_TOUCH_TASK_H__

#include "stdint.h"

#define LCD_X_LENGTH 480
#define LCD_Y_LENGTH 800

#define COLOR_BLOCK_WIDTH   50//90
#define COLOR_BLOCK_HEIGHT  90//50

#define BUTTON_NUM 18

#define PALETTE_START_Y   0
#define PALETTE_END_Y     LCD_Y_LENGTH-COLOR_BLOCK_HEIGHT*2//LCD_Y_LENGTH

#if 1     //��ť�������
  #define BUTTON_START_X      0//0
  #define BUTTON_START_Y      LCD_Y_LENGTH-COLOR_BLOCK_HEIGHT//0
  #define PALETTE_START_X   0//(COLOR_BLOCK_WIDTH*2+1)
  #define PALETTE_END_X     LCD_X_LENGTH

#else     //��ť�����ұߣ�(���ڴ�������ʱҲ���bug�����ڲ��Դ�������߽�)
  #define BUTTON_START_X     ( LCD_X_LENGTH-2*COLOR_BLOCK_WIDTH)
  #define PALETTE_START_X   0
  #define PALETTE_END_X     (LCD_X_LENGTH-2*COLOR_BLOCK_WIDTH)

#endif

/*
	LCD ��ɫ���룬CL_��Color�ļ�д
	16Bit�ɸ�λ����λ�� RRRR RGGG GGGB BBBB

	�����RGB �꽫24λ��RGBֵת��Ϊ16λ��ʽ��
	����windows�Ļ��ʳ��򣬵���༭��ɫ��ѡ���Զ�����ɫ�����Ի�õ�RGBֵ��

	�Ƽ�ʹ������ȡɫ����������㿴���Ľ�����ɫ��
*/
#if LCD_RGB_888
/*RGB888��ɫת��*/
#define RGB(R,G,B)	( (R<< 16) | (G << 8) | (B))	/* ��8λR,G,Bת��Ϊ 24λRGB888��ʽ */

#else 
/*RGB565 ��ɫת��*/
#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	/* ��8λR,G,Bת��Ϊ 16λRGB565��ʽ */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)

#endif


enum
{
	CL_WHITE    = RGB(255,255,255),	/* ��ɫ */
	CL_BLACK    = RGB(  0,  0,  0),	/* ��ɫ */
	CL_RED      = RGB(255,	0,  0),	/* ��ɫ */
	CL_GREEN    = RGB(  0,255,  0),	/* ��ɫ */
	CL_BLUE     = RGB(  0,	0,255),	/* ��ɫ */
	CL_YELLOW   = RGB(255,255,  0),	/* ��ɫ */

	CL_GREY    = RGB( 98, 98, 98), 	/* ���ɫ */
	CL_GREY1		= RGB( 150, 150, 150), 	/* ǳ��ɫ */
	CL_GREY2		= RGB( 180, 180, 180), 	/* ǳ��ɫ */
	CL_GREY3		= RGB( 200, 200, 200), 	/* ��ǳ��ɫ */
	CL_GREY4		= RGB( 230, 230, 230), 	/* ��ǳ��ɫ */

	CL_BUTTON_GREY	= RGB( 220, 220, 220), /* WINDOWS ��ť�����ɫ */

	CL_MAGENTA      = RGB(255, 0, 255),	/* ����ɫ�����ɫ */
	CL_CYAN         = RGB( 0, 255, 255),	/* ����ɫ����ɫ */

	CL_BLUE1        = RGB(  0,  0, 240),		/* ����ɫ */
	CL_BLUE2        = RGB(  0,  0, 128),		/* ����ɫ */
	CL_BLUE3        = RGB(  68, 68, 255),		/* ǳ��ɫ1 */
	CL_BLUE4        = RGB(  0, 64, 128),		/* ǳ��ɫ1 */

	/* UI ���� Windows�ؼ�����ɫ */
	CL_BTN_FACE		  = RGB(236, 233, 216),	/* ��ť������ɫ(��) */
	CL_BOX_BORDER1	= RGB(172, 168,153),	/* �����������ɫ */
	CL_BOX_BORDER2	= RGB(255, 255,255),	/* �������Ӱ����ɫ */

	CL_MASK			    = 0x7FFF	/* RGB565��ɫ���룬�������ֱ���͸�� */
};

typedef struct 
{
  uint16_t start_x;   //������x��ʼ����  
  uint16_t start_y;   //������y��ʼ����
  uint16_t end_x;     //������x�������� 
  uint16_t end_y;     //������y��������
  uint32_t para;      //��ɫ��ť�б�ʾѡ�����ɫ���ʼ���״��ť�б�ʾѡ��Ļ�ˢ
  uint8_t touch_flag; //�������µı�־
    
  void (*draw_btn)(void * btn);     //������溯��
  void (*btn_command)(void * btn);  //��������ִ�к����������л���ɫ����ˢ
 
}Touch_Button;

/*��ˢ��״�б�*/
typedef enum 
{
  LINE_SINGLE_PIXCEL = 0,   //��������
  
  LINE_2_PIXCEL,  //2������
  
  LINE_4_PIXCEL,  //4������
  
  LINE_6_PIXCEL,  //6������
  
  LINE_8_PIXCEL,  //8������
  
  LINE_16_PIXCEL, //16������
  
  LINE_20_PIXCEL, //20������
  
  LINE_WITH_CIRCLE,  //��������
    
  RUBBER,           //��Ƥ

}SHAPE;


/*��ˢ����*/
typedef struct
{
  uint32_t color;
  
  SHAPE  shape;
  
}Brush_Style;

/*���ʲ���*/
extern Brush_Style brush;


void App_Touch_Task_Init(void);
void App_Touch_Task(void);

void Palette_Init(void);

void Touch_Button_Down(uint16_t x,uint16_t y);
void Touch_Button_Up(uint16_t x,uint16_t y);
void Draw_Trail(int16_t pre_x,int16_t pre_y,int16_t x,int16_t y,Brush_Style* brush);
void Touch_Button_Init(void);

void NT35510_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint16_t color, uint8_t ucFilled );
void NT35510_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 ,uint16_t color);
void NT35510_DrawLineCircle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t, uint8_t thick );
void NT35510_DrawCircle ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint16_t color, uint8_t ucFilled );


#endif
