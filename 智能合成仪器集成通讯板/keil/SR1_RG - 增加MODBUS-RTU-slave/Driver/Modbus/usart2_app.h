
#ifndef __USART2_APP_H
#define __USART2_APP_H

#include "stm32f10x_conf.h"

//模式控制
//#define RS485_TX_EN		PGout(3)	//485模式控制.0,接收;1,发送.
														 
void RS485_Init(uint32_t bound);
//void TIM2_Init(void);
//void UartRxMonitor(uint8_t ms); //串口接收监控
//void UartDriver(void); //串口驱动函数void UartRead(uint8_t *buf, uint8_t len); //串口接收数据
//void rs485_UartWrite(uint8_t *buf2 ,uint8_t len2);  //串口发送数据
//uint8_t UartRead(uint8_t *buf, uint8_t len) ;
uint8_t RS485_READ_DATA_LEN(void);
void RS485_READ_DATA_ARRAY(uint8_t * ReadDataArray);
void RS485_WRITE_DATA_ARRAY(uint8_t * WriteDataArray,uint8_t WriteDataLen);


#endif /* __USART2_APP_H */
