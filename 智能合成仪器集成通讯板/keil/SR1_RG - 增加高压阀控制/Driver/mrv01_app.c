/*****************************************************************************
文件名称: mrv01_app.c
文件描述: 高压阀控制驱动库
作    者: zm
作者邮箱：2468635978@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/
#include "mrv01_app.h"

/*****************************************************************************
函数名称: MRV01_Init
函数描述: 初始化高压阀控制IO
调用函数: STM32固件库函数
被调函数：main
传入参数: 无
返 回 值: 空
其    他: 无
*****************************************************************************/
void MRV_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB, ENABLE);				//打开GPIOA时钟
  
  /***********************配置管脚*******************************************/
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							//推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);												//初始化
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							//推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);												//初始化
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);												//初始化
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);												//初始化
  
  GPIO_ResetBits(GPIOC,GPIO_Pin_12);   //初始化为低电平
  GPIO_ResetBits(GPIOD,GPIO_Pin_2);
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_12);
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}

void MRV1_Change()
{
  GPIO_SetBits(GPIOC,GPIO_Pin_12);//led2
  GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void MRV1_Reback()
{
  GPIO_ResetBits(GPIOC,GPIO_Pin_12);
  GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}

void MRV2_Change()
{
  GPIO_SetBits(GPIOD,GPIO_Pin_2);//led1
  GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

void MRV2_Reback()
{
  GPIO_ResetBits(GPIOD,GPIO_Pin_2);
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
