
//#include "rs485_usart2_app.h"
#include "rs232_usart3_app.h"
#include "can1_app.h"
#include "mrv01_app.h"
#include "systick.h"

int main()
{	
	uint8_t temp_num;		//��ʱSRxѡ�����
	uint8_t temp[200];	//��ʱ���ݻ���
	uint8_t temp_len;		//��ʱ���ݳ���
	
	SysTick_Init(72);				//ϵͳʱ�ӳ�ʼ��
    //TIM2_Init();
    MRV_Init();

	//RS485_Init(9600);			//USART2��ʼ��
	USART3_Init(9600);			//USART3��ʼ��
	
	//CAN1_Init();				//��ΪCAN�Ĺܽź�USART1�ܽŸ�������  ���뽫CAN1 �ĳ�ʼ���ŵ� USART1����
	
	//printf("system start ... \r\n");
	while (1)
	{
        /*
		for(temp_num = 0;temp_num<4;temp_num++)					
		{
			temp_len = CAN1_Read_Data_Len(temp_num);			//��ȡCAN1��Ӧ��SRx�豸�Ļ������ݳ���
			if(temp_len > 0)                              //���CAN1���ж�Ӧ��SRx�豸������
			{                                             
				for(timei=0;timei<10000;timei++);		        //��ʱ�ȴ����ݽ���
				if(temp_len ==CAN1_Read_Data_Len(temp_num)) //���ݽ�����ɣ��������ݴ���
				{                                           
					CAN1_Read_Data_Array(temp,temp_num);      //��ȡCAN1��������Ӧ��SRx�豸������
					USART3_Write_Data_Array(temp,temp_len);		//������ͨ��USART3���ͳ�ȥ
				
					//printf("read can data:");									//���������Ϣ
					//UART1_WriteHEX(temp,temp_len);            //���������Ϣ
				}
			}
		}
		
		temp_len = USART3_Read_Data_Len();							//��ȡUSART3�������ݳ���
		if(temp_len > 0)                                //���USART3��������
		{                                               
			for(timei=0;timei<10000;timei++);			        //��ʱ�ȴ����ݽ���
			if(temp_len ==USART3_Read_Data_Len() )        //���ݽ�����ɣ��������ݴ���
			{ 
                USART3_Read_Data_Array(temp);               //��ȡUSART3����������
                if(temp[0] == 0xAA && temp[1] == 0xBB)
                {
                    CAN1_Write_Data_Array(temp,temp_len);       //������ͨ��CAN1���ͳ�ȥ
                    //printf("read usart3 data:");								//���������Ϣ
                    //UART1_WriteHEX(temp,temp_len);							//���������Ϣ
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
        temp_len = USART3_Read_Data_Len();							//��ȡUSART3�������ݳ���
		if(temp_len > 0)                                //���USART3��������
		{                                               
			delay_ms(200);
			if(temp_len ==USART3_Read_Data_Len() )        //���ݽ�����ɣ��������ݴ���
			{ 
                USART3_Read_Data_Array(temp);               //��ȡUSART3����������
                RS485_WRITE_DATA_ARRAY(temp,temp_len);
            }
        }
        
        temp_len = RS485_READ_DATA_LEN();							//��ȡUSART3�������ݳ���
		if(temp_len > 0)                                //���USART3��������
		{                                               
			delay_ms(200);			        //��ʱ�ȴ����ݽ���
			if(temp_len ==RS485_READ_DATA_LEN() )        //���ݽ�����ɣ��������ݴ���
			{ 
                RS485_READ_DATA_ARRAY(temp);               //��ȡUSART3����������
                USART3_Write_Data_Array(temp,temp_len);
            }
        }
        */
    }
}


