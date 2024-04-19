#include "bsp_timer.h"


void Bsp_TIMx_NVIC_Cfg(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	// 设置中断组为0
	//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn; 	
	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;	 
	// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Bsp_TIMx_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK
	RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

	/* 累计 TIM_Period个后产生一个更新或者中断*/		
	//当定时器从0计数到4999，即为5000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Period = 1000-1;       

	//定时器时钟源TIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK=HCLK/2=SystemCoreClock/2=84MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 168-1;	

	// 初始化定时器TIMx
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);


	// 清除定时器更新中断标志位
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

	// 开启定时器更新中断
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);

	// 使能定时器
	TIM_Cmd(BASIC_TIM, ENABLE);	
}

volatile uint32_t CPU_RunTime = 0;

void BASIC_TIM_IRQHandler(void)
{
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ){
		CPU_RunTime++;
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);
	}
}

