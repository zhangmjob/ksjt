/*****************************************************************************
文件名称: system_config.h
文件描述: 子系统运行配置文件
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/
#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H

#define SR23										//用来识别是哪个芯片 取值(SR21,SR22,SR23,SR24)

/*************************************
CAN1_STDID  CAN1数据的标准ID
UART1_ID    USART1对应的协议中的串口号
UART2_ID    USART2对应的协议中的串口号
UART3_ID    USART3对应的协议中的串口号
*************************************/

#ifdef SR21
	#define CAN1_STDID 0x0101
	#define UART1_ID 0x01
	#define UART2_ID 0x0A					//没用可以设成任意数字 但是三个数不能相同
	#define UART3_ID 0x0B					//没用可以设成任意数字 但是三个数不能相同
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
	#define UART2_ID 0x0C					//没用可以设成任意数字 但是三个数不能相同
	#define UART3_ID 0x0D					//没用可以设成任意数字 但是三个数不能相同
#endif	/* SR24 */
	
#endif /* __SYSTEM_CONFIG_H */
