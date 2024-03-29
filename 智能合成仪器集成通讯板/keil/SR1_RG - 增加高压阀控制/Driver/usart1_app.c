/*****************************************************************************
文件名称: usart1_app.c
文件描述: usart1驱动文件
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/
#include "usart1_app.h"

/*****************************************************************************
函数名称: USART1_Init
函数描述: USART1始始化
调用函数: STM32固件库函数
被调函数：main
传入参数: 无
返 回 值: 空
其    他: 无
*****************************************************************************/
void USART1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;  	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/***********************打开时钟*******************************************/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);			//打开USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				//打开GPIOA时钟
	
	/***********************配置管脚*******************************************/
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 							//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//初始化PA9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				//开漏输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//初始化PA10

	
	/***********************配置串口*******************************************/
	
	USART_InitStructure.USART_BaudRate = 9600;										//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    		//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;   				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 								//收发模式
	USART_Init(USART1, &USART_InitStructure);	
	
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);									//开UART1中断
	USART_Cmd(USART1, ENABLE);																		//启动USART1
	USART_GetFlagStatus(USART1,USART_FLAG_TC);										//清空状态 避免第一个字符丢失
	
	/***********************配置中断*******************************************/
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);								//选择分组0  
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;							//USART1中断通道
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;						//子优先1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能USART1中断
  NVIC_Init(&NVIC_InitStructure);																//对NVIC初始化
}

/*****************************************************************************
函数名称: USART1_Write_Data_Array
函数描述: 向USART1发送数据
调用函数: STM32固件库函数
被调函数：main
传入参数: 
					WriteDataArray			需要发送的数据
					WriteDataLen				需要发送的数据长度
返 回 值: 空
其    他: 无
*****************************************************************************/

void USART1_Write_Data_Array(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{	
	uint8_t i;	
	for(i=0;i<WriteDataLen;i++)
	{
		USART_SendData(USART1, WriteDataArray[i]);										//向USART1发送数据
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   //等待USART1数据发送完成
	}
}

/*****************************************************************************
变量名称: USART1_READ_DATA_Buffer
变量描述: USART1接收数据缓存
其    他: 无
*****************************************************************************/
uint8_t USART1_READ_DATA_Buffer[200];

/*****************************************************************************
变量名称: USART1_READ_DATA_Len
变量描述: USART1接收数据长度
其    他: 无
*****************************************************************************/
uint8_t USART1_READ_DATA_Len = 0;

/*****************************************************************************
函数名称: USART1_IRQHandler
函数描述: USART1中断方法
调用函数: 无
被调函数：无
传入参数: 无
返 回 值: 无
其    他: 无
*****************************************************************************/

void USART1_IRQHandler()
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)														//如果寄存器中有数据
  {		
    USART1_READ_DATA_Buffer[USART1_READ_DATA_Len++] = USART_ReceiveData(USART1);	//将数据放入缓存区，长度加1
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 																//清空接收中断标志位
  }	
}

/*****************************************************************************
函数名称: USART1_Read_Data_Len
函数描述: 获取USART1缓存区数据长度
调用函数: 无
被调函数：main
传入参数: 无
返 回 值: 缓存区数据长度
其    他: 无
*****************************************************************************/

uint8_t USART1_Read_Data_Len()
{
	return USART1_READ_DATA_Len;													//返回数据长度																						
}


/*****************************************************************************
函数名称: USART1_Read_Data_Array
函数描述: 从USART1缓存区读取数据 并清空缓存区
调用函数: 无
被调函数：main
传入参数: ReadDataArray	 用来保存数据的地址
返 回 值: 空
其    他: 无
*****************************************************************************/

void USART1_Read_Data_Array(uint8_t * ReadDataArray)
{
	uint8_t i;
	for(i=0;i<USART1_READ_DATA_Len;i++)
	{
		ReadDataArray[i] = USART1_READ_DATA_Buffer[i];		//将缓存区数据循环放入保存数据地址
	}
	USART1_READ_DATA_Len = 0;	
}

/*****************************************************************************
函数名称: UART1_SendByte
函数描述: 向USART1发送1位数据
调用函数: STM32固件库函数
被调函数：main
传入参数: data			需要发送的数据
返 回 值: 无
其    他: 无
*****************************************************************************/
void UART1_SendByte(uint8_t data)
{
	USART_SendData(USART1, data);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

/*****************************************************************************
函数名称: UART1_WriteHEX
函数描述: 将二进制数据转换成文本数据发送到USART1
调用函数: STM32固件库函数
被调函数：main
传入参数: 
					pData			需要发送的数据
					nLen			需要发送的数据长度
返 回 值: 无
其    他: 无
*****************************************************************************/
void UART1_WriteHEX(uint8_t * pData,uint8_t nLen)
{
	char i;		
	char hextostr[]={"0123456789ABCDEF"};
	
	for (i=0; i<nLen; i++)
	{
		UART1_SendByte(' ');
		UART1_SendByte(hextostr[pData[i]/16]);
		UART1_SendByte(hextostr[pData[i]%16]);
		UART1_SendByte(' ');
	}
	UART1_SendByte('\r');
	UART1_SendByte('\n');
}

/*********************对printf方法的重映射*******************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}

int fgetc(FILE *f)
{  
   while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET));    
   return (USART_ReceiveData(USART1));
}
