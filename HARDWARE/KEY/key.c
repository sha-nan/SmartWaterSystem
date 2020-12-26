/**************************************************************
* @file         key.c
* @brief        ��������

* ����ģ����ö�ʱ�� + GPIO״̬��ȡ���ƣ�GPIO������Ҫ����STM32������ֲ�������

* ������֧�� 0 ~ 12 ��GPIO������չ��֧�ֿ�ƽ̨��ֲ��
* @author       Gizwits
* @date         2016-09-05
* @version      V03010101
* @copyright    Gizwits
* 
* @note         ������.ֻΪ����Ӳ������
*               Gizwits Smart Cloud  for Smart Products
*               ����|��ֵ?|����|����|��ȫ|����|����|��̬
*               www.gizwits.com
*
***********************************************************/

#include "stm32f10x.h"
#include "sys.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include <math.h>
#include "oled.h"

#include "stdint.h"
#include "bmp.h"
//#include "stmflash.h"
//���ݴ洢
//Ҫд�뵽STM32 FLASH���ַ�������
u16 Data_Buffer[4]={0};
#define DATASIZE sizeof(Data_Buffer)		//���鳤��
#define FLASH_SAVE_ADDR  0X08070000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
u16 datatemp[DATASIZE]={0};

//ʱ������
uint16_t t1_count=0;
uint16_t t2_count=0;
uint16_t t3_count=0;
uint16_t t4_count=0;

uint16_t * t1=&t1_count;
uint16_t * t2=&t2_count;
uint16_t * t3=&t3_count;
uint16_t * t4=&t4_count;

uint16_t new_t1=0;
uint16_t new_t2=0;
uint16_t new_t3=0;
uint16_t new_t4=0;
//ˮ��״̬��־
uint8_t LP1_state=0;
uint8_t LP2_state=0;
uint8_t LP3_state=0;
uint8_t GP_state=0;
//��ŷ�״̬��־
uint8_t S1_state=0;
uint8_t S2_state=0;
uint8_t S3_state=0;
uint8_t S4_state=0;
uint8_t S5_state=0;
uint8_t S6_state=0;
//ʱ���־
uint8_t change_time=0;

uint8_t t1_state=0;
uint8_t t2_state=0;
uint8_t t3_state=0;
uint8_t t4_state=0;


vu8 key=0;



int8_t  Menu_count=0;//�˵��л�����
int8_t	Shuiben_count=2;//ѡ��ˮ��
int8_t	Diancifa_count=2;//ѡ���ŷ�
int8_t  TimeChoose_count=2;//ѡ��ʱ��


void Main_Interface_APP(void);//��ʾ������
void Menu_Interface_APP(void);//��ʾ�˵�����
void Function_Menu_APP(void);//��ʾ���ܽ���

void shuiben_choose(void);//ѡ���ĸ�ˮ��
void diancifa_choose(void);//ѡ���ĸ���ŷ�
void time_choose(void);//ѡ���޸��ĸ�ʱ��

void change_display_queren(void);
void change_display_fanhui(void);

//��ˮ������
uint8_t sewage_state=1;//��ʼ״̬ 

uint8_t step2_flag=0;//״̬2��־
uint8_t step3_flag=0;//״̬3��־
uint8_t step4_flag=0;//״̬4��־



extern int time1_flag;//�Ƿ��time_count����
extern int time2_flag;
extern int time3_flag;
extern int time4_flag;




void key_control(void);
/*********************************************
 * ����һ��ö��
 * ��Ž������
*/
/*******************************************/
enum
{
	Main_Interface = 0x01, /****������*****/
	Menu_Interface = 0x02, /****�˵�����***/
	Task_Interface = 0x03, /****�������***/
};

/*********************************************
 * ����һ��ö��
 * ����������
*/
/*******************************************/
enum
{
	Shuiben_Set = 0x01, /****ˮ�����ý���*****/
	Diancifa_Set = 0x02, /****��ŷ����ý���***/
	Time_Set = 0x03, /****ʱ�����ý���***/
};

Mark Mark_Sign;//״̬��־λ
void water(void);

