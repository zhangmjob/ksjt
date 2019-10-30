#include "rs485_usart2_app.h"
#include "systick.h"


uint8_t USART2_Read_Data_Buffer[64];                   //���ջ��壬���64�ֽ�
uint8_t USART2_Read_Data_Len=0;                       //�����ֽڼ�����


void RS485_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOA,ENABLE); //ʹ��GPIOA\Gʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;	//TX-485	//�������PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);		/* ��ʼ����������IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//RX-485	   //��������PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ��� 2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���������ж�

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	//RS485_TX_EN=0;				//Ĭ��Ϊ����ģʽ	
}
									
		
/*******************************************************************************
* �� �� ��         : USART2_IRQHandler
* ��������		   : USART2�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
void USART2_IRQHandler(void)
{
	uint8_t res;	                                    //�������ݻ������
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{	 	
		res =USART_ReceiveData(USART2);//;��ȡ���յ�������USART2->DR
		if(USART2_Read_Data_Len<sizeof(USART2_Read_Data_Buffer))    //һ��ֻ�ܽ���64���ֽڣ���Ϊ�趨�����Ը��󣬵��˷�ʱ��
		{
			USART2_Read_Data_Buffer[USART2_Read_Data_Len]=res;  //��¼���յ���ֵ
			USART2_Read_Data_Len++;        //ʹ����������1 
		}
	}
}

//���ؽ��ܵ������ݳ���
uint8_t RS485_READ_DATA_LEN()
{
    return USART2_Read_Data_Len;
}

//�Ӵ��ڶ�ȡ����
//ReadDataArray�����������ַ������׵�ַ
void RS485_READ_DATA_ARRAY(uint8_t * ReadDataArray)
{
	uint8_t i;
	for(i=0;i<USART2_Read_Data_Len;i++)
	{
		ReadDataArray[i] = USART2_Read_Data_Buffer[i];		//������������ѭ�����뱣�����ݵ�ַ
	}
	USART2_Read_Data_Len = 0;	
}

//�򴮿�д������
//WriteDataArray��Ҫд�����ݵ��׵�ַ
//WriteDataLen��д�����ݵ��ֽڳ���
void RS485_WRITE_DATA_ARRAY(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{	
	uint8_t i;	
	for(i=0;i<WriteDataLen;i++)
	{
		USART_SendData(USART2, WriteDataArray[i]);										//��USART2��������
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);   //�ȴ�USART2���ݷ������
	}
}
