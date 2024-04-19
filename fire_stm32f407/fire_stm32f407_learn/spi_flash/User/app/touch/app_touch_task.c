#include "app_touch_task.h"
#include "app_touch_gt911.h"
#include "app_nt35510_lcd.h"
#include "hal_nt35510_lcd.h"
#include "app_lcd_task.h"


/*��ť�ṹ������*/
Touch_Button button[BUTTON_NUM];

/*���ʲ���*/
Brush_Style brush;

void App_Touch_Task_Init(void)
{
	App_Touch_Gt911_Init();
}

void App_Touch_Task(void)
{
	App_LCD_Task_Init();

	App_Touch_Task_Init();

	Palette_Init();

	while(1)
	{
		App_Touch_Gt911_Work_Func();
	}
}

/**
* @brief  Palette_Init �����ʼ��
* @param  ��
* @retval ��
*/
void Palette_Init(void)
{
  
  uint8_t i;
	
//  NT35510_GramScan ( LCD_Mode );
//
//  /* ������Ϊ��ɫ */
//	LCD_SetBackColor(CL_WHITE);
//  NT35510_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	

  
  /* ��ʼ����ť */
  Touch_Button_Init();
  
  /* ��水ť */
  for(i=0;i<BUTTON_NUM;i++)
  {
    button[i].draw_btn(&button[i]);
  }
  
	
  /* ��ʼ������ */
  brush.color = CL_BLACK;
  brush.shape = LINE_SINGLE_PIXCEL;
	
//	LCD_SetTextColor(brush.color);


}

/**
* @brief  Touch_Button_Down ����������ʱ���õĺ������ɴ���������
* @param  x ����λ�õ�x����
* @param  y ����λ�õ�y����
* @retval ��
*/
void Touch_Button_Down(uint16_t x,uint16_t y)
{
  uint8_t i;
  for(i=0;i<BUTTON_NUM;i++)
  {
    /* �������˰�ť */
    if(x<=button[i].end_x && y<=button[i].end_y && y>=button[i].start_y && x>=button[i].start_x )
    {
      if(button[i].touch_flag == 0)     /*ԭ����״̬Ϊû�а��£������״̬*/
      {
      button[i].touch_flag = 1;         /* ��¼���±�־ */
      
      button[i].draw_btn(&button[i]);  /*�ػ水ť*/
      }        
      
    }
    else if(button[i].touch_flag == 1) /* �����Ƴ��˰����ķ�Χ��֮ǰ�а��°�ť */
    {
      button[i].touch_flag = 0;         /* ������±�־���ж�Ϊ�����*/
      
      button[i].draw_btn(&button[i]);   /*�ػ水ť*/
    }

  }

}

/**
* @brief  Touch_Button_Up �������ͷ�ʱ���õĺ������ɴ���������
* @param  x ��������ͷ�ʱ��x����
* @param  y ��������ͷ�ʱ��y����
* @retval ��
*/
void Touch_Button_Up(uint16_t x,uint16_t y)
{
   uint8_t i; 
   for(i=0;i<BUTTON_NUM;i++)
   {
     /* �����ڰ�ť�����ͷ� */
      if((x<button[i].end_x && x>button[i].start_x && y<button[i].end_y && y>button[i].start_y))
      {        
        button[i].touch_flag = 0;       /*�ͷŴ�����־*/
        
        button[i].draw_btn(&button[i]); /*�ػ水ť*/        
      
        button[i].btn_command(&button[i]);  /*ִ�а����Ĺ�������*/
        
        break;
      }
    }  

}