#if 0
extern keysTypedef_t keys;
uint8_t  keyCountTime;
static uint8_t keyTotolNum = 0;

/**
* @brief Read the GPIO state
* @param [in] keys :Key global structure pointer
* @return GPIO status 
*/
uint16_t getKey(keysTypedef_t *keyS)
{
    uint8_t i = 0; 
    uint16_t readKey = 0;              //Essential init
    
    //GPIO Cyclic scan
    for(i = 0; i < keys.keyTotolNum; i++)
    {
        if(!GPIO_ReadInputDataBit((GPIO_TypeDef*)keyS->singleKey[i].keyPort,keyS->singleKey[i].keyGpio))
        {
            G_SET_BIT(readKey, keyS->singleKey[i].keyNum);
        }
    }
    
    return readKey;
}

/**
* @brief Read the KEY value
* @param [in] keys :Key global structure pointer
* @return GPIO status
*/
uint16_t readKeyValue(keysTypedef_t *keyS)
{
    static uint8_t keyCheck = 0;
    static uint8_t keyState = 0;
    static uint16_t keyLongCheck = 0;
    static uint16_t keyPrev = 0;      //last key

    uint16_t keyPress = 0;
    uint16_t keyReturn = 0;
    
    keyCountTime ++;
    
    if(keyCountTime >= (DEBOUNCE_TIME / KEY_TIMER_MS))          //keyCountTime 1MS+1  key eliminate jitter 10MS
    {
        keyCountTime = 0;
        keyCheck = 1;
    }
    if(1 == keyCheck)
    {
        keyCheck = 0;
        keyPress = getKey(keyS);
        switch (keyState)
        {
            case 0:
                if(keyPress != 0)
                {
                    keyPrev = keyPress;
                    keyState = 1;
                }
                break;
                
            case 1:
                if(keyPress == keyPrev)
                {
                    keyState = 2;
                    keyReturn= keyPrev | KEY_DOWN;
                }
                else                //Button lift, jitter, no response button
                {
                    keyState = 0;
                }
                break;
                
            case 2:

                if(keyPress != keyPrev)
                {
                    keyState = 0;
                    keyLongCheck = 0;
                    keyReturn = keyPrev | KEY_UP;
                    return keyReturn;
                }
                if(keyPress == keyPrev)
                {
                    keyLongCheck++;
                    if(keyLongCheck >= (PRESS_LONG_TIME / DEBOUNCE_TIME))    //Long press 3S effective
                    {
                        keyLongCheck = 0;
                        keyState = 3;
                        keyReturn= keyPress | KEY_LONG;
                        return keyReturn;
                    }
                }
                break;

            case 3:
                if(keyPress != keyPrev)
                {
                    keyState = 0;
                }
                break;
        }
    }
    return  NO_KEY;
}

/**
* @brief Key call-back function
* Detects the keys state and call the corresponding callback function
* @param [in] keys :Key global structure pointer
* @return none
*/
void keyHandle(keysTypedef_t *keyS)
{
    uint8_t i = 0;
    uint16_t key_value = 0;

    key_value = readKeyValue(keyS);

    if(!key_value) return;
    
    //Check short press button
    if(key_value & KEY_UP)
    {
        //Valid key is detected
        for(i = 0; i < keyS->keyTotolNum; i++)
        {
            if(G_IS_BIT_SET(key_value, keyS->singleKey[i].keyNum)) 
            {
                //key callback function of short press
                if(keyS->singleKey[i].shortPress) 
                {
                    keyS->singleKey[i].shortPress(); 

                }
            }
        }
    }

    //Check short long button
    if(key_value & KEY_LONG)
    {
        //Valid key is detected
        for(i = 0; i < keyS->keyTotolNum; i++)
        {
            if(G_IS_BIT_SET(key_value, keyS->singleKey[i].keyNum))
            {
                //key callback function of long press
                if(keyS->singleKey[i].longPress) 
                {
                    keyS->singleKey[i].longPress(); 

                }
            }
        }
    }
} 


