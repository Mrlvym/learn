#include "bsp_relocate.h"

void Bsp_Relocate(char *src, char *dest, unsigned int len)
{
		while(len--)
		{
			*dest++ = *src++;
		}
}



void Bsp_Clear_Bss(char *buf, unsigned int len)
{
		while(len--)
		{
				*buf++ = 0;
		}
}

