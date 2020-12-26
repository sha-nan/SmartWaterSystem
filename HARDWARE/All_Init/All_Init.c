#include "All_Init.h"
extern Mark Mark_Sign;//״̬��־λ
/*******************************************************************************
**��������init
**����  : ��
**���  : ��
**����  �����ò���ʼ������
*******************************************************************************/
void All_Init()
{
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
	uart1_init(115200);	 //����1��ʼ��Ϊ115200��������Ϣ��ӡ
	//uart2_init(9600);  //����2�������豸MCU��WiFiģ��֮�������ͨ��
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_GPIO_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
 	GPIO_Configuration();//IO�ڳ�ʼ��
	TIM2_Int_Init(9999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	TIM3_Int_Init(100,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	//TIM4_Int_Init(9999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	OLED_Init();
	Mark_Sign.Interface_Mark=0x01;//������
//	Adc_Init();		  		//ADC��ʼ��
//	usmart_dev.init(SystemCoreClock/1000000);//usmart���Թ��߳�ʼ��
//	gizwitsInit();
//	GIZWITS_LOG("MCU Init Success \n");//ϵͳ��ʼ�����
	//__set_PRIMASK(1);          //�����ж�
}

/*******************************************************************************
**��������GPIO_Init
**����  : ��
**���  : ��
**����  �����ò���ʼ������Ĺܽ�
*******************************************************************************/
void  GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����GPIO�ܽų�ʼ���ṹ��    
	/*��һ���̵�������������Ϊ1��2��3��4��5��6*/
	//	/*��ʼ��PA8,PA=11*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	//GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11);			  //����GPIOA�ĵ�3�ܽ�Ϊ1
	/*��ʼ����ŷ��Ĺܽţ�PD8,PD10,PD12,PD14��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	//GPIO_SetBits(GPIOD, GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14);			  //����GPIOD�ĵ�3�ܽ�Ϊ1


	
	/*�ڶ����̵�������������Ϊ7��8��9��10��11��12*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PD�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/*��ʼ�����Һλ�����Һλ���Ĺܽţ�PG12,PG14��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������	
	GPIO_Init(GPIOG,&GPIO_InitStructure);	

	
}
