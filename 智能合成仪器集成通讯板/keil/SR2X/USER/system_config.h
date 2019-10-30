/*****************************************************************************
�ļ�����: system_config.h
�ļ�����: ��ϵͳ���������ļ�
��    ��: CMM
�������䣺316642070@qq.com
��    ��: 1.0
��    ��: 20190311
��ʷ�汾: ��
*****************************************************************************/
#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H

#define SR23										//����ʶ�����ĸ�оƬ ȡֵ(SR21,SR22,SR23,SR24)

/*************************************
CAN1_STDID  CAN1���ݵı�׼ID
UART1_ID    USART1��Ӧ��Э���еĴ��ں�
UART2_ID    USART2��Ӧ��Э���еĴ��ں�
UART3_ID    USART3��Ӧ��Э���еĴ��ں�
*************************************/

#ifdef SR21
	#define CAN1_STDID 0x0101
	#define UART1_ID 0x01
	#define UART2_ID 0x0A					//û�ÿ�������������� ����������������ͬ
	#define UART3_ID 0x0B					//û�ÿ�������������� ����������������ͬ
#endif	/* SR21 */
#ifdef SR22
	#define CAN1_STDID 0x0102
	#define UART1_ID 0x04
	#define UART2_ID 0x02
	#define UART3_ID 0x03	
#endif	/* SR22 */
#ifdef SR23
	#define CAN1_STDID 0x0103
	#define UART1_ID 0x07
	#define UART2_ID 0x05
	#define UART3_ID 0x06	
#endif	/* SR23 */
#ifdef SR24
	#define CAN1_STDID 0x0104
	#define UART1_ID 0x08
	#define UART2_ID 0x0C					//û�ÿ�������������� ����������������ͬ
	#define UART3_ID 0x0D					//û�ÿ�������������� ����������������ͬ
#endif	/* SR24 */
	
#endif /* __SYSTEM_CONFIG_H */
