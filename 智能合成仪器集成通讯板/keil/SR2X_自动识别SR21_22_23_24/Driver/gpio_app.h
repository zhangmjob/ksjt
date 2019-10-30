/*****************************************************************************
文件名称: gpio_app.h
文件描述: GPIO口驱动头文件
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190322
历史版本: 无
*****************************************************************************/
#ifndef __GPIO_APP_H
#define __GPIO_APP_H

#include "stm32f10x_conf.h"
void GPIO_App_Init(void);
uint8_t Get_GPIO_Input_Value(void);

#endif /* __USART3_APP_H */

