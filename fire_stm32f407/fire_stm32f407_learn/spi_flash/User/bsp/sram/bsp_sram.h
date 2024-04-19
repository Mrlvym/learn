#ifndef __BSP_SRAM_H__
#define __BSP_SRAM_H__
#include "stdint.h"
#include "stm32f4xx.h"


#define Bank1_SRAM3_ADDR	0x68000000
#define Bank1_SRAM3_SIZE	0x100000	//1M


#define FSMC_CLK		RCC_AHB3Periph_FSMC

#define FSMC_A_GPIO_CLK	RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG
#define FSMC_D_GPIO_CLK	RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE

#define FSMC_NBLx_GPIO_CLK	RCC_AHB1Periph_GPIOE
#define FSMC_NOWE_GPIO_CLK	RCC_AHB1Periph_GPIOD
#define FSMC_NE3_GPIO_CLK	RCC_AHB1Periph_GPIOG

#define FSMC_A_GPIO_F	GPIOF
#define FSMC_A_GPIO_G	GPIOG
#define FSMC_A_GPIO_D	GPIOD

#define FSMC_D_GPIO_D	GPIOD
#define FSMC_D_GPIO_E	GPIOE

#define FSMC_NBLx_GPIO_E	GPIOE
#define FSMC_NOWE_GPIO_D	GPIOD
#define FSMC_NE3_GPIO_G		GPIOG

//gpio

//GPIO F
#define FSMC_A0		GPIO_Pin_0
#define FSMC_A1		GPIO_Pin_1
#define FSMC_A2		GPIO_Pin_2
#define FSMC_A3		GPIO_Pin_3
#define FSMC_A4		GPIO_Pin_4
#define FSMC_A5		GPIO_Pin_5

#define FSMC_A6		GPIO_Pin_12
#define FSMC_A7		GPIO_Pin_13
#define FSMC_A8		GPIO_Pin_14
#define FSMC_A9		GPIO_Pin_15

//GPIO G
#define FSMC_A10	GPIO_Pin_0
#define FSMC_A11	GPIO_Pin_1
#define FSMC_A12	GPIO_Pin_2
#define FSMC_A13	GPIO_Pin_3
#define FSMC_A14	GPIO_Pin_4
#define FSMC_A15	GPIO_Pin_5

//GPIO D
#define	FSMC_A16	GPIO_Pin_11
#define	FSMC_A17	GPIO_Pin_12
#define	FSMC_A18	GPIO_Pin_13


//GPIO D
#define	FSMC_D0		GPIO_Pin_14
#define	FSMC_D1		GPIO_Pin_15
#define	FSMC_D2		GPIO_Pin_0
#define	FSMC_D3		GPIO_Pin_1

//GPIO E
#define	FSMC_D4		GPIO_Pin_7
#define	FSMC_D5		GPIO_Pin_8
#define	FSMC_D6		GPIO_Pin_9
#define	FSMC_D7		GPIO_Pin_10
#define	FSMC_D8		GPIO_Pin_11
#define	FSMC_D9		GPIO_Pin_12
#define	FSMC_D10	GPIO_Pin_13
#define	FSMC_D11	GPIO_Pin_14
#define	FSMC_D12	GPIO_Pin_15

//GPIO D
#define	FSMC_D13	GPIO_Pin_8
#define	FSMC_D14	GPIO_Pin_9
#define	FSMC_D15	GPIO_Pin_10

//GPIO E
#define FSMC_NBL0	GPIO_Pin_0
#define FSMC_NBL1	GPIO_Pin_1

//GPIO D
#define FSMC_NOE	GPIO_Pin_4
#define	FSMC_NWE	GPIO_Pin_5

//GPIO G
#define	FSMC_NE3	GPIO_Pin_10

//gpio AF

//GPIO F
#define FSMC_AF_A0 		GPIO_PinSource0
#define FSMC_AF_A1		GPIO_PinSource1
#define FSMC_AF_A2		GPIO_PinSource2
#define FSMC_AF_A3		GPIO_PinSource3
#define FSMC_AF_A4		GPIO_PinSource4
#define FSMC_AF_A5		GPIO_PinSource5

#define FSMC_AF_A6		GPIO_PinSource12
#define FSMC_AF_A7		GPIO_PinSource13
#define FSMC_AF_A8		GPIO_PinSource14
#define FSMC_AF_A9		GPIO_PinSource15

//GPIO G
#define FSMC_AF_A10		GPIO_PinSource0
#define FSMC_AF_A11		GPIO_PinSource1
#define FSMC_AF_A12		GPIO_PinSource2
#define FSMC_AF_A13		GPIO_PinSource3
#define FSMC_AF_A14		GPIO_PinSource4
#define FSMC_AF_A15		GPIO_PinSource5

//GPIO D
#define	FSMC_AF_A16		GPIO_PinSource11
#define	FSMC_AF_A17		GPIO_PinSource12
#define	FSMC_AF_A18		GPIO_PinSource13


//GPIO D
#define	FSMC_AF_D0		GPIO_PinSource14
#define	FSMC_AF_D1		GPIO_PinSource15
#define	FSMC_AF_D2		GPIO_PinSource0
#define	FSMC_AF_D3		GPIO_PinSource1

//GPIO E
#define	FSMC_AF_D4		GPIO_PinSource7
#define	FSMC_AF_D5		GPIO_PinSource8
#define	FSMC_AF_D6		GPIO_PinSource9
#define	FSMC_AF_D7		GPIO_PinSource10
#define	FSMC_AF_D8		GPIO_PinSource11
#define	FSMC_AF_D9		GPIO_PinSource12
#define	FSMC_AF_D10		GPIO_PinSource13
#define	FSMC_AF_D11		GPIO_PinSource14
#define	FSMC_AF_D12		GPIO_PinSource15

//GPIO D
#define	FSMC_AF_D13		GPIO_PinSource8
#define	FSMC_AF_D14		GPIO_PinSource9
#define	FSMC_AF_D15		GPIO_PinSource10

//GPIO E
#define FSMC_AF_NBL0	GPIO_PinSource0
#define FSMC_AF_NBL1	GPIO_PinSource1

//GPIO D
#define FSMC_AF_NOE		GPIO_PinSource4
#define	FSMC_AF_NWE		GPIO_PinSource5

//GPIO G
#define	FSMC_AF_NE3		GPIO_PinSource10


void BSP_Sram_GPIO_Init(void);
void BSP_Sram_Timing_Init(void);

void BSP_Sram_ReadByte(uint32_t addr_byte, uint8_t *data_byte, uint32_t len);
void BSP_Sram_WriteByte(uint32_t addr_byte, uint8_t *data_byte, uint32_t len);

void BSP_Sram_ReadShort(uint32_t addr_short, uint16_t *data_short, uint32_t len);
void BSP_Sram_WriteShort(uint32_t addr_short, uint16_t *data_short, uint32_t len);

void BSP_Sram_ReadWord(uint32_t addr_word, uint32_t *data_word, uint32_t len);
void BSP_Sram_WriteWord(uint32_t addr_word, uint32_t *data_word, uint32_t len);


#endif


