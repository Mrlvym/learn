/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

#include "hal_debug_usart.h"
#include "hal_delay.h"
#include "hal_led.h"
#include "app_rtc.h"
#include "hal_iwwdg.h"
#include "bsp_sdio.h"

//wwdg提前唤醒中断
void WWDG_IRQHandler(void)
{
//	Hal_Debug(INFO_LEVEL, "产生wwdg提前唤醒中断 ... \r\n");
	Hal_Led_on(LED0);
	WWDG_ClearFlag();
}

extern void App_Touch_Gt911_Int_CallBack(void);

//gt911 int
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_SET)
		{		
			App_Touch_Gt911_Int_CallBack();
		}
	}

	EXTI_ClearITPendingBit(EXTI_Line1);
}


//key0
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		Hal_Delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
		{	
			//设置时间或闹钟设置标志位
//			App_Set_DataTimeAlarm_Set_Flag(APP_SET_DataTimeAlarm_FLAG_SET);

			//iwdg喂狗
			Hal_IWDG_Feed();
		}
	}

	EXTI_ClearITPendingBit(EXTI_Line4);
}


//RTC-Alarlm
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		Hal_Led_on(LED0);

		RTC_ClearFlag(RTC_FLAG_ALRAF);
		
		EXTI_ClearITPendingBit(EXTI_Line17);					
	}
	else if(RTC_GetITStatus(RTC_IT_ALRB) != RESET)
	{
		Hal_Led_on(LED1);

		RTC_ClearFlag(RTC_FLAG_ALRBF);
		
		EXTI_ClearITPendingBit(EXTI_Line17);					
	}	 

}


void SDIO_IRQHandler(void)
{
	SD_ProcessIRQSrc();
}

void SD_SDIO_DMA_IRQHANDLER(void)
{
	SD_ProcessDMAIRQ();
}

uint8_t  Buffer_Rx_IIC1[40];//接收缓存
uint8_t  Rx_Idx_IIC1 = 0;//接收计数
uint8_t  Flag_RcvOK_IIC1 = 0;// 接收完成标志 
uint8_t  Tx_Idx_IIC1 = 0;//发送计数
u8 Response_Message[40];//发送缓存


void I2C1_EV_IRQHandler(void)
{
	uint32_t Last_Event;

	Last_Event = I2C_GetLastEvent(I2C1);

	if(Last_Event & 0x00010000 != 0x00010000)//slave mode
	{
		switch(Last_Event)
		{
			//send EV1 -> EV3_1 -> EV3 -> EV3_2
			case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED://EV1
				Tx_Idx_IIC1 = 0;
				break;
			case I2C_EVENT_SLAVE_BYTE_TRANSMITTED://EV3_1
			case I2C_EVENT_SLAVE_BYTE_TRANSMITTING://EV3
				I2C_SendData(I2C1, Response_Message[Tx_Idx_IIC1++]);
				break;
			case I2C_EVENT_SLAVE_ACK_FAILURE://EV3_2
				I2C_ClearFlag(I2C1, I2C_FLAG_AF);
				break;
				
			//recve	EV1 -> EV2 -> EV4
			case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED://EV1
				Rx_Idx_IIC1 = 0;
				break;
			
			case I2C_EVENT_SLAVE_BYTE_RECEIVED://EV2
				Buffer_Rx_IIC1[Rx_Idx_IIC1++] = I2C_ReceiveData(I2C1);
				break;

			case I2C_EVENT_SLAVE_STOP_DETECTED://EV4
				I2C_GenerateSTOP(I2C1, ENABLE);//release sda scl bus
				Flag_RcvOK_IIC1 = 1;
				break;
		}
	}
}

void I2C1_ER_IRQHandler(void)
{
	if(I2C_GetITStatus(I2C1,  I2C_IT_SMBALERT)) {
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT)) {
    } 
    else if(I2C_GetITStatus(I2C1, I2C_IT_PECERR)) {
    } 
    else if(I2C_GetITStatus(I2C1, I2C_IT_OVR)) {

    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_AF)) {

        I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_ARLO)) {

    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_BERR)) {

    }

	I2C_GenerateSTOP(I2C1, ENABLE);//release sda scl bus
}


/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{}



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
