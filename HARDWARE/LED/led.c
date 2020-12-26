#include "led.h"
//初始化PF8和PF9为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PF端口时钟

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;				 //LED0-->PF.8, LED0-->PF.9端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	 GPIO_SetBits(GPIOF,GPIO_Pin_8|GPIO_Pin_9);						 //PF.8 输出高
}
 
