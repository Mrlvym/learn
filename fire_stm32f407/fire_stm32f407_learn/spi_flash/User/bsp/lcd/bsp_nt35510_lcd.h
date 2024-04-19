#ifndef __BSP_NT35510_LCD_H__
#define __BSP_NT35510_LCD_H__
#include "stm32f4xx.h"

#define BSP_LCD_BL_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define BSP_LCD_GPIO_CLK		(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG)
#define BSP_LCD_TIMING_CLK		RCC_AHB3Periph_FSMC

#define BSP_LCD_CMD		*((uint16_t *)0x6C00007E)	//BSP_LCD_RS --> FSMC_A6 --> 0
#define BSP_LCD_DATA	*((uint16_t *)0x6C000080)	//BSP_LCD_RS --> FSMC_A6 --> 1

#define	BSP_LCD_BL_GPIO		GPIOB
#define	BSP_LCD_RS_GPIO		GPIOF
#define BSP_LCD_CS_GPIO		GPIOG
#define	BSP_LCD_WRD_GPIO	GPIOD

#define BSP_LCD_D_D			GPIOD
#define BSP_LCD_D_E			GPIOE


//#define BSP_LCD_RST	
#define	BSP_LCD_BL	GPIO_Pin_15	//PB15

#define	BSP_LCD_RS	GPIO_Pin_12	//FSMC_A6-->PF12

#define BSP_LCD_CS	GPIO_Pin_12	//FSMC_NE4-->PG12
#define BSP_LCD_WR	GPIO_Pin_5	//FSMC_NWE-->PD5
#define BSP_LCD_RD	GPIO_Pin_4	//FSMC_NOE-->PD4

#define	BSP_LCD_D0	GPIO_Pin_14	//PD14
#define	BSP_LCD_D1	GPIO_Pin_15	//PD15
#define	BSP_LCD_D2	GPIO_Pin_0	//PD0
#define	BSP_LCD_D3	GPIO_Pin_1	//PD1

#define	BSP_LCD_D4	GPIO_Pin_7	//PE7
#define	BSP_LCD_D5	GPIO_Pin_8	//PE8
#define	BSP_LCD_D6	GPIO_Pin_9	//PE9
#define	BSP_LCD_D7	GPIO_Pin_10	//PE10
#define	BSP_LCD_D8	GPIO_Pin_11	//PE11
#define	BSP_LCD_D9	GPIO_Pin_12	//PE12
#define	BSP_LCD_D10	GPIO_Pin_13	//PE13
#define	BSP_LCD_D11	GPIO_Pin_14	//PE14
#define	BSP_LCD_D12	GPIO_Pin_15	//PE15

#define	BSP_LCD_D13	GPIO_Pin_8	//PD8
#define	BSP_LCD_D14	GPIO_Pin_9	//PD9
#define	BSP_LCD_D15	GPIO_Pin_10	//PD10

#define	BSP_LCD_RS_AF	GPIO_PinSource12	//FSMC_A6-->PF12

#define BSP_LCD_CS_AF	GPIO_PinSource12	//FSMC_NE4-->PG12
#define BSP_LCD_WR_AF	GPIO_PinSource5		//FSMC_NWE-->PD5
#define BSP_LCD_RD_AF	GPIO_PinSource4		//FSMC_NOE-->PD4

#define	BSP_LCD_D0_AF	GPIO_PinSource14	//PD14
#define	BSP_LCD_D1_AF	GPIO_PinSource15	//PD15
#define	BSP_LCD_D2_AF	GPIO_PinSource0		//PD0
#define	BSP_LCD_D3_AF	GPIO_PinSource1		//PD1

#define	BSP_LCD_D4_AF	GPIO_PinSource7		//PE7
#define	BSP_LCD_D5_AF	GPIO_PinSource8		//PE8
#define	BSP_LCD_D6_AF	GPIO_PinSource9		//PE9
#define	BSP_LCD_D7_AF	GPIO_PinSource10	//PE10
#define	BSP_LCD_D8_AF	GPIO_PinSource11	//PE11
#define	BSP_LCD_D9_AF	GPIO_PinSource12	//PE12
#define	BSP_LCD_D10_AF	GPIO_PinSource13	//PE13
#define	BSP_LCD_D11_AF	GPIO_PinSource14	//PE14
#define	BSP_LCD_D12_AF	GPIO_PinSource15	//PE15

#define	BSP_LCD_D13_AF	GPIO_PinSource8		//PD8
#define	BSP_LCD_D14_AF	GPIO_PinSource9		//PD9
#define	BSP_LCD_D15_AF	GPIO_PinSource10	//PD10


void BSP_LCD_BL_Init(void);

void BSP_LCD_BL_On(void);
void BSP_LCD_BL_Off(void);

void BSP_LCD_8080_Gpio_Init(void);
void BSP_LCD_8080_Timing_Init(void);

void BSP_LCD_WR_Cmd(uint16_t cmd);
void BSP_LCD_WR_Data(uint16_t w_data);

uint16_t BSP_LCD_RD_Data(void);


#endif

