/*****************************************************************************
�ļ�����: mrv01_app.c
�ļ�����: ��ѹ������������
��    ��: zm
�������䣺2468635978@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#include "mrv01_app.h"

/*****************************************************************************
��������: MRV01_Init
��������: ��ʼ����ѹ������IO
���ú���: STM32�̼��⺯��
����������main
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/
void MRV_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB, ENABLE);				//��GPIOAʱ��
  
  /***********************���ùܽ�*******************************************/
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							//�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);												//��ʼ��
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							//�������
	GPIO_Init(GPIOD, &GPIO_InitStructure);												//��ʼ��
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							//�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);												//��ʼ��
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 							//�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);												//��ʼ��
  
  GPIO_ResetBits(GPIOC,GPIO_Pin_12);   //��ʼ��Ϊ�͵�ƽ
  GPIO_ResetBits(GPIOD,GPIO_Pin_2);
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_12);
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}

void MRV1_Change()
{
  GPIO_SetBits(GPIOC,GPIO_Pin_12);//led2
  GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void MRV1_Reback()
{
  GPIO_ResetBits(GPIOC,GPIO_Pin_12);
  GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}

void MRV2_Change()
{
  GPIO_SetBits(GPIOD,GPIO_Pin_2);//led1
  GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

void MRV2_Reback()
{
  GPIO_ResetBits(GPIOD,GPIO_Pin_2);
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
