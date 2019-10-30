/*****************************************************************************
�ļ�����: usart2_app.h
�ļ�����: USART2������ͷ�ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#ifndef __USART2_APP_H
#define __USART2_APP_H

#include "stm32f10x_conf.h"

void USART2_Init(void);
uint8_t USART2_Read_Data_Len(void);
void USART2_Read_Data_Array(uint8_t * ReadDataArray);
void USART2_Write_Data_Array(uint8_t *WriteDataArray,uint8_t WriteDataLen);

#endif /* __USART2_APP_H */
