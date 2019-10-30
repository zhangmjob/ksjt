#include "rs485_usart2_app.h"
#include "systick.h"


uint8_t USART2_Read_Data_Buffer[64];                   //接收缓冲，最大64字节
uint8_t USART2_Read_Data_Len=0;                       //接收字节计数器


void RS485_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOA,ENABLE); //使能GPIOA\G时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;	//TX-485	//串口输出PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);		/* 初始化串口输入IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//RX-485	   //串口输入PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	USART_Cmd(USART2, ENABLE);  //使能串口 2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接受中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	//RS485_TX_EN=0;				//默认为接收模式	
}
									
		
/*******************************************************************************
* 函 数 名         : USART2_IRQHandler
* 函数功能		   : USART2中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
void USART2_IRQHandler(void)
{
	uint8_t res;	                                    //定义数据缓存变量
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
		res =USART_ReceiveData(USART2);//;读取接收到的数据USART2->DR
		if(USART2_Read_Data_Len<sizeof(USART2_Read_Data_Buffer))    //一次只能接收64个字节，人为设定，可以更大，但浪费时间
		{
			USART2_Read_Data_Buffer[USART2_Read_Data_Len]=res;  //记录接收到的值
			USART2_Read_Data_Len++;        //使收数据增加1 
		}
	}
}

//返回接受到的数据长度
uint8_t RS485_READ_DATA_LEN()
{
    return USART2_Read_Data_Len;
}

//从串口读取数据
//ReadDataArray：接受数据字符串的首地址
void RS485_READ_DATA_ARRAY(uint8_t * ReadDataArray)
{
	uint8_t i;
	for(i=0;i<USART2_Read_Data_Len;i++)
	{
		ReadDataArray[i] = USART2_Read_Data_Buffer[i];		//将缓存区数据循环放入保存数据地址
	}
	USART2_Read_Data_Len = 0;	
}

//向串口写入数据
//WriteDataArray：要写入数据的首地址
//WriteDataLen：写入数据的字节长度
void RS485_WRITE_DATA_ARRAY(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{	
	uint8_t i;	
	for(i=0;i<WriteDataLen;i++)
	{
		USART_SendData(USART2, WriteDataArray[i]);										//向USART2发送数据
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);   //等待USART2数据发送完成
	}
}
