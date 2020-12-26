/**************************************************************
* @file         hal_key.h
* @brief        hal_key.c��Ӧͷ�ļ�
* @author       Gizwits
* @date         2016-09-05
* @version      V03010101
* @copyright    Gizwits
* 
* @note         ������.ֻΪ����Ӳ������
*               Gizwits Smart Cloud  for Smart Products
*               ����|��ֵ|����|����|��ȫ|����|����|��̬
*               www.gizwits.com
*
***********************************************************/

#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include <stdio.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "stmflash.h"
#define G_SET_BIT(a,b)                            (a |= (1 << b))
#define G_CLEAR_BIT(a,b)                          (a &= ~(1 << b))
#define G_IS_BIT_SET(a,b)                         (a & (1 << b))

#define KEY_TIMER_MS                            1
#define KEY_MAX_NUMBER                          12
#define DEBOUNCE_TIME                           30
#define PRESS_LONG_TIME                         3000

#define NO_KEY                                  0x0000
#define KEY_DOWN                                0x1000
#define KEY_UP                                  0x2000
#define KEY_LIAN                                0x4000
#define KEY_LONG                                0x8000




#define GP PEout(7)//7��������
#define LP2 PEout(9)// 8��ˮ��2
#define LP3 PEout(11)// 9,ˮ��3
#define S7 PEout(13)// 10
#define LED6 PEout(15)// 11
#define LP1 PBout(11)// 12,ˮ��1

#define S1 PAout(8)// 1,��ŷ�1
#define S2 PAout(11)// 2
#define S3 PDout(8)// 3
#define S4 PDout(10)// 4
#define S5 PDout(12)// 5
#define S6 PDout(14)// 6


#define shidu GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0)//ʪ�ȼ��
#define	water_l GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12)//ˮλ���
#define	water_h GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_14)

#define KEY0  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_13)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)//��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//��ȡ����4
#define KEY5  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_15)//��ȡ����5

#define KEY6  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����6
//#define KEY7  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ����7


#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����
#define KEY3_PRES 	4	//KEY3
#define KEY4_PRES	5	//KEY4����
#define KEY5_PRES 	6	//KEY5����

//���غ��İ��ϵİ������������ҷֱ�Ϊ	KEY6��KEY7��
#define KEY6_PRES 	7	//KEY5����
#define KEY7_PRES 	8	//KEY5����





extern int8_t water_flag;
extern int16_t control_flag;//�Զ�ģʽ(0),�ֶ�ģʽ(1)

extern uint8_t step2_flag;//״̬2��־
extern uint8_t step3_flag;//״̬3��־
extern uint8_t step4_flag;//״̬4��־

void KEY_GPIO_Init(void);//IO��ʼ��
u8 KEY_Scan(void);  	//����ɨ�躯��		
void key_control(void);//�������ƺ���
void auto_control(void);//�Զ����ƺ���
void display_default(void);//������ŷ���ˮ��״̬��ʾ������

void water(void);
typedef void (*gokitKeyFunction)(void);

/*********************************************
 * ����һ���ṹ��
 * ��Ž����־λ
*/
typedef struct
{
    uint8_t Interface_Mark;     //����״̬
    uint8_t Task_Mark;          //����״̬
    uint8_t Run_Task;           //��ʼ��������
} Mark;




__packed typedef struct
{
    uint8_t          keyNum;
    uint32_t         keyRccPeriph;
    GPIO_TypeDef     *keyPort;
    uint32_t         keyGpio;
    gokitKeyFunction shortPress; 
    gokitKeyFunction longPress; 
}keyTypedef_t;

__packed typedef struct
{
    uint8_t      keyTotolNum;
    keyTypedef_t *singleKey; 
}keysTypedef_t; 

void keyGpioInit(void);
void keyHandle(keysTypedef_t *keys);
void keyParaInit(keysTypedef_t *keys);
uint16_t getKey(keysTypedef_t *key);
uint16_t readKeyValue(keysTypedef_t *keys);
keyTypedef_t keyInitOne(uint32_t keyRccPeriph, GPIO_TypeDef * keyPort, uint32_t keyGpio, gokitKeyFunction shortPress, gokitKeyFunction longPress);
#endif/*_KEY_H*/
