/*****************************************************************************
文件名称: gpio_app.c
文件描述: GPIO驱动文件
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190322
历史版本: 无
*****************************************************************************/
#include "gpio_app.h"

/*****************************************************************************
函数名称: GPIO_Init
函数描述: GPIO始始化
调用函数: STM32固件库函数
被调函数：main
传入参数: 无
返 回 值: 空
其    他: 无
*****************************************************************************/
void GPIO_App_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	/***********************打开时钟*******************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);				//打开GPIOB时钟
	
	/***********************配置管脚*******************************************/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 							  //上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);												//初始化PB10
}

/*****************************************************************************
函数名称: Get_GPIO_Input_Value
函数描述: 返回IO输入的状态
调用函数: STM32固件库函数
被调函数：main
传入参数: 无
返 回 值: 空
其    他: 无
*****************************************************************************/

uint8_t Get_GPIO_Input_Value(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==SET)
		{
			return 3;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==SET)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}	
}
