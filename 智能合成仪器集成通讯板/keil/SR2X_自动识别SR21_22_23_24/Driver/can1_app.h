/*****************************************************************************
�ļ�����: can1_app.h
�ļ�����: CAN1������ͷ�ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#ifndef __CAN1_APP_H
#define __CAN1_APP_H

#include "stm32f10x_conf.h"

void CAN1_Init(void);
uint8_t CAN1_Read_Data_Len(void);
void CAN1_Read_Data_Array(uint8_t * ReadDataArray);
void CAN1_Write_Data_Array(uint16_t STDID,uint8_t * WriteDataArray,uint8_t WriteDataLen);

#endif /* __CAN1_APP_H */
