/*****************************************************************************
�ļ�����: usart3_app.h
�ļ�����: USART3������ͷ�ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#ifndef __USART3_APP_H
#define __USART3_APP_H

#include "stm32f10x_conf.h"

void USART3_Init(uint32_t bound);
uint8_t USART3_Read_Data_Len(void);
void USART3_Read_Data_Array(uint8_t * ReadDataArray);
void USART3_Write_Data_Array(uint8_t *WriteDataArray,uint8_t WriteDataLen);

#endif /* __USART3_APP_H */
