/*****************************************************************************
文件名称: usart1_app.h
文件描述: USART1口驱动头文件
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/
#ifndef __USART1_APP_H
#define __USART1_APP_H

#include "stm32f10x_conf.h"

void USART1_Init(void);
uint8_t USART1_Read_Data_Len(void);
void USART1_Read_Data_Array(uint8_t * ReadDataArray);
void USART1_Write_Data_Array(uint8_t *WriteDataArray,uint8_t WriteDataLen);

#endif /* __USART1_APP_H */
