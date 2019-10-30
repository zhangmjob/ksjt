/*****************************************************************************
�ļ�����: main.c
�ļ�����: ������������CAN1�ڷ��͵����ݽ���Э�黯   ���ͨ��IO���Զ�ʶ�����ĸ�оƬ
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.1
��    ��: 20190322
��ʷ�汾: 1.0
*****************************************************************************/
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
#include "gpio_app.h"
#include "can1_app.h"


uint16_t CAN1_STDID = 0x0101;
uint8_t  UART1_ID = 0x01;
uint8_t  UART2_ID = 0x0A;					
uint8_t  UART3_ID = 0x0B;			


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
	can_send_data[Data_Len+7] = 0xFF;	
	
	CAN1_Write_Data_Array(CAN1_STDID,can_send_data,Data_Len+8);
}

void Work_LED_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	/***********************��ʱ��*******************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				//��GPIOAʱ��
	
	/***********************���ùܽ�*******************************************/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							  //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//��ʼ��PA4
    
    GPIO_ResetBits(GPIOA,GPIO_Pin_4);                   //��������״̬LED��
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
    uint8_t test[6] = {0x00,0x11,0x22,0x33,0x44,0x55};
    
	uint16_t timei;				//��ʱ����
	uint8_t temp[200];		//��ʱ���ݻ���
	uint8_t temp_len;			//��ʱ���ݳ���

	SystemInit();					//ϵͳʱ�ӳ�ʼ��
	
	GPIO_App_Init();			//��ʼ������IO��
    Work_LED_Init();
	                      
	USART1_Init();        //USART1��ʼ��
	USART2_Init();        //USART2��ʼ��
	USART3_Init();        //USART3��ʼ��
	
	CAN1_Init();         //��ΪCAN�Ĺܽź�USART1�ܽŸ�������  ���뽫CAN1 �ĳ�ʼ���ŵ� USART1����
	
	switch(Get_GPIO_Input_Value())															//ͨ��IO������״̬ʶ������һ��оƬ
	{
		case 0:
			CAN1_STDID = 0x0101;
            UART1_ID = 0x01;
            UART2_ID = 0x0A;
			UART3_ID = 0x0B;
			break;
		case 1:
			CAN1_STDID = 0x0102;
            UART1_ID = 0x04;
            UART2_ID = 0x02;
			UART3_ID = 0x03;
            break;
		case 2:
			CAN1_STDID = 0x0103;
            UART1_ID = 0x07;
            UART2_ID = 0x05;
			UART3_ID = 0x06;
			break;
		case 3:
			CAN1_STDID = 0x0104;
            UART1_ID = 0x08;
            UART2_ID = 0x0C;
			UART3_ID = 0x0D;
			break;
		default:
			break;
	}
	
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
				if(temp[3]== UART1_ID)																//�ж���Ҫת���Ĵ��ںţ����͵���Ӧ�Ĵ���
					USART1_Write_Data_Array(temp+5,temp_len-8);
				else if(temp[3]== UART2_ID)
					USART2_Write_Data_Array(temp+5,temp_len-8);
				else if(temp[3]== UART3_ID)
					USART3_Write_Data_Array(temp+5,temp_len-8);
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
                USART1_Write_Data_Array(test,6);
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
