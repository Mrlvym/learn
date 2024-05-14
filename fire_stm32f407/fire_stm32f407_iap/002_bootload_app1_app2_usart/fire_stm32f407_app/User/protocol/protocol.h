#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <stdint.h>

#define Max_SerialBuf_Len		256

typedef enum
{
	PROTOCOL_TYPE_UART	=0x01,
}protocol_type_e;




//串口接收缓冲区结构体
typedef struct
{
	uint8_t 		message_buf[Max_SerialBuf_Len];
	uint8_t 		message_len;
	protocol_type_e	protocol_type;
	uint8_t 		msg_recv_done_flag;
	
}uart_recv_handle_t;


void protocol_init(void);
uint8_t mh_serial_messagehandle(void);
void uart_msg_handle(uint8_t *buf, uint32_t len);
uint8_t check_sum( uint8_t *buf, int buf_len );


#endif
