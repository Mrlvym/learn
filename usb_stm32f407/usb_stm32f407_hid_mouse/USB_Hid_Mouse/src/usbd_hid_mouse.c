#include "usbd_hid_mouse.h"

static uint8_t s_key_sta = KEY_NUL;
static uint8_t s_key_click_sta = KEY_NUL;


uint8_t Get_Key_Sta(void)
{
	return s_key_sta;
}

void Set_Key_Sta(uint8_t key_sta)
{
	s_key_sta = key_sta;
}

uint8_t Get_Key_Click_Sta(void)
{
	return s_key_click_sta;
}

void Set_Key_Click_Sta(uint8_t key_sta)
{
	s_key_click_sta = key_sta;
}


uint8_t *USBD_HID_Mouse_GetPos(void)
{
	static uint8_t HID_Mouse_Buffer [4];

	int8_t click = 0, x = 0, y = 0, roll = 0;
	memset(HID_Mouse_Buffer, 0, 4);

	switch(Get_Key_Click_Sta())
	{
		case KEY_CLICK_LEFT:
			click |= BIT0;
			break;
		case KEY_CLICK_RIGHT:
			click |= BIT1;
			break;
		case KEY_CLICK_MIDDLE:
			click |= BIT2;
			break;
		case KEY_CLICK_ROLL:
			roll += MOUSE_ROLL_STEP;
			break;
		case KEY_NUL:
		default:
			click = 0;
			roll = 0;
			break;
	}
	
	switch(Get_Key_Sta())
	{
		case KEY_UP:
			y -= MOUSE_STEP;
			click |= BIT5;
			break;
		case KEY_DOWN:
			y += MOUSE_STEP;
			break;
		case KEY_LEFT:
			x -= MOUSE_STEP;
			click |= BIT4;
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
	HID_Mouse_Buffer[0] = click;
	HID_Mouse_Buffer[1] = x;
	HID_Mouse_Buffer[2] = y;
	HID_Mouse_Buffer[3] = roll;

	return HID_Mouse_Buffer;
}