/**
* @brief key init function
* @param [in] keyRccPeriph APB2_peripheral
* @param [in] keyPort Peripheral_declaration
* @param [in] keyGpio GPIO_pins_define 
* @param [in] short_press :Short press state callback function address
* @param [in] long_press :Long press state callback function address
* @return key structure pointer
*/
keyTypedef_t keyInitOne(uint32_t keyRccPeriph, GPIO_TypeDef * keyPort, uint32_t keyGpio, gokitKeyFunction shortPress, gokitKeyFunction longPress)
{
    static int8_t key_total = -1;

    keyTypedef_t singleKey;
    
    //Platform-defined GPIO
    singleKey.keyRccPeriph = keyRccPeriph;
    singleKey.keyPort = keyPort;
    singleKey.keyGpio = keyGpio;
    singleKey.keyNum = ++key_total;
    
    //Button trigger callback type
    singleKey.longPress = longPress;
    singleKey.shortPress = shortPress;
    
    keyTotolNum++;
    
    return singleKey;
}

/**
* @brief key parameter init function

* Keys GPIO init,start timer detect keys state
* @param [in] keys ::Key global structure pointer
* @return none
*/
void keyParaInit(keysTypedef_t *keys)
{
//     uint8_t temI = 0; 
    
    if(NULL == keys)
    {
        return ;
    }
    
    keys->keyTotolNum = keyTotolNum;
    
    //Limit on the number keys (Allowable number: 0~12)
    if(KEY_MAX_NUMBER < keys->keyTotolNum) 
    {
        keys->keyTotolNum = KEY_MAX_NUMBER; 
    }
}
#endif