/**
* @brief  Draw_Trail �ڻ���������津���켣
* @param  pre_x ��һ���x����
* @param  pre_y ��һ���y����
* @param  x     ����һ���x����
* @param  y     ����һ���y����
* @param  brush ��ˢ����
* @retval ��
*/
void Draw_Trail(int16_t pre_x,int16_t pre_y,int16_t x,int16_t y,Brush_Style* brush)
{
  /*���û�������Ϊ�����,bsp_lcd.c������û�������ĺ������������ƻ滭����*/
//  RA8875_SetActiveWindow(PALETTE_START_X,PALETTE_START_Y,PALETTE_END_X,PALETTE_END_Y);
  
	
  /*����λ���ڻ�������*/
//  if(x>PALETTE_START_X && pre_x>PALETTE_START_X )
  if(x>PALETTE_START_X && pre_x>PALETTE_START_X && y<PALETTE_END_Y && pre_y<PALETTE_END_Y)
  {
    switch(brush->shape)  /*���ݻ�ˢ������治ͬ�Ĺ켣*/
    {
      /* ���1���ؿ�ȵĹ켣�� */
      case LINE_SINGLE_PIXCEL:                 
            if(pre_x< 0 || pre_y < 0) //�µıʼ�
            {      
              //PutPixel(x,y);              
            }
            else //������һ�εıʼ�
            {      
              NT35510_DrawLine(pre_x,pre_y,x,y, brush->color);
            } 
        
        break;
            
      case LINE_2_PIXCEL:
        
				if(x-1<PALETTE_START_X||pre_x-1<PALETTE_START_X)	//������߽�
					break;

        /* ���2���ؿ�ȵĹ켣�� */
        NT35510_DrawLineCircle(pre_x,pre_y,x,y,brush->color,1);
        
        break;
      
      case LINE_4_PIXCEL:
				
				if(x-2<PALETTE_START_X||pre_x-2<PALETTE_START_X)	//������߽�
					break;
        
            NT35510_DrawLineCircle(pre_x,pre_y,x,y,brush->color,2);

        break;
      
      case LINE_6_PIXCEL:
        
				if(x-3<PALETTE_START_X||pre_x-3<PALETTE_START_X)	//������߽�
					break;
      
        NT35510_DrawLineCircle(pre_x,pre_y,x,y,brush->color,3);
        
        break;
      
      case LINE_8_PIXCEL:
				
				if(x-4<PALETTE_START_X||pre_x-4<PALETTE_START_X)	//������߽�
					break;
        
            NT35510_DrawLineCircle(pre_x,pre_y,x,y,brush->color,4);

        break;
      
     case LINE_16_PIXCEL:
			 
		 		if(x-8<PALETTE_START_X||pre_x-8<PALETTE_START_X)	//������߽�
					break;
        
            NT35510_DrawLineCircle(pre_x,pre_y,x,y,brush->color,8);

        break;
            
      case LINE_20_PIXCEL:
				
				if(x-10<PALETTE_START_X ||pre_x-10<PALETTE_START_X)	//������߽�
					break;
        
            NT35510_DrawLineCircle(pre_x,pre_y,x,y,brush->color,10);

        break;
      
      /*�������ӵĵ�������*/
      case LINE_WITH_CIRCLE:  

		 		if(x-3<PALETTE_START_X||pre_x-3<PALETTE_START_X)	//������߽�
					break;			
           
            if(pre_x< 0 || pre_y< 0)//�µıʼ�
            {      
             // PutPixel(x,y); 
            }
            else //������һ�εıʼ�
            {      
              NT35510_DrawLine(pre_x,pre_y,x,y,brush->color);
              NT35510_DrawCircle(x,y,3,brush->color,1);
            } 
        
        break;
           
      /*��Ƥ����*/            
      case RUBBER:
				
				if(x-20<PALETTE_START_X ||						//������߽�
					  x+20>LCD_X_LENGTH || x-20<0 || //Һ�����ұ߽�
						y+20>LCD_Y_LENGTH || y-20<0)	 //Һ�����±߽�				
					break;	
			
//        if(x>PALETTE_START_X+20)
        {
//						LCD_SetColors(CL_WHITE,CL_WHITE);
            
						NT35510_DrawRectangle( x-40/2,
                                y-40/2,
                                40,
                                40,brush->color,1);   
      
        }
      break;

    }
  }
  
  /*�˳����޻���Ļ�ͼ���ڣ�bsp_lcd.c������û�������ĺ������������ƻ滭����*/
//  RA8875_SetActiveWindow(0,0,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT);


}

static void Draw_Color_Button(void *btn);
static void Draw_Clear_Button(void *btn);
static void Draw_Shape_Button(void *btn);
static void Command_Select_Brush(void *btn);
static void Command_Clear_Palette(void *btn);
static void Command_Select_Color(void *btn);



