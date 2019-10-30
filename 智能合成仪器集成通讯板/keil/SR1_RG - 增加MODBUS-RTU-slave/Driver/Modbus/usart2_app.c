#include "usart2_app.h"
#include "systick.h"
#include "crc16.h"


uint8_t USART2_Read_Data_Buffer[64];                   //���ջ��壬���64�ֽ�
uint8_t USART2_Read_Data_Len=0;                       //�����ֽڼ�����
//uint8_t flagFrame=0;                         //֡������ɱ�־�������յ�һ֡������
//uint8_t regGroup[5];  //Modbus�Ĵ����飬��ַΪ0x00~0x04


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

	
/*
//1ms��ʱ
void TIM2_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=1000;   //�Զ�װ��ֵ 
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��	
}
*/

/*
//���㷢�͵����ݳ��ȣ����ҽ����ݷŵ�*buf������                     
uint8_t UartRead(uint8_t *buf, uint8_t len)  
{
	 uint8_t i;
	if(len>USART2_RX_CNT)  //ָ����ȡ���ȴ���ʵ�ʽ��յ������ݳ���ʱ
	{
		len=USART2_RX_CNT; //��ȡ��������Ϊʵ�ʽ��յ������ݳ���
	}
	for(i=0;i<len;i++)  //�������յ������ݵ�����ָ����
	{
		*buf=USART2_RX_BUF[i];  //�����ݸ��Ƶ�buf��
		buf++;
	}
	USART2_RX_CNT=0;              //���ռ���������
	return len;                   //����ʵ�ʶ�ȡ����
}
*/

/*
void rs485_UartWrite(uint8_t *buf ,uint8_t len) 										//����
{
	uint8_t i=0; 
    GPIO_SetBits(GPIOG,GPIO_Pin_3);                                            //����ģʽ
    delay_ms(3);                                                               //3MS��ʱ
    for(i=0;i<=len;i++)
    {
	USART_SendData(USART2,buf[i]);	                                      //ͨ��USARTx���跢�͵�������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);             //���ָ����USART��־λ������񣬷������ݿ�λ��־
    }
    GPIO_ResetBits(GPIOG,GPIO_Pin_3);                                         //����Ϊ����ģʽ

}
*/

/*
//���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е���
void UartDriver()
{
	unsigned char i=0,cnt;
	unsigned int crc;
	unsigned char crch,crcl;
	static uint8_t len;
	static uint8_t buf[60];
	if(flagFrame)            //֡������ɱ�־�������յ�һ֡������
	{
		flagFrame=0;           //֡������ɱ�־����
		len = UartRead(buf,sizeof(buf));   //�����յ������������������
		if(buf[0]==0x01)                   //�жϵ�ַ�ǲ���0x01
		{
			crc=GetCRC16(buf,len-2);       //����CRCУ��ֵ����ȥCRCУ��ֵ
			crch=crc>>8;    				//crc��λ
			crcl=crc&0xFF;					//crc��λ
			if((buf[len-2]==crch)&&(buf[len-1]==crcl))  //�ж�CRCУ���Ƿ���ȷ
			{
				switch (buf[1])  //��������ִ�в���
				{
					case 0x03:     //������
						if((buf[2]==0x00)&&(buf[3]<=0x05))  //�Ĵ�����ַ֧��0x0000~0x0005
						{
							
							if(buf[3]<=0x04) 
							{
								i=buf[3];//��ȡ�Ĵ�����ַ
								cnt=buf[5];  //��ȡ����ȡ�ļĴ�������
								buf[2]=cnt*2;  //��ȡ���ݵ��ֽ�����Ϊ�Ĵ���*2����modbus����ļĴ���Ϊ16λ
								len=3;							
								while(cnt--)
								{
									buf[len++]=0x00;				//�Ĵ������ֽڲ�0
									buf[len++]=regGroup[i++];		//���ֽ�
							}
							
						}
							break;
					}
						else  //�Ĵ�����ַ����֧��ʱ�����ش�����
						{   
							buf[1]=0x83;  //���������λ��1
							buf[2]=0x02;  //�����쳣��Ϊ02-��Ч��ַ
							len=3;
							break;
						}
					case 0x06:           //д�뵥���Ĵ���
						if((buf[2]==0x00)&&(buf[3]<=0x05))   //�Ĵ�����ַ֧��0x0000-0x0005
						{
							if(buf[3]<=0x04)
							{
								i=buf[3];				//��ȡ�Ĵ�����ַ
								regGroup[i]=buf[5];		//����Ĵ�������

							}
							len -=2;                 //����-2�����¼���CRC������ԭ֡
							break;
						}
						else  
						{							//�Ĵ�����ַ����֧�֣����ش�����
							buf[1]=0x86;           //���������λ��1
							buf[2]=0x02;           //�����쳣��Ϊ02-��Ч��ַ
							len=3;
							break;
					}
					default:    //������֧�ֵĹ�����
						    buf[1]=0x80;     //���������λ��1
							buf[2]=0x01;     //�����쳣��Ϊ01����Ч����
							len=3;
							break;
				}
			    crc=GetCRC16(buf,len);           //����CRCУ��ֵ
				buf[len++]=crc>>8;           //CRC���ֽ�
				buf[len++]=crc&0xff;        //CRC���ֽ�
				rs485_UartWrite(buf,len);  //������Ӧ֡
			}
		}
	}
}
*/

/*				
void UartRxMonitor(uint8_t ms) //���ڽ��ռ��
{
	static uint8_t USART2_RX_BKP=0;  //����USART2_RC_BKP��ʱ�洢ʫ�䳤����ʵ�ʳ��ȱȽ�
	static uint8_t idletmr=0;        //������ʱ��
	if(USART2_RX_CNT>0)//���ռ�����������ʱ��������߿���ʱ��
	{
		if(USART2_RX_BKP!=USART2_RX_CNT) //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
		{
			USART2_RX_BKP=USART2_RX_CNT;  //��ֵ��������ʵ�ʳ��ȸ�USART2_RX_BKP
			idletmr=0;                    //�����ʱ������
		}
		else                              ////���ռ�����δ�ı䣬�����߿���ʱ���ۼƿ���ʱ��
		{
			//�����һ֡�������֮ǰ�г���3.5���ֽ�ʱ���ͣ�٣������豸��ˢ�µ�ǰ����Ϣ���ٶ���һ���ֽ���һ���µ�����֡�Ŀ�ʼ
			if(idletmr<5)                  //����ʱ��С��1msʱ�������ۼ�
			{
				idletmr +=ms;
				if(idletmr>=5)             //����ʱ��ﵽ1msʱ�����ж�Ϊ1֡�������
				{
					flagFrame=1;//����������־��֡������ϱ�־
				}
			}
		}
	}
	else
	{
		USART2_RX_BKP=0;
	}
}
*/										
		
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

			 
/*******************************************************************************
* �� �� ��         : TIM2_IRQHandler
* ��������		   : TIM2�жϺ���
* ��    ��         : ��
* ��    ��         : ��
******************************************************************************
void TIM2_IRQHandler(void)
{


	UartRxMonitor(1); ////���ڽ��ռ��;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	//�����ж�
}

*/