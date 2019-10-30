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
#include "mrv01_app.h"
#include "crc16.h"

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



//把串口收到的数据打包发送给Modbus-rtu设备
void Modbus_Buff_Package(uint8_t *buff1,uint8_t *buff2,uint8_t buff_len)
{
    uint8_t i;
    uint16_t crc;
    uint8_t crc_tmp[2];
    buff2[0] = buff1[3];
    buff2[1] = buff1[4];
    for(i = 2; i < buff_len-3; i++){
        buff2[i] = buff1[i+3];
    }
    
    buff_len = buff_len - 3;
    crc = crc16tablefast(buff2, buff_len);
    Get_Crc_Array(crc,crc_tmp);
    
    for(i = 0;i < 2; i++){
        
        buff2[i + buff_len] = crc_tmp[i];
    }
    
    buff_len = buff_len + 2;
}
//把Modbus-rtu收到的数据打包发送给上位机
void Modbus_Reback_Package(uint8_t *buff1,uint8_t *buff2,uint8_t buff_len)
{
    uint8_t i;
    buff2[0] = 0x1A;
    buff2[1] = 0x1B;
    buff2[2] = 0xA2;
    for(i = 0; i < buff_len;i++){
       buff2[i+3] = buff1[i];
    }
    buff_len += 3;
}

uint16_t timei;			//延时变量
uint8_t temp_num;		//临时SRx选择变量
uint8_t temp[200];	//临时数据缓存
uint8_t temp_len;		//临时数据长度
uint8_t Modbus_Buff[200];
uint8_t Modbus_len;

int main()
{	
	
	SystemInit();				//系统时钟初始化
    MRV_Init();

	USART1_Init();			//USART1初始化
	USART2_Init();			//USART2初始化
	USART3_Init();			//USART3初始化
	
	CAN1_Init();				//因为CAN的管脚和USART1管脚复用问题  必须将CAN1 的初始化放到 USART1后面
	
	
	while (1)
	{
		for(temp_num = 0;temp_num<4;temp_num++)					
		{
			temp_len = CAN1_Read_Data_Len(temp_num);			//读取CAN1对应的SRx设备的缓存数据长度
			if(temp_len > 0)                              //如果CAN1口有对应的SRx设备的数据
			{                                             
				for(timei=0;timei<20000;timei++);		        //延时等待数据接收
				if(temp_len ==CAN1_Read_Data_Len(temp_num)) //数据接收完成，进行数据处理
				{                                           
					CAN1_Read_Data_Array(temp,temp_num);      //读取CAN1缓存区对应的SRx设备的数据
					USART3_Write_Data_Array(temp,temp_len);		//将数据通过USART3发送出去
				}
			}
		}
		
		temp_len = USART3_Read_Data_Len();							//读取USART3缓存数据长度
		if(temp_len > 0)                                //如果USART3口有数据
		{                                               
			for(timei=0;timei<20000;timei++);			        //延时等待数据接收
			if(temp_len == USART3_Read_Data_Len() )        //数据接收完成，进行数据处理
			{ 
                USART3_Read_Data_Array(temp);               //读取USART3缓存区数据
                
                /*向下行Can设备发送数据*/
                if(temp[0] == 0xAA && temp[1] == 0xBB)
                {
                    CAN1_Write_Data_Array(temp,temp_len);       //将数据通过CAN1发送出去
                    USART1_Write_Data_Array(temp,temp_len);    //将下行的数据通过串口一打印出来
                }
                                
                /*向高压阀设备发送数据*/
                if(temp[0] == 0x0A && temp[1] == 0x0B)
                {
                    if(temp[2] == 0x01)
                    {
                        if(temp[3] == 0x10)
                        {
                            MRV1_Change();// 0A 0B 01 10  PB12  33   = 1
                        }
                        else
                        {
                        MRV1_Reback();//0A 0B 01 00
                        }
                    }
                    if(temp[2] == 0x02)
                    {
                        if(temp[3] == 0x10)
                        {
                            MRV2_Change();//0A 0B 02 10
                        }
                        else
                        {
                            MRV2_Reback();//0A 0B 02 00
                        }
                    }
                    if(temp[2] == 0x03)
                    {
                        if(temp[3] == 0x10) //0A 0B 03 10
                        {
                            MRV1_Change();
                            MRV2_Change();
                        }
                        else                // 0A 0B 03 00
                        {
                            MRV1_Reback();
                            MRV2_Reback();
                        }
                    }
                }
                
                /*向Modbus slave设备发送数据*/
                if(temp[0] == 0x1A && temp[1] == 0x1B)
                {
                    Modbus_Buff_Package(temp,Modbus_Buff,temp_len);
                    USART2_Write_Data_Array(Modbus_Buff,temp_len-1);
                }
            }   
			
		}	
        /*Modbus slave设备的数据接受*/
        temp_len = USART2_Read_Data_Len();
        if(temp_len > 0)
        {
            for(timei=0;timei<20000;timei++);		        //延时等待数据接收
            if(temp_len == USART2_Read_Data_Len())
            {
                USART2_Read_Data_Array(temp);
                Modbus_Reback_Package(temp,Modbus_Buff,temp_len);
                USART3_Write_Data_Array(Modbus_Buff,temp_len+3);
            }
        }
		
    }
}
