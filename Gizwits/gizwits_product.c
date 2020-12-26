/**************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*说明：产品相关处理函数
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "gizwits_product.h"
#include "All_Init.h"

static uint32_t timerMsCount;

/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_OxygenMotor_OnOff:
        currentDataPoint.valueOxygenMotor_OnOff = dataPointPtr->valueOxygenMotor_OnOff;
        GIZWITS_LOG("Evt: EVENT_OxygenMotor_OnOff %d \n", currentDataPoint.valueOxygenMotor_OnOff);
        if(0x01 == currentDataPoint.valueOxygenMotor_OnOff)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_WaterMotor_OnOff:
        currentDataPoint.valueWaterMotor_OnOff = dataPointPtr->valueWaterMotor_OnOff;
        GIZWITS_LOG("Evt: EVENT_WaterMotor_OnOff %d \n", currentDataPoint.valueWaterMotor_OnOff);
        if(0x01 == currentDataPoint.valueWaterMotor_OnOff)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_solenoid_valve:
        currentDataPoint.valuesolenoid_valve = dataPointPtr->valuesolenoid_valve;
        GIZWITS_LOG("Evt: EVENT_solenoid_valve %d \n", currentDataPoint.valuesolenoid_valve);
        if(0x01 == currentDataPoint.valuesolenoid_valve)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_SolenoidValve_Water:
        currentDataPoint.valueSolenoidValve_Water = dataPointPtr->valueSolenoidValve_Water;
        GIZWITS_LOG("Evt: EVENT_SolenoidValve_Water %d \n", currentDataPoint.valueSolenoidValve_Water);
        if(0x01 == currentDataPoint.valueSolenoidValve_Water)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_SolenoidValve_Soil:
        currentDataPoint.valueSolenoidValve_Soil = dataPointPtr->valueSolenoidValve_Soil;
        GIZWITS_LOG("Evt: EVENT_SolenoidValve_Soil %d \n", currentDataPoint.valueSolenoidValve_Soil);
        if(0x01 == currentDataPoint.valueSolenoidValve_Soil)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;

      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
        break;
      case WIFI_DISCON_ROUTER:
        break;
      case WIFI_CON_M2M:
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 /*
    currentDataPoint.valueLiquidLevel_Low = ;//Add Sensor Data Collection
    currentDataPoint.valueLiquidLevel_High = ;//Add Sensor Data Collection
    currentDataPoint.valueSoilHumidity = ;//Add Sensor Data Collection

    */
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
	All_Init();//全部外设初始化
	S1=0;
	S2=0;
	S3=0;
	S4=0;
	S5=0;
	S6=0;
	
	LP1=0;
	LP2=0;
	LP3=0;
	
	GP=0;
	//memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valueOxygenMotor_OnOff = ;
      currentDataPoint.valueWaterMotor_OnOff = ;
      currentDataPoint.valuesolenoid_valve = ;
      currentDataPoint.valueSolenoidValve_Water = ;
      currentDataPoint.valueSolenoidValve_Soil = ;
      currentDataPoint.valueLiquidLevel_Low = ;
      currentDataPoint.valueLiquidLevel_High = ;
      currentDataPoint.valueSoilHumidity = ;
    */

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)//串口2接受功能，用于接收与WiFi模组之间的通信协议数据
{
  uint8_t value = 0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		value = USART_ReceiveData(USART2);//STM32 test demo
		gizPutData(&value, 1);
	}
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)//串口2写操作，发送数据报文到WiFi模组
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
		
    for(i=0; i<len; i++)
    {
        USART_SendData(USART2, buf[i]);//STM32 test demo
				while(USART_GetITStatus(USART2,USART_FLAG_TXE)==RESET)
        //Serial port to achieve the function, the buf[i] sent to the module
        if(i >=2 && buf[i] == 0xFF)
        {
          //Serial port to achieve the function, the 0x55 sent to the module
          USART_SendData(USART2, 0x55);//STM32 test demo
					while(USART_GetITStatus(USART1,USART_FLAG_TXE)==RESET);
        }
    }   
		
		#ifdef PROTOCOL_DEBUG
			GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
			for(i=0; i<len; i++)
			{
					GIZWITS_LOG("%02x ", buf[i]);
					if(i>2&&buf[i]==0xff)
					{
							GIZWITS_LOG("%02x ", 0x55);
					}
			}
			GIZWITS_LOG("\n");
    #endif
    return len;
}