/**
* @brief  Touch_Button_Init ��ʼ����ť����
* @param  ��
* @retval ��
*/
void Touch_Button_Init(void)
{
	/*��һ�У���ҪΪ��ɫ��ť*/
  button[0].start_x = BUTTON_START_X;
  button[0].start_y = BUTTON_START_Y;
  button[0].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*1;
  button[0].end_y = BUTTON_START_Y+COLOR_BLOCK_HEIGHT;
  button[0].para = CL_BLACK;
  button[0].touch_flag = 0;  
  button[0].draw_btn = Draw_Color_Button ;
  button[0].btn_command = Command_Select_Color ;
  
  button[1].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*1;
  button[1].start_y = BUTTON_START_Y;
  button[1].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*2;
  button[1].end_y = BUTTON_START_Y+COLOR_BLOCK_HEIGHT;
  button[1].para = CL_GREY;
  button[1].touch_flag = 0;  
  button[1].draw_btn = Draw_Color_Button ;
  button[1].btn_command = Command_Select_Color ;
  
  button[2].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*2;
  button[2].start_y = BUTTON_START_Y;
  button[2].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*3;
  button[2].end_y = BUTTON_START_Y+COLOR_BLOCK_HEIGHT;
  button[2].para = CL_BLUE;
  button[2].touch_flag = 0;  
  button[2].draw_btn = Draw_Color_Button ;
  button[2].btn_command = Command_Select_Color ;
  
  button[3].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*3;
  button[3].start_y = BUTTON_START_Y;
  button[3].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*4;
  button[3].end_y = BUTTON_START_Y+COLOR_BLOCK_HEIGHT;
  button[3].para = CL_BLUE2;
  button[3].touch_flag = 0;  
  button[3].draw_btn = Draw_Color_Button ;
  button[3].btn_command = Command_Select_Color ;
  
  button[4].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*4;
  button[4].start_y = BUTTON_START_Y;
  button[4].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*5;
  button[4].end_y = BUTTON_START_Y+COLOR_BLOCK_HEIGHT;
  button[4].para = CL_RED;
  button[4].touch_flag = 0;  
  button[4].draw_btn = Draw_Color_Button ;
  button[4].btn_command = Command_Select_Color ;
  
  button[5].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*5;
  button[5].start_y = BUTTON_START_Y;
  button[5].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*6;
  button[5].end_y = BUTTON_START_Y+COLOR_BLOCK_HEIGHT;
  button[5].para = CL_MAGENTA;
  button[5].touch_flag = 0;  
  button[5].draw_btn = Draw_Color_Button ;
  button[5].btn_command = Command_Select_Color ;
  
  button[6].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*6;
  button[6].start_y = BUTTON_START_Y;
  button[6].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*7;
  button[6].end_y = BUTTON_START_Y+COLOR_BLOCK_HEIGHT;
  button[6].para = CL_GREEN;
  button[6].touch_flag = 0;  
  button[6].draw_btn = Draw_Color_Button ;
  button[6].btn_command = Command_Select_Color ;
  
  button[7].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*7;
  button[7].start_y = BUTTON_START_Y;
  button[7].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*8;
  button[7].end_y = BUTTON_START_Y+COLOR_BLOCK_HEIGHT;
  button[7].para = CL_CYAN;
  button[7].touch_flag = 0;  
  button[7].draw_btn = Draw_Color_Button ;
  button[7].btn_command = Command_Select_Color ;
  
  button[8].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*8;
  button[8].start_y = BUTTON_START_Y;
  button[8].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*9;
  button[8].end_y = LCD_Y_LENGTH;
  button[8].para = CL_BUTTON_GREY;
  button[8].touch_flag = 0;  
  button[8].draw_btn = Draw_Clear_Button ;
  button[8].btn_command = Command_Clear_Palette ;

  
  /*�ڶ��У���ҪΪ��ˢ��ť*/
  button[9].start_x = BUTTON_START_X;
  button[9].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[9].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*1 ;
  button[9].end_y = BUTTON_START_Y;
  button[9].para = LINE_SINGLE_PIXCEL;
  button[9].touch_flag = 0;  
  button[9].draw_btn = Draw_Shape_Button ;
  button[9].btn_command = Command_Select_Brush ;
  
  button[10].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*1;
  button[10].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[10].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*2 ;
  button[10].end_y = BUTTON_START_Y;
  button[10].para = LINE_2_PIXCEL;
  button[10].touch_flag = 0;  
  button[10].draw_btn = Draw_Shape_Button ;
  button[10].btn_command = Command_Select_Brush ;
  
  button[11].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*2;
  button[11].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[11].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*3 ;
  button[11].end_y = BUTTON_START_Y;
  button[11].para = LINE_4_PIXCEL;
  button[11].touch_flag = 0;  
  button[11].draw_btn = Draw_Shape_Button ;
  button[11].btn_command = Command_Select_Brush ;
  
  button[12].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*3;
  button[12].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[12].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*4 ;
  button[12].end_y = BUTTON_START_Y;
  button[12].para = LINE_6_PIXCEL;
  button[12].touch_flag = 0;  
  button[12].draw_btn = Draw_Shape_Button ;
  button[12].btn_command = Command_Select_Brush ;
  
  button[13].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*4;
  button[13].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[13].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*5 ;
  button[13].end_y = BUTTON_START_Y;
  button[13].para = LINE_8_PIXCEL;
  button[13].touch_flag = 0;  
  button[13].draw_btn = Draw_Shape_Button ;
  button[13].btn_command = Command_Select_Brush ;
  
  button[14].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*5;
  button[14].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[14].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*6 ;
  button[14].end_y = BUTTON_START_Y;
  button[14].para = LINE_16_PIXCEL;
  button[14].touch_flag = 0;  
  button[14].draw_btn = Draw_Shape_Button ;
  button[14].btn_command = Command_Select_Brush ;
  
  button[15].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*6;
  button[15].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[15].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*7 ;
  button[15].end_y = BUTTON_START_Y;
  button[15].para = LINE_20_PIXCEL;
  button[15].touch_flag = 0;  
  button[15].draw_btn = Draw_Shape_Button ;
  button[15].btn_command = Command_Select_Brush ;   
  
  button[16].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*7;
  button[16].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[16].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*8 ;
  button[16].end_y = BUTTON_START_Y;
  button[16].para = LINE_WITH_CIRCLE;
  button[16].touch_flag = 0;  
  button[16].draw_btn = Draw_Shape_Button ;
  button[16].btn_command = Command_Select_Brush ;
  
  button[17].start_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*8;
  button[17].start_y = BUTTON_START_Y-COLOR_BLOCK_HEIGHT;
  button[17].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH*9 ;
  button[17].end_y = BUTTON_START_Y;
  button[17].para = RUBBER;
  button[17].touch_flag = 0;  
  button[17].draw_btn = Draw_Shape_Button ;
  button[17].btn_command = Command_Select_Brush ;

}

