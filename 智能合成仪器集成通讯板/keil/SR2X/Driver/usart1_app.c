/*****************************************************************************
�ļ�����: usart1_app.c
�ļ�����: usart1�����ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#include "usart1_app.h"

/*****************************************************************************
��������: USART1_Init
��������: USART1ʼʼ��
���ú���: STM32�̼��⺯��
����������main
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/
void USART1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;  	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/***********************��ʱ��*******************************************/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);			//��USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				//��GPIOAʱ��
	
	/***********************���ùܽ�*******************************************/
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 							//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//��ʼ��PA9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				//��©����
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//��ʼ��PA10

	
	/***********************���ô���*******************************************/
	
	USART_InitStructure.USART_BaudRate = 9600;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    		//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;   				//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 								//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);	
	
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);									//��UART1�ж�
	USART_Cmd(USART1, ENABLE);																		//����USART1
	USART_GetFlagStatus(USART1,USART_FLAG_TC);										//���״̬ �����һ���ַ���ʧ
	
	/***********************�����ж�*******************************************/
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);								//ѡ�����0  
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;							//USART1�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;						//������1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ��USART1�ж�
  NVIC_Init(&NVIC_InitStructure);																//��NVIC��ʼ��
}

/*****************************************************************************
��������: USART1_Write_Data_Array
��������: ��USART1��������
���ú���: STM32�̼��⺯��
����������main
�������: 
					WriteDataArray			��Ҫ���͵�����
					WriteDataLen				��Ҫ���͵����ݳ���
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART1_Write_Data_Array(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{	
	uint8_t i;	
	for(i=0;i<WriteDataLen;i++)
	{
		USART_SendData(USART1, WriteDataArray[i]);										//��USART1��������
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   //�ȴ�USART1���ݷ������
	}
}

/*****************************************************************************
��������: USART1_READ_DATA_Buffer
��������: USART1�������ݻ���
��    ��: ��
*****************************************************************************/
uint8_t USART1_READ_DATA_Buffer[200];

/*****************************************************************************
��������: USART1_READ_DATA_Len
��������: USART1�������ݳ���
��    ��: ��
*****************************************************************************/
uint8_t USART1_READ_DATA_Len = 0;

/*****************************************************************************
��������: USART1_IRQHandler
��������: USART1�жϷ���
���ú���: ��
������������
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART1_IRQHandler()
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)														//����Ĵ�����������
  {		
    USART1_READ_DATA_Buffer[USART1_READ_DATA_Len++] = USART_ReceiveData(USART1);	//�����ݷ��뻺���������ȼ�1
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 																//��ս����жϱ�־λ
  }	
}

/*****************************************************************************
��������: USART1_Read_Data_Len
��������: ��ȡUSART1���������ݳ���
���ú���: ��
����������main
�������: ��
�� �� ֵ: ���������ݳ���
��    ��: ��
*****************************************************************************/

uint8_t USART1_Read_Data_Len()
{
	return USART1_READ_DATA_Len;													//�������ݳ���																						
}


/*****************************************************************************
��������: USART1_Read_Data_Array
��������: ��USART1��������ȡ���� ����ջ�����
���ú���: ��
����������main
�������: ReadDataArray	 �����������ݵĵ�ַ
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART1_Read_Data_Array(uint8_t * ReadDataArray)
{
	uint8_t i;
	for(i=0;i<USART1_READ_DATA_Len;i++)
	{
		ReadDataArray[i] = USART1_READ_DATA_Buffer[i];		//������������ѭ�����뱣�����ݵ�ַ
	}
	USART1_READ_DATA_Len = 0;	
}