#if 1
//������ʼ������
void KEY_GPIO_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʪ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//ʹ��PORTG
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	//���İ��ϵİ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;//KEY6,7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//�׿ذ��ϰ����ֱȱ�ΪKEY0--KEY5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//ʹ��PORTF
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_11|GPIO_Pin_15;//KEY0,1,5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTE
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_12;//KEY2��3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTB
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;//KEY4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� 
//5��KEY4���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3>KEY4!!
u8 KEY_Scan(void)
{	
	static u8 key_up=1;//�����ɿ���־
	key_up =1;
	if (key_up&&(KEY0==1||KEY1==1||KEY2==1||KEY3==1||KEY4==1||KEY5==1))
	{
		delay_ms(100);//ȥ����
		key_up=0;
		if(KEY0==1)return KEY0_PRES;
		else if(KEY1==1)return KEY1_PRES;
		else if(KEY2==1)return KEY2_PRES;
		else if(KEY3==1)return KEY3_PRES;
		else if(KEY4==1)return KEY4_PRES;
		else if(KEY5==1)return KEY5_PRES;
	}		
 	return 0;// �ް�������
}
//�������ƺ�����ʵ����Ӧ����
void key_control(void)
{
//	int i,j;
	//������ȡ
		key=KEY_Scan();	//�õ���ֵ					   
		switch(key)
		{		
			case KEY5_PRES:	//����
//				new_t1 = (*t1)*1;//200;
//				new_t2 = (*t2)*1;;//200
//				new_t3 = (*t3)*1;
//				new_t4 = (*t4)*1;
				new_t1 = (*t1)*60;//200;
				new_t2 = (*t2)*60;;//200
				new_t3 = (*t3)*60;
				new_t4 = (*t4)*60;
				Data_Buffer[0] = new_t1;
				Data_Buffer[1] = new_t2;
				Data_Buffer[2] = new_t3;
				Data_Buffer[3] = new_t4;
				STMFLASH_Write(FLASH_SAVE_ADDR,(uint16_t*)Data_Buffer,DATASIZE);//д������
//				delay_ms(100);
				OLED_ShowString(85,6,"W"); 
			break;	
			
			case KEY4_PRES:	//�¼�/��
				LED1=!LED1;
				OLED_Clear(); 
				Menu_count++;
				if(3==Menu_count) Menu_count=0;
				//ˮ��ѡ��
				if(Mark_Sign.Task_Mark == Shuiben_Set)
				{
					Shuiben_count++;
					if(6==Shuiben_count) Shuiben_count=2;
				}
				//��ŷ�ѡ��
				else if(Mark_Sign.Task_Mark == Diancifa_Set)
				{
					Diancifa_count++;
					if(8==Diancifa_count) Diancifa_count=2;
				}
				//ʱ��ѡ��
				else if(Mark_Sign.Task_Mark == Time_Set)
				{
					TimeChoose_count++;
					if(6==TimeChoose_count)	TimeChoose_count=2;
				}
			
			break;
			
			case KEY3_PRES:	//�ϼ�/��
				OLED_Clear(); 
				LED1=!LED1;
				Menu_count--;
				if((-3)==Menu_count) Menu_count=0;
				//ˮ��ѡ��
				if(Mark_Sign.Task_Mark == Shuiben_Set)
				{
					Shuiben_count--;
					if((-2)==Shuiben_count) Shuiben_count=2;
				}
				//��ŷ�ѡ��
				else if(Mark_Sign.Task_Mark == Diancifa_Set)
				{
					Diancifa_count--;
					if((-4)==Diancifa_count) Diancifa_count=2;
				}
				//ʱ��ѡ��
				if(Mark_Sign.Task_Mark == Time_Set)
				{
					TimeChoose_count--;
					if((-2)==TimeChoose_count) TimeChoose_count=2;
				}				
			break;
			
			case KEY2_PRES:	//���ؼ�
				LED1=!LED1;
				OLED_Clear(); 
				change_display_fanhui();
			break;
			
			case KEY1_PRES:	//ȷ�ϼ�
				OLED_Clear();
				change_display_queren();
				LED1=!LED1;
			break;
			
			case KEY0_PRES:	//����ģʽ�л�
				OLED_Clear();
				LED1=!LED1;
				control_flag=~control_flag;
				STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,DATASIZE);
				new_t1=datatemp[0];
				new_t2=datatemp[1];
				new_t3=datatemp[2];
				new_t4=datatemp[3];
				*t1 = new_t1/60;
				*t2 = new_t2/60;
				*t3 = new_t3/60;
				*t4 = new_t4/60;
//				*t1 = new_t1;
//				*t2 = new_t2;
//				*t3 = new_t3;
//				*t4 = new_t4;
				OLED_ShowString(85,6,"R"); 

				break;
		}
}
	/*************************************************************************
 *  ��������   auto_control  
 *  ����˵���� ��ʼ״̬
 *  ����˵���� ��
 *  �������أ� ��
 *  �޸�ʱ�䣺
 *  ��    ע��
 *************************************************************************/
#define step1       1     //����1:��1��2��LP1,��3��4��5
#define step2       2     //����2������Һλ2����1����3��ά��0.5h��Һλ���ƣ�
#define step3       3     //����3����2��3��LP1����5,GP����2h
#define step4       4     //����4����1��2��3��4��5��LP1��GP������1h
#define step5       5     //����5����4,LP3��Һλ1ʱ���ر�4
void auto_control(void)
{
  switch(sewage_state)
  { 
    case step1:
				S2 =1;
				LP1 =1;
				S4=0;
				LP3=0;
				
				if(1==water_h)
				{
					while(0==water_h);
					sewage_state=2;//���벽��2
				}
     break;
				
    case step2:
				step2_flag=1;
				S2 =0;
				LP1 =0;
				//���ƽ������
				S1=1;
				LP3=1;
	
				S4=0;
//				if(1800==time_count1) //����0.5h
//				if(20==time_count1) //����0.5h
				if(new_t1==time_count1) //����0.5h
				{
					sewage_state=3;//���벽��3
					step2_flag=0;//ֹͣ��time_count1����������
				}
     break;
				
	case step3:
				step3_flag=1;
				S2 =0;
				LP1 =0;

				GP=1;
	
				LP3=0;
				S1=0;	
				S4=0;
//				if(7200==time_count2) //����2h
//				if(20==time_count2) //����2h
				if(new_t2==time_count2) //����0.5h
				{
					sewage_state=4;//���벽��4
					step3_flag=0;//ֹͣ��time_count2����������
				}
     break;
				
	case step4:
				step4_flag=1;
			    S1 =0;
				S2 =0;
				LP1 =0;
				S4=0;
				GP=0;
				LP3=0;
//				if(3600==time_count3) //����1h
//				if(20==time_count3) //����1h
				if(new_t3==time_count3) //����0.5h	
				{
					step4_flag=0;//ֹͣ��time_count3����������
					sewage_state=5;//���벽��5
				}
     break;
				
	case step5: 
			    S1=0;
				S2 =0;
				LP1 =0;
				S4=1;
				LP3=1;
				S1 =0;//S1���ƻ���
				if(1==water_l) //����Һλ1
				{
					
					step2_flag=0;//�رն�time_count3����
					step3_flag=0;//��time_count3����
					step4_flag=0;//��time_count3����
					sewage_state=1;//���벽��1
				}
		break;
    default :break;    
  }
 }