/**
* @brief  Draw_Color_Button ��ɫ��ť����溯��
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Draw_Color_Button(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  
  /*�ͷŰ���*/
  if(ptr->touch_flag == 0)
  {
		/*����Ϊ���ܼ���Ӧ����ɫ*/
//		LCD_SetColors(ptr->para,CL_WHITE);
    NT35510_DrawRectangle(ptr->start_x,
                      ptr->start_y,
                      ptr->end_x - ptr->start_x,
											ptr->end_y - ptr->start_y, ptr->para, 1);
  }
  else  /*��������*/
  {
    /*��ɫ����*/
//		LCD_SetColors(CL_WHITE,CL_WHITE);
    NT35510_DrawRectangle(ptr->start_x,
                      ptr->start_y,
                      ptr->end_x - ptr->start_x,
											ptr->end_y - ptr->start_y, ptr->para, 1);
  } 
    /*��ť�߿�*/
//		LCD_SetColors(CL_BLUE4,CL_WHITE);
    NT35510_DrawRectangle(ptr->start_x,
									ptr->start_y,
									ptr->end_x - ptr->start_x,
									ptr->end_y - ptr->start_y,ptr->para, 0);
  
}

/**
* @brief  Draw_Clear_Button ������ť����溯��
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Draw_Clear_Button(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
    
  /*�ͷŰ���*/
  if(ptr->touch_flag == 0)
  {
	
//		LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    NT35510_DrawRectangle(
    				  ptr->start_x,
                      ptr->start_y,
                      ptr->end_x - ptr->start_x,
					  ptr->end_y - ptr->start_y,
					  CL_BUTTON_GREY, 1);
    

		
//		LCD_SetColors(CL_RED,CL_BUTTON_GREY);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���16*32�����壬
		*���������С��32*32�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
