#include <stdio.h>
#include <string.h>


#include "protocol.h"
#include "ringbuffer.h"


static uint8_t tmp_buf[Max_SerialBuf_Len] = {0}; 


//���崮�ڶ�ȡ�������ṹ��
static uart_recv_handle_t uart_recv_handle;

//�����ȡ���λ�����
RingBuffer read_rb;

void protocol_init(void)
{
	memset(&uart_recv_handle, 0, sizeof(uart_recv_handle_t));
	rb_new(&read_rb, Max_SerialBuf_Len);
}
	

//�Ӵ���ץȡһ�����ݺ����ӿ�
static uint8_t pro_get_frame(void)
{
	static int count = 0;
	static uint8_t curValue = 0;
	static uint8_t lastValue = 0;
	static uint16_t data_len = 0;
	
	//�ж��Ƿ��ڶ�ȡ��״̬
	if(0 == uart_recv_handle.msg_recv_done_flag)
	{
		//�жϻ��λ������Ƿ������ݿɶ�
		if(rb_can_read(&read_rb) > 0)
		{
			//�ӻ��λ�������ȡһ������
			rb_read(&read_rb, &curValue, 1);
			if((lastValue == 0xEF) && (curValue == 0xAA))//�жϰ�ͷ
			{
				tmp_buf[0] = 0xEF;
				tmp_buf[1] = 0xAA;
				count = 2;
				uart_recv_handle.protocol_type = PROTOCOL_TYPE_UART;
				return 1;
			}
			tmp_buf[count] = curValue;
			count++;
			
			if(PROTOCOL_TYPE_UART == uart_recv_handle.protocol_type)
			{
				if(5 == count)//�ж��Ƿ����data����
				{
					data_len = (tmp_buf[3] << 8) | tmp_buf[4];
				}
				if(data_len + 5 + 1 == count)//�ж��Ƿ����һ������
				{
					memcpy(uart_recv_handle.message_buf, tmp_buf, count);
					memset(tmp_buf, 0, Max_SerialBuf_Len);
					uart_recv_handle.message_len = count;
					
					uart_recv_handle.msg_recv_done_flag = 1;
					
					lastValue = curValue = 0;
					count = 0;	

					return 0;
				}
			}
			
			lastValue = curValue;
		}
	}
	
	return 0;
}


void App_Iap_Deal_Usart_Buf(uint8_t *buf, uint8_t len);

//���ڽ������ݺ����ӿ�
uint8_t mh_serial_messagehandle(void)
{
	uint8_t sum = 0;
	//��ȡһ������
	pro_get_frame();
	
	if((uart_recv_handle.msg_recv_done_flag) && (PROTOCOL_TYPE_UART == uart_recv_handle.protocol_type))
	{
		//����У���
		sum = check_sum(uart_recv_handle.message_buf, uart_recv_handle.message_len);
		if(sum != uart_recv_handle.message_buf[uart_recv_handle.message_len-1])
		{
			memset(&uart_recv_handle, 0, sizeof(uart_recv_handle_t));	
			return 1;
		}
		
		//��������
		App_Iap_Deal_Usart_Buf(uart_recv_handle.message_buf, uart_recv_handle.message_len);
	
		memset(&uart_recv_handle, 0, sizeof(uart_recv_handle_t));		
	}
	
	return 0;
}


/*******************************************************************************
* Function Name  : check_sum
* Description    : 
* Input          : buf:; buf_len:;
* Output         : None
* Return         :
* Attention		 : None
*******************************************************************************/

uint8_t check_sum( uint8_t *buf, int buf_len)
{
    int			i = 0;
    uint8_t		sum = 0;

	if(buf == 0 || buf_len <= 0){
		return 0;
	} 
	
    for(i=2; i<buf_len-1; i++)
        sum += buf[i];
	
    return sum;
}














