/*****************************************************************************
�ļ�����: usart1_app.h
�ļ�����: USART1������ͷ�ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#ifndef __USART1_APP_H
#define __USART1_APP_H

#include "stm32f10x_conf.h"

void USART1_Init(void);
uint8_t USART1_Read_Data_Len(void);
void USART1_Read_Data_Array(uint8_t * ReadDataArray);
void USART1_Write_Data_Array(uint8_t *WriteDataArray,uint8_t WriteDataLen);

#endif /* __USART1_APP_H */
