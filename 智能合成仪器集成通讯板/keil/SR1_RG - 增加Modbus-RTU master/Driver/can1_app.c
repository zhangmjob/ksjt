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

	/*******************����CAN1������ֻ������������**********************************/
	
	CAN_FilterInitStructure.CAN_FilterNumber=0;                     //ָ��������Ϊ1
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;   //ָ��������Ϊ��ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;  //������λ��Ϊ32λ
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;                //��������ʶ���ĸ�16λֵ
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;									//��������ʶ���ĵ�16λֵ
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;            //���������α�ʶ���ĸ�16λֵ
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;             //���������α�ʶ���ĵ�16λֵ
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;     //�趨��ָ���������FIFOΪ0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;            //ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);                       //������Ĳ�����ʼ��������

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

	TxMessage.StdId=0x0100;      											//������׼��ʶ��Ϊ�̶�ֵ0x0100
	TxMessage.IDE=CAN_ID_STD;   											//ʹ�ñ�׼��ʶ��
	TxMessage.RTR=CAN_RTR_DATA; 											//Ϊ����֡
	
	/***********************�����ݽ��з��췢��*******************************************/
	datenum = WriteDataLen/8;													//�ж���Ҫ���ͼ���
	if(WriteDataLen%8!=0)datenum++;
	for(i=0;i< datenum;i++)														//ѭ�����췢��
	{
		if(i<datenum-1){
			onedatalen = 8;
        }else{
			onedatalen = WriteDataLen-8*(datenum-1);
		}
        
		TxMessage.DLC=onedatalen; 	//���ñ������ݳ���
		for(j=0;j<onedatalen;j++){
			TxMessage.Data[j] = WriteDataArray[8*i+j]; 	//���뱾������
		}

        mbox= CAN_Transmit(CAN1,&TxMessage);  							//��������׼������
        while(CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed);		//�ȴ����ͽ���
        
		if((i+1)%3==0){//ÿ��������ȴ�����
			for(timei=0;timei<10000;timei++);	
        }            
	}

}


/*****************************************************************************
��������: CAN1_READ_DATA_Buffer
��������: CAN1�������ݻ�����
��    ��: ��Ϊ�������ĸ����豸����������������
*****************************************************************************/
uint8_t CAN1_READ_SR2x_DATA_Buffer[4][200];
/*****************************************************************************
��������: CAN1_READ_DATA_Len
��������: CAN1�������ݳ�����
��    ��: ��Ϊ�������ĸ����豸����������������
*****************************************************************************/
uint8_t CAN1_READ_SR2x_DATA_Len[4];

/*****************************************************************************
��������: CAN1_Read_Data_Len
��������: ��ȡCAN1�������ж�Ӧ���豸�����ݳ���
���ú���: ��
����������main
�������: SR_Num ���豸��ʶ�� ȡֵ(0,1,2,3)
�� �� ֵ: ��������Ӧ���豸���ݳ���
��    ��: ��
*****************************************************************************/
uint8_t CAN1_Read_Data_Len(uint8_t SR_Num)
{
	if(SR_Num>3){
        return 0; //�ж�SR_Num�Ƿ�Ϸ� ���Ϸ�����
    }else{     
        return CAN1_READ_SR2x_DATA_Len[SR_Num];					//���ػ�������ӦSR_Num�����ݳ���
    }
}

/*****************************************************************************
��������: CAN1_Read_Data_Array
��������: ��CAN1��������ȡ��Ӧ���豸������ ����ն�Ӧ�Ļ�����
���ú���: ��
����������main
�������: 
					ReadDataArray	 �����������ݵĵ�ַ
					SR_Num ���豸��ʶ�� ȡֵ(0,1,2,3)
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/
void CAN1_Read_Data_Array(uint8_t * ReadDataArray,uint8_t SR_Num)
{
	uint8_t i;
	if(SR_Num>3){ //�ж�SR_Num�Ƿ�Ϸ� ���Ϸ�����
        return;
    }       
	for(i=0;i<CAN1_READ_SR2x_DATA_Len[SR_Num];i++)
	{
		ReadDataArray[i] = CAN1_READ_SR2x_DATA_Buffer[SR_Num][i]; //����������ӦSR_Num������ѭ�����뱣�����ݵ�ַ
	}
	CAN1_READ_SR2x_DATA_Len[SR_Num] = 0;						  //���SR_Num��Ӧ�Ļ�����
	
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
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage); 		//����FIFO0�е�����  

	for(i=0;i<RxMessage.DLC;i++)
	{
		switch(RxMessage.StdId)										//ͨ����׼��ʶ���ж�������һ̨���豸��������
		{
			case 0x0101:
				CAN1_READ_SR2x_DATA_Buffer[0][CAN1_READ_SR2x_DATA_Len[0]++]=RxMessage.Data[i];		//ѭ�������ݷ����Ӧ�Ļ����������ȼ�1
				break;
			case 0x0102:
				CAN1_READ_SR2x_DATA_Buffer[1][CAN1_READ_SR2x_DATA_Len[1]++]=RxMessage.Data[i];
				break;
			case 0x0103:
				CAN1_READ_SR2x_DATA_Buffer[2][CAN1_READ_SR2x_DATA_Len[2]++]=RxMessage.Data[i];
				break;
			case 0x0104:
				CAN1_READ_SR2x_DATA_Buffer[3][CAN1_READ_SR2x_DATA_Len[3]++]=RxMessage.Data[i];
				break;
			default:break;
		}
	}
	//CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
} 
