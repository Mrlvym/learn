#include "bsp_iap.h"
#include "bsp_iwwdg.h"


void Bsp_Per_DeInit(void)
{
	RCC->AHB1RSTR = 0xFFFFFFFFU;
	RCC->AHB1RSTR = 0x00U;

	RCC->AHB2RSTR = 0xFFFFFFFFU;
	RCC->AHB2RSTR = 0x00U;

	RCC->AHB3RSTR = 0xFFFFFFFFU;
	RCC->AHB3RSTR = 0x00U;

	RCC->APB1RSTR = 0xFFFFFFFFU;
	RCC->APB1RSTR = 0x00U;

	RCC->APB2RSTR = 0xFFFFFFFFU;
	RCC->APB2RSTR = 0x00U;
}

void Bsp_SysTick_DeInit(void)
{
	SysTick->CTRL = 0x00U;
	SysTick->LOAD = 0x00U;
	SysTick->VAL  = 0x00U;
}

void Bsp_Clear_AllIRQ_Pending(void)
{
	uint8_t i = 0;

	for(i = 0; i < 8; i++)
	{
		NVIC->ICER[i] = 0xFFFFFFFFU;
		NVIC->ICPR[i] = 0xFFFFFFFFU;
	}
}

void Bsp_Jump_2_App(uint32_t app_addr)
{
	Bsp_Boot_Jump_Func Bsp_Boot2App;

	//关闭全局中断
	__disable_irq();

	//复位所有外设
	Bsp_Per_DeInit();

	//关闭所有中断，清除所有中断挂起标志
	Bsp_Clear_AllIRQ_Pending();

	//复位systick
	Bsp_SysTick_DeInit();

	//重设独立看门狗，超时时间为5s
//	Bsp_IWDG_Init(BSP_IWDG_Prescaler_64, 5*500);
//	Bsp_IWDG_Feed();

	//开启全局中断
	__enable_irq();

	//APP首地址+4就是APP复位中断服务程序地址
	Bsp_Boot2App = (Bsp_Boot_Jump_Func)(*(uint32_t *)(app_addr + 4));

	//设置主堆栈指针
	__set_MSP(*(uint32_t *)app_addr);

	//设置为特权级模式，使用MSP指针 （BOOT是RTOS的一定要设置，否则后面的APP无法运行，最好都加上无伤大雅）
	__set_CONTROL(0);

	//跳转到系统BootLoader
	Bsp_Boot2App();
	
}



