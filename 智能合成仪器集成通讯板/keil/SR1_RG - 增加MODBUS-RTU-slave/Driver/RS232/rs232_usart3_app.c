/*****************************************************************************
�ļ�����: usart3_app.c
�ļ�����: USART3�����ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#include "rs232_usart3_app.h"

/*****************************************************************************
��������: USART3_Init
��������: USART3ʼʼ��
���ú���: STM32�̼��⺯��
����������main
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/
void USART3_Init(uint32_t bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;  	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/***********************��ʱ��*******************************************/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);			//��USART3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);				//��GPIOBʱ��
	
	/***********************���ùܽ�*******************************************/
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 							//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);												//��ʼ��PB10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				//��©����
	GPIO_Init(GPIOB, &GPIO_InitStructure);												//��ʼ��PB11

	
	/***********************���ô���*******************************************/
	
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    		//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;   				//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 								//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);	
	
	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);									//��UART1�ж�
	USART_Cmd(USART3, ENABLE);																		//����USART3
	USART_GetFlagStatus(USART3,USART_FLAG_TC);										//���״̬ �����һ���ַ���ʧ
	
	/***********************�����ж�*******************************************/
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);								//ѡ�����0  
	
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;							//USART3�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;						//������3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ��USART3�ж�
  NVIC_Init(&NVIC_InitStructure);																//��NVIC��ʼ��
}

/*****************************************************************************
��������: USART3_Write_Data_Array
��������: ��USART3��������
���ú���: STM32�̼��⺯��
����������main
�������: 
					WriteDataArray			��Ҫ���͵�����
					WriteDataLen				��Ҫ���͵����ݳ���
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART3_Write_Data_Array(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{	
	uint8_t i;	
	for(i=0;i<WriteDataLen;i++)
	{
		USART_SendData(USART3, WriteDataArray[i]);										//��USART3��������
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   //�ȴ�USART3���ݷ������
	}
}

/*****************************************************************************
��������: USART3_READ_DATA_Buffer
��������: USART3�������ݻ���
��    ��: ��
*****************************************************************************/
uint8_t USART3_READ_DATA_Buffer[200];

/*****************************************************************************
��������: USART3_READ_DATA_Len
��������: USART3�������ݳ���
��    ��: ��
*****************************************************************************/
uint8_t USART3_READ_DATA_Len = 0;

/*****************************************************************************
��������: USART3_IRQHandler
��������: USART3�жϷ���
���ú���: ��
������������
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART3_IRQHandler()
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)														//����Ĵ�����������
  {		
    USART3_READ_DATA_Buffer[USART3_READ_DATA_Len++] = USART_ReceiveData(USART3);	//�����ݷ��뻺���������ȼ�1
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE); 																//��ս����жϱ�־λ
  }	
}

/*****************************************************************************
��������: USART3_Read_Data_Len
��������: ��ȡUSART3���������ݳ���
���ú���: ��
����������main
�������: ��
�� �� ֵ: ���������ݳ���
��    ��: ��
*****************************************************************************/

uint8_t USART3_Read_Data_Len()
{
	return USART3_READ_DATA_Len;													//�������ݳ���																						
}


/*****************************************************************************
��������: USART3_Read_Data_Array
��������: ��USART3��������ȡ���� ����ջ�����
���ú���: ��
����������main
�������: ReadDataArray	 �����������ݵĵ�ַ
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART3_Read_Data_Array(uint8_t * ReadDataArray)
{
	uint8_t i;
	for(i=0;i<USART3_READ_DATA_Len;i++)
	{
		ReadDataArray[i] = USART3_READ_DATA_Buffer[i];		//������������ѭ�����뱣�����ݵ�ַ
	}
	USART3_READ_DATA_Len = 0;	
}
