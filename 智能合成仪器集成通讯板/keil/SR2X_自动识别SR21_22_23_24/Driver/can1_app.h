/*****************************************************************************
文件名称: can1_app.h
文件描述: CAN1口驱动头文件
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/
#ifndef __CAN1_APP_H
#define __CAN1_APP_H

#include "stm32f10x_conf.h"

void CAN1_Init(void);
uint8_t CAN1_Read_Data_Len(void);
void CAN1_Read_Data_Array(uint8_t * ReadDataArray);
void CAN1_Write_Data_Array(uint16_t STDID,uint8_t * WriteDataArray,uint8_t WriteDataLen);

#endif /* __CAN1_APP_H */
