/*****************************************************************************
�ļ�����: gpio_app.c
�ļ�����: GPIO�����ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190322
��ʷ�汾: ��
*****************************************************************************/
#include "gpio_app.h"

/*****************************************************************************
��������: GPIO_Init
��������: GPIOʼʼ��
���ú���: STM32�̼��⺯��
����������main
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/
void GPIO_App_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	/***********************��ʱ��*******************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);				//��GPIOBʱ��
	
	/***********************���ùܽ�*******************************************/	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 							  //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);												//��ʼ��PB10
}

/*****************************************************************************
��������: Get_GPIO_Input_Value
��������: ����IO�����״̬
���ú���: STM32�̼��⺯��
����������main
�������: ��
�� �� ֵ: ��
��    ��: ��
*****************************************************************************/

uint8_t Get_GPIO_Input_Value(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==SET)
		{
			return 3;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==SET)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}	
}