#endif


void water(void)
{
	//��ȿ���
	if(1==water_flag||1==shidu)//ˮ��ֲ��
	{
		LP2=1;
		if(1==water_flag) S6=1;
		else S6=0;
		if(1==shidu) S5=1;
		else S5=0;
		
	}
	else if(0==water_flag&&0==shidu)
	{
//		if(0==water_flag) S6=0;
//		else
//		{
			S5=0;
			S6=0;
			LP2=0;
//		}
//		if(0==shidu) S5=0;
//		else 
//		{
//			S5=1;
//			LP2=1;
//		}
		
	}
//	else if(0==water_flag)
//	{
//		S6=0;
////		LP2=0;
//	}
//	else if(1 == shidu)//����ֲ��
//	{
//		S5=1;
//		LP2=1;
//	}
//	else if(0==shidu)
//	{
//		S5=0;
//		LP2=0;
//	}
}


void change_display_queren(void)
{
	if(Main_Interface == Mark_Sign.Interface_Mark)
	{
		 /**************����˵�����*************/
		Mark_Sign.Interface_Mark = Menu_Interface;
	}
	/************�жϵ�ǰ����Ϊ�˵�����*******************/
	else if(Menu_Interface == Mark_Sign.Interface_Mark)
	{
		/***************�����������************/
		Mark_Sign.Interface_Mark = Task_Interface;
	}
        /************�жϵ�ǰ����Ϊ�������******************/
	else if(Task_Interface == Mark_Sign.Interface_Mark)
	{
	/*******�жϵ�ǰ�������е�����*******/
	}
}

void change_display_fanhui(void)
{
	//�����·��ؼ�ʱ���жϵ�ǰ����
	/************�жϵ�ǰ����Ϊ�˵�����*******************/
	if(Menu_Interface == Mark_Sign.Interface_Mark)
	{
		/*******�˳��˵�����***����������**/
		Mark_Sign.Interface_Mark = Main_Interface;
	}
	/************�жϵ�ǰ����Ϊ�������******************/
	else if(Task_Interface == Mark_Sign.Interface_Mark)
	{
		/*******�˳��������*****/
		Mark_Sign.Interface_Mark = Menu_Interface;
	}
}

void display_mode(void)//
{
	key_control();
	switch(Mark_Sign.Interface_Mark)
	{
		//״̬��־λ ������
		case Main_Interface:
			Main_Interface_APP();//��ʾ������
		break;
		//״̬��־λ �˵�����
		case Menu_Interface:
			Menu_Interface_APP();//��ʾ�˵�����
		break;
		//״̬��־λ �������
		case Task_Interface:
			Function_Menu_APP();//��ʾ���ܽ���
		break;
		
		default:
		break;
	}
}

void Main_Interface_APP(void)//��ʾ������
{
		//д���ʱ��ÿ���б������16����
		//������
		OLED_ShowCHinese(16,1,0);//���ܹ��ϵͳ
		OLED_ShowCHinese(32,1,1);
		OLED_ShowCHinese(48,1,2);
		OLED_ShowCHinese(64,1,3);
		OLED_ShowCHinese(80,1,4);
		OLED_ShowCHinese(96,1,5);
		OLED_ShowString(50,4,"V0.1");//�汾��
}

