#ifndef __BSP_RELOCATE_H__
#define __BSP_RELOCATE_H__

void Bsp_Relocate(char *src, char *dest, unsigned int len);

void Bsp_Clear_Bss(char *buf, unsigned int len);

#endif
