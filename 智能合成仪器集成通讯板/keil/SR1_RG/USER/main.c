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

int main()
{	
	uint16_t timei;			//��ʱ����
	uint8_t temp_num;		//��ʱSRxѡ�����
	uint8_t temp[200];	//��ʱ���ݻ���
	uint8_t temp_len;		//��ʱ���ݳ���
	
	SystemInit();				//ϵͳʱ�ӳ�ʼ��

	USART1_Init();			//USART1��ʼ��
	USART2_Init();			//USART2��ʼ��
	USART3_Init();			//USART3��ʼ��
	
	CAN1_Init();				//��ΪCAN�Ĺܽź�USART1�ܽŸ�������  ���뽫CAN1 �ĳ�ʼ���ŵ� USART1����
	
	printf("system start ... \r\n");
	while (1)
	{
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
				
					printf("read can data:");									//���������Ϣ
					UART1_WriteHEX(temp,temp_len);            //���������Ϣ
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
				CAN1_Write_Data_Array(temp,temp_len);       //������ͨ��CAN1���ͳ�ȥ
				
				printf("read usart3 data:");								//���������Ϣ
				UART1_WriteHEX(temp,temp_len);							//���������Ϣ
			}	
		}			
  }
}
