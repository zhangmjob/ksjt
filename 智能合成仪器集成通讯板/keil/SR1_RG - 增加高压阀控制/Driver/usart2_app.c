/*****************************************************************************
文件名称: usart2_app.c
文件描述: USART2驱动文件
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/
#include "usart2_app.h"

/*****************************************************************************
函数名称: USART2_Init
函数描述: USART2始始化
调用函数: STM32固件库函数
被调函数：main
传入参数: 无
返 回 值: 空
其    他: 无
*****************************************************************************/
void USART2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;  	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/***********************打开时钟*******************************************/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);			//打开USART2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				//打开GPIOA时钟
	
	/***********************配置管脚*******************************************/
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 							//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//初始化PA2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				//开漏输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//初始化PA3

	
	/***********************配置串口*******************************************/
	
	USART_InitStructure.USART_BaudRate = 9600;										//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    		//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;   				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 								//收发模式
	USART_Init(USART2, &USART_InitStructure);	
	
	USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);									//开UART1中断
	USART_Cmd(USART2, ENABLE);																		//启动USART2
	USART_GetFlagStatus(USART2,USART_FLAG_TC);										//清空状态 避免第一个字符丢失
	
	/***********************配置中断*******************************************/
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);								//选择分组0  
	
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;							//USART2中断通道
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						//子优先2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能USART2中断
    NVIC_Init(&NVIC_InitStructure);																//对NVIC初始化
}

/*****************************************************************************
函数名称: USART2_Write_Data_Array
函数描述: 向USART2发送数据
调用函数: STM32固件库函数
被调函数：main
传入参数: 
					WriteDataArray			需要发送的数据
					WriteDataLen				需要发送的数据长度
返 回 值: 空
其    他: 无
*****************************************************************************/

void USART2_Write_Data_Array(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{	
	uint8_t i;	
	for(i=0;i<WriteDataLen;i++)
	{
		USART_SendData(USART2, WriteDataArray[i]);										//向USART2发送数据
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);   //等待USART2数据发送完成
	}
}

/*****************************************************************************
变量名称: USART2_READ_DATA_Buffer
变量描述: USART2接收数据缓存
其    他: 无
*****************************************************************************/
uint8_t USART2_READ_DATA_Buffer[200];

/*****************************************************************************
变量名称: USART2_READ_DATA_Len
变量描述: USART2接收数据长度
其    他: 无
*****************************************************************************/
uint8_t USART2_READ_DATA_Len = 0;

/*****************************************************************************
函数名称: USART2_IRQHandler
函数描述: USART2中断方法
调用函数: 无
被调函数：无
传入参数: 无
返 回 值: 无
其    他: 无
*****************************************************************************/

void USART2_IRQHandler()
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)														//如果寄存器中有数据
  {		
    USART2_READ_DATA_Buffer[USART2_READ_DATA_Len++] = USART_ReceiveData(USART2);	//将数据放入缓存区，长度加1
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 																//清空接收中断标志位
  }	
}

/*****************************************************************************
函数名称: USART2_Read_Data_Len
函数描述: 获取USART2缓存区数据长度
调用函数: 无
被调函数：main
传入参数: 无
返 回 值: 缓存区数据长度
其    他: 无
*****************************************************************************/

uint8_t USART2_Read_Data_Len()
{
	return USART2_READ_DATA_Len;													//返回数据长度																						
}


/*****************************************************************************
函数名称: USART2_Read_Data_Array
函数描述: 从USART2缓存区读取数据 并清空缓存区
调用函数: 无
被调函数：main
传入参数: ReadDataArray	 用来保存数据的地址
返 回 值: 空
其    他: 无
*****************************************************************************/

void USART2_Read_Data_Array(uint8_t * ReadDataArray)
{
	uint8_t i;
	for(i=0;i<USART2_READ_DATA_Len;i++)
	{
		ReadDataArray[i] = USART2_READ_DATA_Buffer[i];		//将缓存区数据循环放入保存数据地址
	}
	USART2_READ_DATA_Len = 0;	
}
