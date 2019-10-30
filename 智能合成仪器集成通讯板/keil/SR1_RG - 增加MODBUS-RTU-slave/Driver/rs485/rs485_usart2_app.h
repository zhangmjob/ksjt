
#ifndef __RS485_USART2_APP_H
#define __RS485_USART2_APP_H

#include "stm32f10x_conf.h"

//模式控制
//#define RS485_TX_EN		PGout(3)	//485模式控制.0,接收;1,发送.
														 
void RS485_Init(uint32_t bound);
uint8_t RS485_READ_DATA_LEN(void);
void RS485_READ_DATA_ARRAY(uint8_t * ReadDataArray);
void RS485_WRITE_DATA_ARRAY(uint8_t * WriteDataArray,uint8_t WriteDataLen);


#endif /* __USART2_APP_H */