void Menu_Interface_APP(void)//��ʾ�˵�����
{	
	//�˵����棬����12*12
	if(0==Menu_count||(-3)==Menu_count) 
	{
		Mark_Sign.Task_Mark = Shuiben_Set;
		OLED_DrawBMP(0,0,12,2,BMP3);//��ͷָ��1������ˮ��
	}
	else if(1==Menu_count||(-2)==Menu_count)	
	{
		Mark_Sign.Task_Mark = Diancifa_Set;
		OLED_DrawBMP(0,2,12,4,BMP3);//2�����Ƶ�ŷ�
	}
	else if(2==Menu_count||(-1)==Menu_count)	
	{
		Mark_Sign.Task_Mark = Time_Set;
		OLED_DrawBMP(0,4,12,6,BMP3);//3���޸�ʱ��
	}
	
	OLED_ShowCHinese(16,0,6);//1������ˮ��
	OLED_ShowCHinese(32,0,7);
	OLED_ShowCHinese(48,0,8);
	OLED_ShowCHinese(64,0,9);

	
	OLED_ShowCHinese(16,2,6);//2�����Ƶ�ŷ�
	OLED_ShowCHinese(32,2,7);
	OLED_ShowCHinese(48,2,10);
	OLED_ShowCHinese(64,2,11);
	OLED_ShowCHinese(80,2,12);

	
	OLED_ShowCHinese(16,4,13);//3���޸�ʱ��
	OLED_ShowCHinese(32,4,14);
	OLED_ShowCHinese(48,4,15);
	OLED_ShowCHinese(64,4,16);
	//ȷ��������
	OLED_ShowCHinese(4,6,17);
	OLED_ShowCHinese(20,6,18);		
	OLED_ShowCHinese(100,6,19);
	OLED_ShowCHinese(116,6,20);
}

void Function_Menu_APP(void)//��ʾ���ܽ���
{	
	switch(Mark_Sign.Task_Mark)
	{
		case Shuiben_Set: 
			shuiben_choose();//ѡ���ĸ�ˮ��
			//ˮ�ÿ��ƽ���
			//��һ��
			OLED_ShowString(16,0,"LP1:");
			OLED_ShowString(16,2,"LP2:");	
			//�ڶ���
			OLED_ShowString(82,0,"LP3:");
			OLED_ShowString(82,2,"GP :");	
			break;
		
		case Diancifa_Set: //��ŷ����ƽ���
			diancifa_choose();//ѡ����һ����ŷ�
			//��һ��
			OLED_ShowString(16,0,"S1:");
			OLED_ShowString(16,2,"S2:");
			OLED_ShowString(16,4,"S3:");
			//�ڶ���
			OLED_ShowString(82,0,"S4:");
			OLED_ShowString(82,2,"S5:");	
			OLED_ShowString(82,4,"S6:");
			break;
		
		case Time_Set: //�޸�ʱ�����
			//ʱ����ƽ���
			//��һ��
			OLED_ShowString(16,0,"T1:");
			OLED_ShowString(16,2,"T2:");	
			//�ڶ���
			OLED_ShowString(80,0,"T3:");
			OLED_ShowString(80,2,"T4:");	
			time_choose();//ѡ���ĸ�ʱ��
			break;		
	}
	//ȷ��������
	OLED_ShowCHinese(4,6,17);
	OLED_ShowCHinese(20,6,18);		
	OLED_ShowCHinese(100,6,19);
	OLED_ShowCHinese(116,6,20);
	
	OLED_ShowString(54,6,"min");
}


