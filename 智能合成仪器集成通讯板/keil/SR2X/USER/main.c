/*****************************************************************************
�ļ�����: main.c
�ļ�����: ������������CAN1�ڷ��͵����ݽ���Э�黯
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

/*****************************************************************************
��������: Edit_CAN_Send_pack
��������: ����CAN1�ڷ��͵����ݽ���Э�黯
���ú���: CAN1_Write_Data_Array ��CAN1��������
����������main
�������: 
					uint8_t * Send_Data   ��Ҫͨ��CAN1�ڷ��͵�����
					uint8_t Usart_Num			����һ��USART�ڷ��͵�
					int8_t Data_Len				��Ҫ���͵����ݳ���
�� �� ֵ: ��
��    ��: ��
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
					Edit_CAN_Send_pack();       �����ݽ��д����ͨ��CAN1���ͳ�ȥ

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

int main()
{	
	uint16_t timei;				//��ʱ����
	uint8_t temp[200];		//��ʱ���ݻ���
	uint8_t temp_len;			//��ʱ���ݳ���

	SystemInit();					//ϵͳʱ�ӳ�ʼ��
	                      
	USART1_Init();        //USART1��ʼ��
	USART2_Init();        //USART2��ʼ��
	USART3_Init();        //USART3��ʼ��
	
	CAN1_Init();         //��ΪCAN�Ĺܽź�USART1�ܽŸ�������  ���뽫CAN1 �ĳ�ʼ���ŵ� USART1����
	
	while (1)																										//��ʼ��ѭ��
	{
		temp_len = CAN1_Read_Data_Len();													//��ȡCAN1�������ݳ���
		if(temp_len>0)																						//���CAN1��������
		{
			for(timei=0;timei<10000;timei++);												//��ʱ�ȴ����ݽ���
			if(temp_len ==CAN1_Read_Data_Len())										  //���ݽ�����ɣ��������ݴ���
			{
				CAN1_Read_Data_Array(temp);														//��ȡCAN1����������
				if(temp[0] != 0xAA||temp[1] != 0xBB)									//�жϱ���ͷ
					continue;
				if(temp[2] != temp_len-5)															//�ж����ݳ���
					continue;
				switch(temp[3])																				//�ж���Ҫת���Ĵ��ںţ����͵���Ӧ�Ĵ���
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
	
		temp_len = USART1_Read_Data_Len();												//��ȡUSART1�������ݳ���
		if(temp_len>0)                                          	//���USART1��������
		{                                                       
			for(timei=0;timei<10000;timei++);	                    	//��ʱ�ȴ����ݽ���
			if(temp_len ==USART1_Read_Data_Len() )                	//���ݽ�����ɣ��������ݴ���
			{
				USART1_Read_Data_Array(temp);													//��ȡUSART1����������
				Edit_CAN_Send_pack(temp,UART1_ID,temp_len);						//�����ݽ��д����ͨ��CAN1���ͳ�ȥ
			}	
		}
		
		temp_len = USART2_Read_Data_Len();												//��ȡUSART2�������ݳ���
		if(temp_len>0)                                            //���USART2��������
		{                                                         
			for(timei=0;timei<10000;timei++);	                      //��ʱ�ȴ����ݽ���
			if(temp_len ==USART2_Read_Data_Len() )                  //���ݽ�����ɣ��������ݴ���
			{                                                       
				 USART2_Read_Data_Array(temp);                        //��ȡUSART2����������
				 Edit_CAN_Send_pack(temp,UART2_ID,temp_len);          //�����ݽ��д����ͨ��CAN1���ͳ�ȥ
			}
		}
		
		temp_len = USART3_Read_Data_Len();												//��ȡUSART3�������ݳ���
		if(temp_len>0)                                            //���USART3��������
		{                                                         
			for(timei=0;timei<10000;timei++);	                      //��ʱ�ȴ����ݽ���
			if(temp_len ==USART3_Read_Data_Len())                   //���ݽ�����ɣ��������ݴ���
			{                                                       
				USART3_Read_Data_Array(temp);                         //��ȡUSART3����������
				Edit_CAN_Send_pack(temp,UART3_ID,temp_len);           //�����ݽ��д����ͨ��CAN1���ͳ�ȥ
			}	 
		}
  }
}
