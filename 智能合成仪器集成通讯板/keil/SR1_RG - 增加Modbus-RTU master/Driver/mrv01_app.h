/*****************************************************************************
文件名称: mrv01_app.h
文件描述: USART3口驱动头文件
作    者: ZM
作者邮箱：2468635978@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/

#ifndef __MRV01_APP_H
#define __MRV01_APP_H


#include "stm32f10x_conf.h"
                     
void MRV_Init(void);
void MRV1_Change(void);
void MRV1_Reback(void);
void MRV2_Change(void);
void MRV2_Reback(void);

#endif
