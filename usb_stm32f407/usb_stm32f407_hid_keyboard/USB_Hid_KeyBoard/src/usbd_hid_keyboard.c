#include "usbd_hid_keyboard.h"

static uint8_t s_key_sta = KEY_NUL;

uint8_t Get_Key_Sta(void)
{
	return s_key_sta;
}

void Set_Key_Sta(uint8_t key_sta)
{
	s_key_sta = key_sta;
}

uint8_t *USBD_HID_Mouse_GetPos(void)
{
	static uint8_t HID_Mouse_Buffer [4];

	int8_t x = 0, y = 0;
	memset(HID_Mouse_Buffer, 0, 4);
	
	switch(Get_Key_Sta())
	{
		case KEY_UP:
			y -= MOUSE_STEP;
			break;
		case KEY_DOWN:
			y += MOUSE_STEP;
			break;
		case KEY_LEFT:
			x -= MOUSE_STEP;
			break;
		case KEY_RIGHT:
			x += MOUSE_STEP;
			break;
		case KEY_NUL:
		default:
			x = 0;
			y = 0;
			break;		
	}

//	Set_Key_Sta(KEY_NUL);
	
	HID_Mouse_Buffer[1] = x;
	HID_Mouse_Buffer[2] = y;

	return HID_Mouse_Buffer;
}

uint8_t *USBD_HID_KeyBoard_GetPos(void)
{
	static uint8_t HID_KeyBoard_Buffer [8];
	
	memset(HID_KeyBoard_Buffer, 0, 8);
	
	switch(Get_Key_Sta())
	{
		case KEY_UP:
			HID_KeyBoard_Buffer[0] = 0x02;//shift		
			HID_KeyBoard_Buffer[2] = KEY_A;
			HID_KeyBoard_Buffer[3] = KEY_B;
			HID_KeyBoard_Buffer[4] = KEY_C;
			break;
		case KEY_DOWN:
			HID_KeyBoard_Buffer[3] = KEY_B;
			HID_KeyBoard_Buffer[4] = KEY_C;
			break;
		case KEY_LEFT:
			HID_KeyBoard_Buffer[4] = KEY_C;
			break;
		case KEY_RIGHT:
			HID_KeyBoard_Buffer[5] = KEY_D;
			break;
		case KEY_NUL:
		default:

			break;		
	}

//	Set_Key_Sta(KEY_NUL);

	return HID_KeyBoard_Buffer;
}


