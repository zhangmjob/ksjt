/*****************************************************************************
�ļ�����: main.c
�ļ�����: ����������
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#include "usart1_app.h"
#include "usart2_app.h"
#include "usart3_app.h"
#include "can1_app.h"
#include "mrv01_app.h"
#include "crc16.h"

/*****************************************************************************
��������: main
��������: ���������
���ú���: 
					SystemInit();								ϵͳʱ�ӳ�ʼ��                                
          USART1_Init();        			USART1��ʼ��
          USART2_Init();        			USART2��ʼ��
          USART3_Init();        			USART3��ʼ��
					CAN1_Init(); 								CAN1��ʼ��

					CAN1_Read_Data_Len();				��ȡCAN1���ݳ���
					CAN1_Read_Data_Array();     ��ȡCAN1����
					CAN1_Write_Data_Array();    ��CAN1��������

					USART1_Read_Data_Len();			��ȡUSART1�������ݳ���
					USART1_Read_Data_Array();		��ȡUSART1����
					USART1_Write_Data_Array();	��USART1��������

					USART2_Read_Data_Len();			��ȡUSART2�������ݳ���
					USART2_Read_Data_Array();   ��ȡUSART2����
					USART2_Write_Data_Array();  ��USART2��������
					
					USART3_Read_Data_Len();			��ȡUSART3�������ݳ���
					USART3_Read_Data_Array();   ��ȡUSART3����
					USART3_Write_Data_Array();  ��USART3��������

������������
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/



//�Ѵ����յ������ݴ�����͸�Modbus-rtu�豸
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
//��Modbus-rtu�յ������ݴ�����͸���λ��
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

uint16_t timei;			//��ʱ����
uint8_t temp_num;		//��ʱSRxѡ�����
uint8_t temp[200];	//��ʱ���ݻ���
uint8_t temp_len;		//��ʱ���ݳ���
uint8_t Modbus_Buff[200];
uint8_t Modbus_len;

int main()
{	
	
	SystemInit();				//ϵͳʱ�ӳ�ʼ��
    MRV_Init();

	USART1_Init();			//USART1��ʼ��
	USART2_Init();			//USART2��ʼ��
	USART3_Init();			//USART3��ʼ��
	
	CAN1_Init();				//��ΪCAN�Ĺܽź�USART1�ܽŸ�������  ���뽫CAN1 �ĳ�ʼ���ŵ� USART1����
	
	
	while (1)
	{
		for(temp_num = 0;temp_num<4;temp_num++)					
		{
			temp_len = CAN1_Read_Data_Len(temp_num);			//��ȡCAN1��Ӧ��SRx�豸�Ļ������ݳ���
			if(temp_len > 0)                              //���CAN1���ж�Ӧ��SRx�豸������
			{                                             
				for(timei=0;timei<20000;timei++);		        //��ʱ�ȴ����ݽ���
				if(temp_len ==CAN1_Read_Data_Len(temp_num)) //���ݽ�����ɣ��������ݴ���
				{                                           
					CAN1_Read_Data_Array(temp,temp_num);      //��ȡCAN1��������Ӧ��SRx�豸������
					USART3_Write_Data_Array(temp,temp_len);		//������ͨ��USART3���ͳ�ȥ
				}
			}
		}
		
		temp_len = USART3_Read_Data_Len();							//��ȡUSART3�������ݳ���
		if(temp_len > 0)                                //���USART3��������
		{                                               
			for(timei=0;timei<20000;timei++);			        //��ʱ�ȴ����ݽ���
			if(temp_len == USART3_Read_Data_Len() )        //���ݽ�����ɣ��������ݴ���
			{ 
                USART3_Read_Data_Array(temp);               //��ȡUSART3����������
                
                /*������Can�豸��������*/
                if(temp[0] == 0xAA && temp[1] == 0xBB)
                {
                    CAN1_Write_Data_Array(temp,temp_len);       //������ͨ��CAN1���ͳ�ȥ
                    USART1_Write_Data_Array(temp,temp_len);    //�����е�����ͨ������һ��ӡ����
                }
                                
                /*���ѹ���豸��������*/
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
                
                /*��Modbus slave�豸��������*/
                if(temp[0] == 0x1A && temp[1] == 0x1B)
                {
                    Modbus_Buff_Package(temp,Modbus_Buff,temp_len);
                    USART2_Write_Data_Array(Modbus_Buff,temp_len-1);
                }
            }   
			
		}	
        /*Modbus slave�豸�����ݽ���*/
        temp_len = USART2_Read_Data_Len();
        if(temp_len > 0)
        {
            for(timei=0;timei<20000;timei++);		        //��ʱ�ȴ����ݽ���
            if(temp_len == USART2_Read_Data_Len())
            {
                USART2_Read_Data_Array(temp);
                Modbus_Reback_Package(temp,Modbus_Buff,temp_len);
                USART3_Write_Data_Array(Modbus_Buff,temp_len+3);
            }
        }
		
    }
}