void shuiben_choose(void)//ѡ���ĸ�ˮ��
{
	if(2==Shuiben_count||(-2)==Shuiben_count)//ѡ��ˮ��1
	{
		if(2==key)	
		{
			LP1=~LP1;
			if(1==LP1)	LP1_state=1;
			else LP1_state=0;
		}
		OLED_DrawBMP(0,0,12,2,BMP3);//ˮ��1
	}
	else if(3==Shuiben_count||(-1)==Shuiben_count)//ѡ��ˮ��2
	{
		if(2==key)	
		{
			LP2=~LP2;
			if(1==LP2)	LP2_state=1;
			else LP2_state=0;
		}
		OLED_DrawBMP(0,2,12,4,BMP3);//ˮ��2
	}	
	else if(4==Shuiben_count||0==Shuiben_count)//ѡ��ˮ��3
	{
		if(2==key)	
		{
			LP3=~LP3;
			if(1==LP3)	LP3_state=1;
			else LP3_state=0;
		}		
		OLED_DrawBMP(66,0,78,2,BMP3);//ˮ��3
	}
	else if(5==Shuiben_count||1==Shuiben_count)//ѡ��ˮ��4
	{
		if(2==key)	
		{
			GP=~GP;
			if(1==GP)	GP_state=1;
			else GP_state=0;
		}		
		OLED_DrawBMP(66,2,78,4,BMP3);//ˮ��4
	}
	else;
	
	if(1==LP1_state) OLED_ShowCHinese(50,0,21);//��
	else OLED_ShowCHinese(50,0,22);//��
	
	if(1==LP2_state) OLED_ShowCHinese(50,2,21);//��
	else OLED_ShowCHinese(50,2,22);//��
	
	if(1==LP3_state) OLED_ShowCHinese(115,0,21);//��
	else OLED_ShowCHinese(115,0,22);//��
	
	if(1==GP_state) OLED_ShowCHinese(115,2,21);//��
	else OLED_ShowCHinese(115,2,22);//��
}


//////////OLED_DrawBMP(0,0,12,2,BMP3);//��ͷ
//////////OLED_DrawBMP(0,2,12,4,BMP3);//��ͷ
//////////OLED_DrawBMP(0,4,12,6,BMP3);//��ͷ
//////////OLED_DrawBMP(66,0,78,2,BMP3);//��ͷ
//////////OLED_DrawBMP(66,2,78,4,BMP3);//��ͷ
//////////OLED_DrawBMP(66,4,78,6,BMP3);//��ͷ

void diancifa_choose(void)//ѡ����һ����ŷ�
{
	if(2==Diancifa_count||(-4)==Diancifa_count)//ѡ���ŷ�1
	{
		if(2==key)	
		{
			S1=~S1;
			if(1==S1)	S1_state=1;
			else S1_state=0;
		}		
		OLED_DrawBMP(0,0,12,2,BMP3);//��ͷ
	}
	else if(3==Diancifa_count||(-3)==Diancifa_count)//ѡ���ŷ�2
	{
		if(2==key)	
		{
			S2=~S2;
			if(1==S2)	S2_state=1;
			else S2_state=0;
		}		
		OLED_DrawBMP(0,2,12,4,BMP3);//��ͷ
	}
	else if(4==Diancifa_count||(-2)==Diancifa_count)//ѡ���ŷ�3
	{
		if(2==key)	
		{
			S3=~S3;
			if(1==S3)	S3_state=1;
			else S3_state=0;
		}		
		OLED_DrawBMP(0,4,12,6,BMP3);//��ͷ
	}
	else if(5==Diancifa_count||(-1)==Diancifa_count)//ѡ���ŷ�4
	{
		if(2==key)	
		{
			S4=~S4;
			if(1==S4)	S4_state=1;
			else S4_state=0;
		}		
		OLED_DrawBMP(66,0,78,2,BMP3);//��ͷ
	}
	else if(6==Diancifa_count||0==Diancifa_count)//ѡ���ŷ�5
	{
		if(2==key)	
		{
			S5=~S5;
			if(1==S5)	S5_state=1;
			else S5_state=0;
		}		
		OLED_DrawBMP(66,2,78,4,BMP3);//��ͷ
	}
	else if(7==Diancifa_count||1==Diancifa_count)//ѡ���ŷ�6
	{
		if(2==key)	
		{
			S6=~S6;
			if(1==S6)	S6_state=1;
			else S6_state=0;
		}		
		OLED_DrawBMP(66,4,78,6,BMP3);//��ͷ
	}
	
	if(1==S1_state) OLED_ShowCHinese(40,0,21);//��
	else OLED_ShowCHinese(40,0,22);//��
	
	if(1==S2_state) OLED_ShowCHinese(40,2,21);//��
	else OLED_ShowCHinese(40,2,22);//��		

	if(1==S3_state) OLED_ShowCHinese(40,4,21);//��
	else OLED_ShowCHinese(40,4,22);//��	

	if(1==S4_state) OLED_ShowCHinese(108,0,21);//��
	else OLED_ShowCHinese(108,0,22);//��	

	if(1==S5_state) OLED_ShowCHinese(108,2,21);//��
	else OLED_ShowCHinese(108,2,22);//��	

	if(1==S6_state) OLED_ShowCHinese(108,4,21);//��
	else OLED_ShowCHinese(108,4,22);//��		
}

