#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include  "usbd_hid_core.h"
#include  "usbd_hid_keyboard.h"


//ALIENTEK ̽����STM32F407������ ʵ��4
//����ͨ��ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//�������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;


int main(void)
{ 
	uint8_t *buf;
	
//	u8 t;
//	u8 len;	
//	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	EXTIX_Init();
	
	USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif  
            &USR_desc, 
            &USBD_HID_cb, 
            &USR_cb);
	
	while(1)
	{
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\n�����͵���ϢΪ:\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);         //�򴮿�1��������
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
//			printf("\r\n\r\n");//���뻻��
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				printf("\r\nALIENTEK ̽����STM32F407������ ����ʵ��\r\n");
//				printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
//			}
//			if(times%200==0)printf("����������,�Իس�������\r\n");  
//			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
//			delay_ms(10);   
//		}
		buf = USBD_HID_KeyBoard_GetPos();
//		if(buf[0] != 0 || buf[1] != 0 || buf[2] != 0 || buf[3] != 0 || buf[4] != 0 || buf[5] != 0 || buf[6] != 0 || buf[7] != 0)
//		{
//			USBD_HID_SendReport(&USB_OTG_dev, buf, 8);			
//		}
//
//		delay_ms(15);

		while(USBD_HID_SendReport(&USB_OTG_dev, buf, 8));

//		LED0=!LED0;
//		delay_ms(1000);
	}
}