//		LCD_SetFont(&Font16x32);
//		
//		NT35510_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 32*2 )/2,
//														ptr->start_y+ ((ptr->end_y - ptr->start_y-32)/2),	
//														"����");
		App_LCD_Set_Point_Back_Color(CL_RED, CL_BUTTON_GREY);

		App_LCD_ShowString(ptr->start_x+5,ptr->start_y+(ptr->end_y-ptr->start_y)/2,200,16,16,"Clear");	
  }
  else  /*��������*/
  {
		
//		LCD_SetColors(CL_WHITE,CL_WHITE);
    NT35510_DrawRectangle(
    				ptr->start_x,
                    ptr->start_y,
                    ptr->end_x - ptr->start_x,
					ptr->end_y - ptr->start_y,
					CL_WHITE, 1);
    
		
//		LCD_SetColors(CL_RED,CL_WHITE);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���16*32�����壬
		*���������С��32*32�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
//		LCD_SetFont(&Font16x32);
//    NT35510_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 32*2 )/2,          
//																ptr->start_y+ ((ptr->end_y - ptr->start_y-32)/2),	
//																"����");

  	App_LCD_Set_Point_Back_Color(CL_RED, CL_WHITE);
  
	App_LCD_ShowString(ptr->start_x+5,ptr->start_y+(ptr->end_y-ptr->start_y)/2,200,16,16,"Clear");

  } 
  
     /*��ť�߿�*/
//		LCD_SetColors(CL_BLUE4,CL_WHITE);
    NT35510_DrawRectangle(
    					ptr->start_x,
						ptr->start_y,
						ptr->end_x - ptr->start_x,
						ptr->end_y - ptr->start_y,
						CL_BLUE4, 0);
  
}


/**
* @brief  Draw_Shape_Button ��ˢ��ť����溯��
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Draw_Shape_Button(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  
   uint16_t i;
  
    /* ������ɫ û����ʱΪ��ɫ������ʱΪ��ɫ*/                  
    if(ptr->touch_flag ==0 )
    {
			
//			LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
			NT35510_DrawRectangle(
					  ptr->start_x,
                      ptr->start_y,
                      ptr->end_x - ptr->start_x,
					  ptr->end_y - ptr->start_y,
					  CL_BUTTON_GREY, 1);
      

			 /*��ʾ����ʱ�ı�����ɫ*/     
//			LCD_SetColors(CL_BLUE4,CL_BUTTON_GREY);
			NT35510_DrawRectangle(ptr->start_x,
										ptr->start_y,
										ptr->end_x - ptr->start_x,
										ptr->end_y - ptr->start_y,
										CL_BLUE4, 0);

    }
    else
    {

//			LCD_SetColors(CL_WHITE,CL_WHITE);
			NT35510_DrawRectangle(
					  ptr->start_x,
                      ptr->start_y,
                      ptr->end_x - ptr->start_x,
					  ptr->end_y - ptr->start_y,
					  CL_WHITE, 1);
      

			 /*��ʾ����ʱ�ı�����ɫ*/     
//			LCD_SetColors(CL_BLUE4,CL_WHITE);
			NT35510_DrawRectangle(
					  ptr->start_x,
					  ptr->start_y,
					  ptr->end_x - ptr->start_x,
					  ptr->end_y - ptr->start_y,
					  CL_BLUE4, 0);
    }
  
//	LCD_SetColors(CL_BLACK,CL_WHITE);	
  /*���ݻ�ˢ��״��水ťͼ��*/
 switch(ptr->para)
  {
    		
    case LINE_SINGLE_PIXCEL:      
     		
//				LCD_SetColors(CL_BLACK,CL_WHITE);
        NT35510_DrawLine(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2, CL_BLACK);
      
      break;   

      
    case LINE_2_PIXCEL:
        
        NT35510_DrawLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          CL_BLACK, 1);
 
    
      break;
        
    case LINE_4_PIXCEL:
   
            NT35510_DrawLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          CL_BLACK, 2);
 
    
      break;
    
        case LINE_6_PIXCEL:
        
        NT35510_DrawLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          CL_BLACK, 3);
 
    
      break;
        
     case LINE_8_PIXCEL:
   
            NT35510_DrawLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          CL_BLACK, 4);
 
    
      break;
    
       case LINE_16_PIXCEL:
   
            NT35510_DrawLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          CL_BLACK, 8 );
 
    
      break;
        
       case LINE_20_PIXCEL:
   
            NT35510_DrawLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          CL_BLACK, 10);
 
    
      break;
            
            
    
     case LINE_WITH_CIRCLE: 
