#ifndef __HAL_LED_H__
#define __HAL_LED_H__
#include "stdint.h"

#define LED0 	0
#define LED1	1
#define LED_ALL	2

void Hal_Led_Init(void);

void Hal_Led0_on(void);
void Hal_Led0_off(void);
void Hal_Led1_on(void);
void Hal_Led1_off(void);

void Hal_Led_on(uint8_t LED);
void Hal_Led_off(uint8_t LED);
void Hal_Led_turn(uint8_t LED);

#endif


