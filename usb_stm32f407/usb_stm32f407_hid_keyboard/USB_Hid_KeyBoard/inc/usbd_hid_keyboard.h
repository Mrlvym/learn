#ifndef _USBD_HID_KEYBOARD_H__
#define _USBD_HID_KEYBOARD_H__
#include <stdint.h>

#define MOUSE_STEP	5

typedef enum{
	KEY_NUL = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT
}key_ste_e;

typedef enum{
	KEY_A = 0x04,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z
}keyboard_e;


uint8_t Get_Key_Sta(void);

void Set_Key_Sta(uint8_t key_sta);

uint8_t *USBD_HID_Mouse_GetPos(void);

uint8_t *USBD_HID_KeyBoard_GetPos(void);


#endif


