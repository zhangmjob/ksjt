/*****************************************************************************
�ļ�����: usart2_app.c
�ļ�����: USART2�����ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#include "usart2_app.h"

/*****************************************************************************
��������: USART2_Init
��������: USART2ʼʼ��
���ú���: STM32�̼��⺯��
����������main
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/
void USART2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;  	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/***********************��ʱ��*******************************************/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);			//��USART2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				//��GPIOAʱ��
	
	/***********************���ùܽ�*******************************************/
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 							//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//��ʼ��PA2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				//��©����
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//��ʼ��PA3

	
	/***********************���ô���*******************************************/
	
	USART_InitStructure.USART_BaudRate = 9600;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    		//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;   				//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 								//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);	
	
	USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);									//��UART1�ж�
	USART_Cmd(USART2, ENABLE);																		//����USART2
	USART_GetFlagStatus(USART2,USART_FLAG_TC);										//���״̬ �����һ���ַ���ʧ
	
	/***********************�����ж�*******************************************/
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);								//ѡ�����0  
	
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;							//USART2�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						//������2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ��USART2�ж�
    NVIC_Init(&NVIC_InitStructure);																//��NVIC��ʼ��
}

/*****************************************************************************
��������: USART2_Write_Data_Array
��������: ��USART2��������
���ú���: STM32�̼��⺯��
����������main
�������: 
					WriteDataArray			��Ҫ���͵�����
					WriteDataLen				��Ҫ���͵����ݳ���
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART2_Write_Data_Array(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{	
	uint8_t i;	
	for(i=0;i<WriteDataLen;i++)
	{
		USART_SendData(USART2, WriteDataArray[i]);										//��USART2��������
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);   //�ȴ�USART2���ݷ������
	}
}

/*****************************************************************************
��������: USART2_READ_DATA_Buffer
��������: USART2�������ݻ���
��    ��: ��
*****************************************************************************/
uint8_t USART2_READ_DATA_Buffer[200];

/*****************************************************************************
��������: USART2_READ_DATA_Len
��������: USART2�������ݳ���
��    ��: ��
*****************************************************************************/
uint8_t USART2_READ_DATA_Len = 0;

/*****************************************************************************
��������: USART2_IRQHandler
��������: USART2�жϷ���
���ú���: ��
������������
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART2_IRQHandler()
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)														//����Ĵ�����������
  {		
    USART2_READ_DATA_Buffer[USART2_READ_DATA_Len++] = USART_ReceiveData(USART2);	//�����ݷ��뻺���������ȼ�1
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 																//��ս����жϱ�־λ
  }	
}

/*****************************************************************************
��������: USART2_Read_Data_Len
��������: ��ȡUSART2���������ݳ���
���ú���: ��
����������main
�������: ��
�� �� ֵ: ���������ݳ���
��    ��: ��
*****************************************************************************/

uint8_t USART2_Read_Data_Len()
{
	return USART2_READ_DATA_Len;													//�������ݳ���																						
}


/*****************************************************************************
��������: USART2_Read_Data_Array
��������: ��USART2��������ȡ���� ����ջ�����
���ú���: ��
����������main
�������: ReadDataArray	 �����������ݵĵ�ַ
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

void USART2_Read_Data_Array(uint8_t * ReadDataArray)
{
	uint8_t i;
	for(i=0;i<USART2_READ_DATA_Len;i++)
	{
		ReadDataArray[i] = USART2_READ_DATA_Buffer[i];		//������������ѭ�����뱣�����ݵ�ַ
	}
	USART2_READ_DATA_Len = 0;	
}