void time_choose(void)
{
	if(2==TimeChoose_count||(-2)==TimeChoose_count)//ѡ��ʱ��1
	{
		OLED_DrawBMP(0,0,12,2,BMP3);//ʱ��1
		if(2==key)
		{
			while(key!=3)
			{
				key=KEY_Scan();	//�õ���ֵ
				if(4==key) *t1=*t1+10;
				if(5==key) 
				{
					*t1=*t1-10;				
				}
				if(*t1<=0) *t1=0;
				OLED_ShowNum(40,0,*t1,3,16);
				OLED_ShowNum(40,2,*t2,3,16);
				OLED_ShowNum(104,0,*t3,3,16);
				OLED_ShowNum(104,2,*t4,3,16);
			}
		}
	}
	else if(3==TimeChoose_count||(-1)==TimeChoose_count)//ѡ��ʱ��2
	{
		OLED_DrawBMP(0,2,12,4,BMP3);//ʱ��2
		if(2==key)
		{
			while(key!=3)
			{
				key=KEY_Scan();	//�õ���ֵ
				if(4==key) *t2=*t2+10;
				if(5==key) 
				{
					*t2=*t2-10;				
				}
				if(*t2<=0) *t2=0;
				OLED_ShowNum(40,0,*t1,3,16);
				OLED_ShowNum(40,2,*t2,3,16);
				OLED_ShowNum(104,0,*t3,3,16);
				OLED_ShowNum(104,2,*t4,3,16);
			}
		}
	}	
	else if(4==TimeChoose_count||0==TimeChoose_count)//ѡ��ʱ��3
	{	
		OLED_DrawBMP(66,0,78,2,BMP3);//ʱ��3
		if(2==key)
		{
			while(key!=3)
			{
				key=KEY_Scan();	//�õ���ֵ
				if(4==key) *t3=*t3+10;
				if(5==key) 
				{
					*t3=*t3-10;				
				}
				if(*t3<=0) *t3=0;
				OLED_ShowNum(40,0,*t1,3,16);
				OLED_ShowNum(40,2,*t2,3,16);
				OLED_ShowNum(104,0,*t3,3,16);
				OLED_ShowNum(104,2,*t4,3,16);
			}
		}		
		
	}
	else if(5==TimeChoose_count||1==TimeChoose_count)//ѡ��ʱ��4
	{	
		OLED_DrawBMP(66,2,78,4,BMP3);//ʱ��4
		if(2==key)
		{
			while(key!=3)
			{
				key=KEY_Scan();	//�õ���ֵ
				if(4==key) *t4=*t4+10;
				if(5==key) 
				{
					*t4=*t4-10;				
				}
				if(*t4<=0) *t4=0;
				OLED_ShowNum(40,0,*t1,3,16);
				OLED_ShowNum(40,2,*t2,3,16);
				OLED_ShowNum(104,0,*t3,3,16);
				OLED_ShowNum(104,2,*t4,3,16);
			}
		}		
	}
	else;
	
	OLED_ShowNum(40,0,*t1,3,16);
	OLED_ShowNum(40,2,*t2,3,16);
	OLED_ShowNum(104,0,*t3,3,16);
	OLED_ShowNum(104,2,*t4,3,16);
}



