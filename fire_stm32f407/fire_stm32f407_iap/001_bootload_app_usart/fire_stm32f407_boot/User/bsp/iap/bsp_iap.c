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

	//�ر�ȫ���ж�
	__disable_irq();

	//��λ��������
	Bsp_Per_DeInit();

	//�ر������жϣ���������жϹ����־
	Bsp_Clear_AllIRQ_Pending();

	//��λsystick
	Bsp_SysTick_DeInit();

	//����������Ź�����ʱʱ��Ϊ5s
//	Bsp_IWDG_Init(BSP_IWDG_Prescaler_64, 5*500);
//	Bsp_IWDG_Feed();

	//����ȫ���ж�
	__enable_irq();

	//APP�׵�ַ+4����APP��λ�жϷ�������ַ
	Bsp_Boot2App = (Bsp_Boot_Jump_Func)(*(uint32_t *)(app_addr + 4));

	//��������ջָ��
	__set_MSP(*(uint32_t *)app_addr);

	//����Ϊ��Ȩ��ģʽ��ʹ��MSPָ�� ��BOOT��RTOS��һ��Ҫ���ã���������APP�޷����У���ö��������˴��ţ�
	__set_CONTROL(0);

	//��ת��ϵͳBootLoader
	Bsp_Boot2App();
	
}



