/*****************************************************************************
文件名称: main.c
文件描述: 程序主方法
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
					CAN1_Write_Data_Array();    向CAN1发送数据

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
	uint16_t timei;			//延时变量
	uint8_t temp_num;		//临时SRx选择变量
	uint8_t temp[200];	//临时数据缓存
	uint8_t temp_len;		//临时数据长度
	
	SystemInit();				//系统时钟初始化

	USART1_Init();			//USART1初始化
	USART2_Init();			//USART2初始化
	USART3_Init();			//USART3初始化
	
	CAN1_Init();				//因为CAN的管脚和USART1管脚复用问题  必须将CAN1 的初始化放到 USART1后面
	
	printf("system start ... \r\n");
	while (1)
	{
		for(temp_num = 0;temp_num<4;temp_num++)					
		{
			temp_len = CAN1_Read_Data_Len(temp_num);			//读取CAN1对应的SRx设备的缓存数据长度
			if(temp_len > 0)                              //如果CAN1口有对应的SRx设备的数据
			{                                             
				for(timei=0;timei<10000;timei++);		        //延时等待数据接收
				if(temp_len ==CAN1_Read_Data_Len(temp_num)) //数据接收完成，进行数据处理
				{                                           
					CAN1_Read_Data_Array(temp,temp_num);      //读取CAN1缓存区对应的SRx设备的数据
					USART3_Write_Data_Array(temp,temp_len);		//将数据通过USART3发送出去
				
					printf("read can data:");									//输出调试信息
					UART1_WriteHEX(temp,temp_len);            //输出调试信息
				}
			}
		}
		
		temp_len = USART3_Read_Data_Len();							//读取USART3缓存数据长度
		if(temp_len > 0)                                //如果USART3口有数据
		{                                               
			for(timei=0;timei<10000;timei++);			        //延时等待数据接收
			if(temp_len ==USART3_Read_Data_Len() )        //数据接收完成，进行数据处理
			{                                             
				USART3_Read_Data_Array(temp);               //读取USART3缓存区数据
				CAN1_Write_Data_Array(temp,temp_len);       //将数据通过CAN1发送出去
				
				printf("read usart3 data:");								//输出调试信息
				UART1_WriteHEX(temp,temp_len);							//输出调试信息
			}	
		}			
  }
}
