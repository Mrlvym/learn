#ifndef _USBD_HID_MOUSE_H__
#define _USBD_HID_MOUSE_H__
#include <stdint.h>

#define MOUSE_STEP	5
#define MOUSE_ROLL_STEP	5

#define BIT0	(0x1 << 0)
#define BIT1	(0x1 << 1)
#define BIT2	(0x1 << 2)
#define BIT3	(0x1 << 3)
#define BIT4	(0x1 << 4)
#define BIT5	(0x1 << 5)
#define BIT6	(0x1 << 6)
#define BIT7	(0x1 << 7)


typedef enum{
	KEY_NUL = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_CLICK_LEFT,
	KEY_CLICK_RIGHT,
	KEY_CLICK_MIDDLE,
	KEY_CLICK_ROLL
}key_ste_e;


uint8_t Get_Key_Sta(void);

void Set_Key_Sta(uint8_t key_sta);

uint8_t Get_Key_Click_Sta(void);

void Set_Key_Click_Sta(uint8_t key_sta);

uint8_t *USBD_HID_Mouse_GetPos(void);



#endif


