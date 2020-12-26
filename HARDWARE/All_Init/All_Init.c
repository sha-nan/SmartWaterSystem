#include "All_Init.h"
extern Mark Mark_Sign;//状态标志位
/*******************************************************************************
**函数名：init
**输入  : 无
**输出  : 无
**描述  ：配置并初始化外设
*******************************************************************************/
void All_Init()
{
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级	
	uart1_init(115200);	 //串口1初始化为115200，调试信息打印
	//uart2_init(9600);  //串口2，用于设备MCU与WiFi模组之间的数据通信
 	LED_Init();			     //LED端口初始化
	KEY_GPIO_Init();          //初始化与按键连接的硬件接口
 	GPIO_Configuration();//IO口初始化
	TIM2_Int_Init(9999,7199);//10Khz的计数频率，计数到5000为500ms
	TIM3_Int_Init(100,7199);//10Khz的计数频率，计数到5000为500ms
	//TIM4_Int_Init(9999,7199);//10Khz的计数频率，计数到5000为500ms
	OLED_Init();
	Mark_Sign.Interface_Mark=0x01;//主界面
//	Adc_Init();		  		//ADC初始化
//	usmart_dev.init(SystemCoreClock/1000000);//usmart调试工具初始化
//	gizwitsInit();
//	GIZWITS_LOG("MCU Init Success \n");//系统初始化完成
	//__set_PRIMASK(1);          //关总中断
}

/*******************************************************************************
**函数名：GPIO_Init
**输入  : 无
**输出  : 无
**描述  ：配置并初始化外设的管脚
*******************************************************************************/
void  GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//声明GPIO管脚初始化结构体    
	/*第一个继电器从左到右依次为1，2，3，4，5，6*/
	//	/*初始化PA8,PA=11*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	//GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11);			  //设置GPIOA的第3管脚为1
	/*初始化电磁阀的管脚（PD8,PD10,PD12,PD14）*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	//GPIO_SetBits(GPIOD, GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14);			  //设置GPIOD的第3管脚为1


	
	/*第二个继电器从左到右依次为7，8，9，10，11，12*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PD端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/*初始化最高液位与最低液位检测的管脚（PG12,PG14）*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入	
	GPIO_Init(GPIOG,&GPIO_InitStructure);	

	
}
