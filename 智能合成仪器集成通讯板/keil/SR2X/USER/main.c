/*****************************************************************************
文件名称: main.c
文件描述: 程序主方法和CAN1口发送的数据进行协议化
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/

#include "usart1_app.h"
#include "usart2_app.h"
#include "usart3_app.h"
#include "can1_app.h"

/*****************************************************************************
函数名称: Edit_CAN_Send_pack
函数描述: 对向CAN1口发送的数据进行协议化
调用函数: CAN1_Write_Data_Array 向CAN1发送数据
被调函数：main
传入参数: 
					uint8_t * Send_Data   需要通过CAN1口发送的数据
					uint8_t Usart_Num			是哪一个USART口发送的
					int8_t Data_Len				需要发送的数据长度
返 回 值: 空
其    他: 无
*****************************************************************************/

void Edit_CAN_Send_pack(uint8_t * Send_Data,uint8_t Usart_Num,uint8_t Data_Len)
{
	uint8_t can_send_data[200];
	uint8_t i;
	
	can_send_data[0] = 0xAA;
	can_send_data[1] = 0xBB;
	can_send_data[2] = Data_Len+3;
	can_send_data[3] = Usart_Num;
	can_send_data[4] = 0xA2;
	
	for(i=0;i<Data_Len;i++)
	{
		can_send_data[5+i] = Send_Data[i];
	}
	
	can_send_data[Data_Len+5] = 0x00;	
	can_send_data[Data_Len+6] = 0x00;	
	can_send_data[Data_Len+7] = 0x00;	
	
	CAN1_Write_Data_Array(can_send_data,Data_Len+8);
}


/*****************************************************************************
函数名称: main
函数描述: 主函数入口
调用函数: 
					SystemInit();								系统时钟初始化                                
          USART1_Init();        			USART1初始化
          USART2_Init();        			USART2初始化
          USART3_Init();        			USART3初始化
					CAN1_Init(); 								CAN1初始化

					CAN1_Read_Data_Len();				获取CAN1数据长度
					CAN1_Read_Data_Array();     读取CAN1数据
					Edit_CAN_Send_pack();       对数据进行打包并通过CAN1发送出去

					USART1_Read_Data_Len();			获取USART1串口数据长度
					USART1_Read_Data_Array();		读取USART1数据
					USART1_Write_Data_Array();	向USART1发送数据

					USART2_Read_Data_Len();			获取USART2串口数据长度
					USART2_Read_Data_Array();   读取USART2数据
					USART2_Write_Data_Array();  向USART2发送数据
					
					USART3_Read_Data_Len();			获取USART3串口数据长度
					USART3_Read_Data_Array();   读取USART3数据
					USART3_Write_Data_Array();  向USART3发送数据

被调函数：无
传入参数: 无
返 回 值: 无
其    他: 无
*****************************************************************************/

int main()
{	
	uint16_t timei;				//延时变量
	uint8_t temp[200];		//临时数据缓存
	uint8_t temp_len;			//临时数据长度

	SystemInit();					//系统时钟初始化
	                      
	USART1_Init();        //USART1初始化
	USART2_Init();        //USART2初始化
	USART3_Init();        //USART3初始化
	
	CAN1_Init();         //因为CAN的管脚和USART1管脚复用问题  必须将CAN1 的初始化放到 USART1后面
	
	while (1)																										//开始主循环
	{
		temp_len = CAN1_Read_Data_Len();													//读取CAN1缓存数据长度
		if(temp_len>0)																						//如果CAN1口有数据
		{
			for(timei=0;timei<10000;timei++);												//延时等待数据接收
			if(temp_len ==CAN1_Read_Data_Len())										  //数据接收完成，进行数据处理
			{
				CAN1_Read_Data_Array(temp);														//读取CAN1缓存区数据
				if(temp[0] != 0xAA||temp[1] != 0xBB)									//判断报文头
					continue;
				if(temp[2] != temp_len-5)															//判断数据长度
					continue;
				switch(temp[3])																				//判断需要转发的串口号，发送到对应的串口
				{
					case UART1_ID:
						USART1_Write_Data_Array(temp+5,temp_len-8);
						break;
					case UART2_ID:
						USART2_Write_Data_Array(temp+5,temp_len-8);
						break;
					case UART3_ID:
						USART3_Write_Data_Array(temp+5,temp_len-8);
						break;
					default:break;
				}
			}
		}
	
		temp_len = USART1_Read_Data_Len();												//读取USART1缓存数据长度
		if(temp_len>0)                                          	//如果USART1口有数据
		{                                                       
			for(timei=0;timei<10000;timei++);	                    	//延时等待数据接收
			if(temp_len ==USART1_Read_Data_Len() )                	//数据接收完成，进行数据处理
			{
				USART1_Read_Data_Array(temp);													//读取USART1缓存区数据
				Edit_CAN_Send_pack(temp,UART1_ID,temp_len);						//对数据进行打包并通过CAN1发送出去
			}	
		}
		
		temp_len = USART2_Read_Data_Len();												//读取USART2缓存数据长度
		if(temp_len>0)                                            //如果USART2口有数据
		{                                                         
			for(timei=0;timei<10000;timei++);	                      //延时等待数据接收
			if(temp_len ==USART2_Read_Data_Len() )                  //数据接收完成，进行数据处理
			{                                                       
				 USART2_Read_Data_Array(temp);                        //读取USART2缓存区数据
				 Edit_CAN_Send_pack(temp,UART2_ID,temp_len);          //对数据进行打包并通过CAN1发送出去
			}
		}
		
		temp_len = USART3_Read_Data_Len();												//读取USART3缓存数据长度
		if(temp_len>0)                                            //如果USART3口有数据
		{                                                         
			for(timei=0;timei<10000;timei++);	                      //延时等待数据接收
			if(temp_len ==USART3_Read_Data_Len())                   //数据接收完成，进行数据处理
			{                                                       
				USART3_Read_Data_Array(temp);                         //读取USART3缓存区数据
				Edit_CAN_Send_pack(temp,UART3_ID,temp_len);           //对数据进行打包并通过CAN1发送出去
			}	 
		}
  }
}