//        LCD_SetColors(CL_BLACK,CL_WHITE);      
        NT35510_DrawLine(ptr->start_x+5,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          ptr->end_x-5,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2, CL_BLACK);
		 
        
        for(i=0;i<((ptr->end_x - ptr->start_x-10)/10);i++)
        {
        
        NT35510_DrawCircle(ptr->start_x+5+i*10,
                            ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                            3,CL_BLACK, 1);
        

        }

      
    break;
      
      
    case RUBBER:
//			LCD_SetColors(CL_WHITE,CL_BLACK);
       NT35510_DrawRectangle( 
       					ptr->start_x+((ptr->end_x - ptr->start_x -40)/2),
                        ptr->start_y+ ((ptr->end_y - ptr->start_y-40)/2),
                        40,
                        40 ,
                        CL_WHITE, 1);   
      
  
//			LCD_SetColors(CL_RED,CL_BUTTON_GREY);    

		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���16*32�����壬
		*���������С��32*32�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
//		LCD_SetFont(&Font16x32);
//    NT35510_DispString_EN_CH( ptr->start_x+(ptr->end_x - ptr->start_x -32*2)/2,
//																ptr->start_y+ ((ptr->end_y - ptr->start_y-32 -16)/2)+32,		
//																"��Ƥ");
//	App_LCD_Set_Point_Back_Color(CL_RED, CL_WHITE);
//  
//	App_LCD_ShowString(ptr->start_x+5,ptr->start_y+(ptr->end_y-ptr->start_y)/2,200,16,16,"Xp");

    break;

  }
  
}

