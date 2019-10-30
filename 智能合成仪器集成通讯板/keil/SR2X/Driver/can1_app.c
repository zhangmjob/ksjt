/*****************************************************************************
�ļ�����: can1_app.c
�ļ�����: ������������CAN1�ڷ��͵����ݽ���Э�黯
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#include "can1_app.h"

void CAN1_Init(void)
{
	GPIO_InitTypeDef 			GPIO_InitStructure; 
	NVIC_InitTypeDef 			NVIC_InitStructure;
	CAN_InitTypeDef				CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;

	/***********************��ʱ��*******************************************/  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 				//��GPIOAʱ��   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 					//��CAN1��ʱ��

	/***********************���ùܽ�*******************************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       					//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//��ʼ��PA11

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     					//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//��ʼ��PA12
	
	/***********************����CAN1*******************************************/

	CAN_InitStructure.CAN_TTCM=DISABLE;         									//û��ʹ��ʱ�䴥��ģʽ
	CAN_InitStructure.CAN_ABOM=DISABLE;         									//û��ʹ���Զ����߹���
	CAN_InitStructure.CAN_AWUM=DISABLE;         									//û��ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART=DISABLE;         									//û��ʹ�ܷ��Զ��ش�ģʽ
	CAN_InitStructure.CAN_RFLM=DISABLE;         									//û��ʹ�ܽ���FIFO����ģʽ
	CAN_InitStructure.CAN_TXFP=DISABLE;         									//û��ʹ�ܷ���FIFO���ȼ�
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal; 									//CAN����Ϊ����ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;      									//����ͬ����Ծ���1��ʱ�䵥λ
	CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;      									//ʱ���1Ϊ3��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;      									//ʱ���2Ϊ2��ʱ�䵥λ
	CAN_InitStructure.CAN_Prescaler=6;         										//ʱ�䵥λ����Ϊ6  1MBPS
	CAN_Init(CAN1,&CAN_InitStructure);          									//������Ϊ��72M/2/6(1+3+2)=1 ��������Ϊ1MBPs

	/*******************����CAN1������ֻ������������(0x0100)**********************************/
	
	CAN_FilterInitStructure.CAN_FilterNumber=0;                     //ָ��������Ϊ0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;   //ָ��������Ϊ��ʶ���б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ32λ
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0100<<5;             //�������б�1��16λ
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;                 //�������б�1��16λ
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0100<<5;         //�������б�2��16λ
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;             //�������б�2��16λ
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;     //�趨��ָ���������FIFOΪ0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;            //ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);                       //��ʼ��������

	/* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);                         //ʹ��FIFO0��Ϣ�Һ��ж�

	/***********************�����ж�*******************************************/  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);									//ѡ�����0  
	                                                                
	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;        //CAN1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //������0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //ʹ��CAN1�ж�
	NVIC_Init(&NVIC_InitStructure);                                 //��NVIC��ʼ��
}

/*****************************************************************************
��������: CAN1_Write_Data_Array
��������: ��CAN1��������
���ú���: STM32�̼��⺯��
����������main
�������: 
					WriteDataArray			��Ҫ���͵�����
					WriteDataLen				��Ҫ���͵����ݳ���
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/
void CAN1_Write_Data_Array(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{ 
	uint8_t mbox;
	uint8_t i,j;
	uint8_t onedatalen,datenum; 											//һ�����ݳ��ȣ�������
	uint16_t timei;																		//�����Ժ����ʱ����
	CanTxMsg TxMessage;                         			//CAN���Ͱ�

	TxMessage.StdId=CAN1_STDID;      									//��׼��ʶ��ΪCAN1_STDID
	TxMessage.IDE=CAN_ID_STD;   											//ʹ�ñ�׼��ʶ��
	TxMessage.RTR=CAN_RTR_DATA; 											//Ϊ����֡
	
	/***********************�����ݽ��з��췢��*******************************************/
	datenum = WriteDataLen/8;													//�ж���Ҫ���ͼ���
	if(WriteDataLen%8!=0)datenum++;
	for(i=0;i< datenum;i++)														//ѭ�����췢��
	{
		if(i<datenum-1)
			onedatalen = 8;
		else
		{
			onedatalen = WriteDataLen-8*(datenum-1);
		}
		TxMessage.DLC=onedatalen; 										//���ñ������ݳ���
		for(j=0;j<onedatalen;j++)
		{
			TxMessage.Data[j] = WriteDataArray[8*i+j]; 	//���뱾������
		}

    mbox= CAN_Transmit(CAN1, &TxMessage);  														//��������׼������
    while(CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed);				//�ȴ����ͽ���
		if((i+1)%3==0)
			for(timei=0;timei<10000;timei++);																//ÿ��������ȴ�����
	}

}


/*****************************************************************************
��������: CAN1_READ_DATA_Buffer
��������: CAN1�������ݻ���
��    ��: ��
*****************************************************************************/
uint8_t CAN1_READ_DATA_Buffer[200];

/*****************************************************************************
��������: CAN1_READ_DATA_Len
��������: CAN1�������ݳ���
��    ��: ��
*****************************************************************************/
uint8_t CAN1_READ_DATA_Len = 0;

/*****************************************************************************
��������: CAN1_Read_Data_Len
��������: ��ȡCAN1���������ݳ���
���ú���: ��
����������main
�������: ��
�� �� ֵ: ���������ݳ���
��    ��: ��
*****************************************************************************/
uint8_t CAN1_Read_Data_Len()
{
	return CAN1_READ_DATA_Len;															//�������ݳ���
}


/*****************************************************************************
��������: CAN1_Read_Data_Array
��������: ��CAN1��������ȡ���� ����ջ�����
���ú���: ��
����������main
�������: ReadDataArray	 �����������ݵĵ�ַ
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/
void CAN1_Read_Data_Array(uint8_t * ReadDataArray)
{
	uint8_t i;
	for(i=0;i<CAN1_READ_DATA_Len;i++)
	{
		ReadDataArray[i] = CAN1_READ_DATA_Buffer[i];				//������������ѭ�����뱣�����ݵ�ַ
	}	
	CAN1_READ_DATA_Len = 0;
	
}

/*****************************************************************************
��������: USB_LP_CAN1_RX0_IRQHandler
��������: USB�жϺ�CAN�����жϷ������
���ú���: ��
������������
�������: ��
�� �� ֵ: ��
��    ��: ����ʹ����PA11��PA12,USB��CAN����I/O�� 
*****************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  uint8_t i;
  CanRxMsg RxMessage;
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage); 														//����FIFO0�е����� 
	
	for(i=0;i<RxMessage.DLC;i++)
	{
		CAN1_READ_DATA_Buffer[CAN1_READ_DATA_Len++]=RxMessage.Data[i];   //ѭ�������ݷ��뻺���������ȼ�1
	}
	//CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
} 
