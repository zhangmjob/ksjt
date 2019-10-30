
//#include "rs485_usart2_app.h"
#include "rs232_usart3_app.h"
#include "can1_app.h"
#include "mrv01_app.h"
#include "systick.h"

int main()
{	
	uint8_t temp_num;		//临时SRx选择变量
	uint8_t temp[200];	//临时数据缓存
	uint8_t temp_len;		//临时数据长度
	
	SysTick_Init(72);				//系统时钟初始化
    //TIM2_Init();
    MRV_Init();

	//RS485_Init(9600);			//USART2初始化
	USART3_Init(9600);			//USART3初始化
	
	//CAN1_Init();				//因为CAN的管脚和USART1管脚复用问题  必须将CAN1 的初始化放到 USART1后面
	
	//printf("system start ... \r\n");
	while (1)
	{
        /*
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
				
					//printf("read can data:");									//输出调试信息
					//UART1_WriteHEX(temp,temp_len);            //输出调试信息
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
                if(temp[0] == 0xAA && temp[1] == 0xBB)
                {
                    CAN1_Write_Data_Array(temp,temp_len);       //将数据通过CAN1发送出去
                    //printf("read usart3 data:");								//输出调试信息
                    //UART1_WriteHEX(temp,temp_len);							//输出调试信息
                }
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
                
			}	
		}*/		
        /*
        temp_len = USART3_Read_Data_Len();							//读取USART3缓存数据长度
		if(temp_len > 0)                                //如果USART3口有数据
		{                                               
			delay_ms(200);
			if(temp_len ==USART3_Read_Data_Len() )        //数据接收完成，进行数据处理
			{ 
                USART3_Read_Data_Array(temp);               //读取USART3缓存区数据
                RS485_WRITE_DATA_ARRAY(temp,temp_len);
            }
        }
        
        temp_len = RS485_READ_DATA_LEN();							//读取USART3缓存数据长度
		if(temp_len > 0)                                //如果USART3口有数据
		{                                               
			delay_ms(200);			        //延时等待数据接收
			if(temp_len ==RS485_READ_DATA_LEN() )        //数据接收完成，进行数据处理
			{ 
                RS485_READ_DATA_ARRAY(temp);               //读取USART3缓存区数据
                USART3_Write_Data_Array(temp,temp_len);
            }
        }
        */
    }
}