/**
* @brief  Command_Select_Brush �л���ˢ��ɫ����ˢ�����Ĺ���ִ�к���
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Command_Select_Brush(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  brush.shape =(SHAPE) ptr->para;
  if(ptr->para == RUBBER)
  	brush.color = CL_WHITE;
//	LCD_SetColors(brush.color,CL_WHITE);
}

/**
* @brief  Command_Select_Brush �л���ˢ��ɫ�����������Ĺ���ִ�к���
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Command_Clear_Palette(void *btn)
{
//		LCD_SetColors(CL_WHITE,CL_WHITE);
    NT35510_DrawRectangle(
    				  PALETTE_START_X,
                      PALETTE_START_Y,                    
                      PALETTE_END_X-(PALETTE_START_X+1), 
					  PALETTE_END_Y-PALETTE_START_Y ,
					  CL_WHITE, 1);
}


/**
* @brief  Command_Select_Color �л���ˢ��ɫ����ɫ�����Ĺ���ִ�к���
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Command_Select_Color(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  
  brush.color = ptr->para;	
//	LCD_SetColors(brush.color,CL_WHITE);
  
  if(brush.shape == RUBBER)
  {
    brush.shape = LINE_SINGLE_PIXCEL;
  }

}

/**
 * @brief  �� NT35510 ��ʾ���ϻ�һ������
 * @param  usX_Start �����ض�ɨ�跽���¾��ε���ʼ��X����
 * @param  usY_Start �����ض�ɨ�跽���¾��ε���ʼ��Y����
 * @param  usWidth�����εĿ�ȣ���λ�����أ�
 * @param  usHeight�����εĸ߶ȣ���λ�����أ�
 * @param  ucFilled ��ѡ���Ƿ����þ���
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0 :���ľ���
  *     @arg 1 :ʵ�ľ��� 
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint16_t color, uint8_t ucFilled )
{
	if ( ucFilled )
	{
//		NT35510_OpenWindow ( usX_Start, usY_Start, usWidth, usHeight );
//		NT35510_FillColor ( usWidth * usHeight ,CurrentTextColor);	
		App_LCD_Fill(usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1, color);
	}
	else
	{
//		NT35510_DrawLine ( usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start );
//		NT35510_DrawLine ( usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );
//		NT35510_DrawLine ( usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1 );
//		NT35510_DrawLine ( usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );	
		App_LCD_DrawRectangle(usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1, color);
	}

}

/**
 * @brief  �� NT35510 ��ʾ����ʹ�� Bresenham �㷨���߶� 
 * @param  usX1 �����ض�ɨ�跽�����߶ε�һ���˵�X����
 * @param  usY1 �����ض�ɨ�跽�����߶ε�һ���˵�Y����
 * @param  usX2 �����ض�ɨ�跽�����߶ε���һ���˵�X����
 * @param  usY2 �����ض�ɨ�跽�����߶ε���һ���˵�Y����
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 ,uint16_t color)
{
	App_LCD_DrawLine(usX1, usY1, usX2, usY2, color);
}

#define ABS(X)  ((X) > 0 ? (X) : -(X))

/**
  * @brief  ������֮�����켣
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void NT35510_DrawLineCircle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color,uint8_t thick )
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
	
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
		
		//�жϱ߽�
		if(x+thick>LCD_X_LENGTH || x-thick<0 || //Һ�����ұ߽�
			y+thick>LCD_Y_LENGTH || y-thick<0  ) //Һ�����±߽�
			continue;

    NT35510_DrawRectangle(x,y,thick<<1,thick<<1,color,1);
//    NT35510_DrawCircle(x,y,thick,1);             /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}


/**
 * @brief  �� NT35510 ��ʾ����ʹ�� Bresenham �㷨��Բ
 * @param  usX_Center �����ض�ɨ�跽����Բ�ĵ�X����
 * @param  usY_Center �����ض�ɨ�跽����Բ�ĵ�Y����
 * @param  usRadius��Բ�İ뾶����λ�����أ�
 * @param  ucFilled ��ѡ���Ƿ�����Բ
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0 :����Բ
  *     @arg 1 :ʵ��Բ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DrawCircle ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint16_t color, uint8_t ucFilled )
{
	int16_t sCurrentX, sCurrentY;
	int16_t sError;
	
	
	sCurrentX = 0; sCurrentY = usRadius;	  
	
	sError = 3 - ( usRadius << 1 );     //�ж��¸���λ�õı�־
	
	
	while ( sCurrentX <= sCurrentY )
	{
		int16_t sCountY;
		
		
		if ( ucFilled ) 			
			for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
			{                      
				App_LCD_DrawPoint ( usX_Center + sCurrentX, usY_Center + sCountY, color);           //1���о����� 
				App_LCD_DrawPoint ( usX_Center - sCurrentX, usY_Center + sCountY, color);           //2       
				App_LCD_DrawPoint ( usX_Center - sCountY,   usY_Center + sCurrentX, color);           //3
				App_LCD_DrawPoint ( usX_Center - sCountY,   usY_Center - sCurrentX, color);           //4
				App_LCD_DrawPoint ( usX_Center - sCurrentX, usY_Center - sCountY, color);           //5    
        		App_LCD_DrawPoint ( usX_Center + sCurrentX, usY_Center - sCountY, color);           //6
				App_LCD_DrawPoint ( usX_Center + sCountY,   usY_Center - sCurrentX, color);           //7 	
       		 	App_LCD_DrawPoint ( usX_Center + sCountY,   usY_Center + sCurrentX, color);           //0				
			}
		
		else
		{          
			App_LCD_DrawPoint ( usX_Center + sCurrentX, usY_Center + sCurrentY, color);             //1���о�����
			App_LCD_DrawPoint ( usX_Center - sCurrentX, usY_Center + sCurrentY, color);             //2      
			App_LCD_DrawPoint ( usX_Center - sCurrentY, usY_Center + sCurrentX, color);             //3
			App_LCD_DrawPoint ( usX_Center - sCurrentY, usY_Center - sCurrentX, color);             //4
			App_LCD_DrawPoint ( usX_Center - sCurrentX, usY_Center - sCurrentY, color);             //5       
			App_LCD_DrawPoint ( usX_Center + sCurrentX, usY_Center - sCurrentY, color);             //6
			App_LCD_DrawPoint ( usX_Center + sCurrentY, usY_Center - sCurrentX, color);             //7 
			App_LCD_DrawPoint ( usX_Center + sCurrentY, usY_Center + sCurrentX, color);             //0
    }			
		
		
		sCurrentX ++;

		
		if ( sError < 0 ) 
			sError += 4 * sCurrentX + 6;	  
		
		else
		{
			sError += 10 + 4 * ( sCurrentX - sCurrentY );   
			sCurrentY --;
		} 	
		
		
	}
	
	
}


