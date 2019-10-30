/*****************************************************************************
文件名称: can1_app.c
文件描述: 程序主方法和CAN1口发送的数据进行协议化
作    者: CMM
作者邮箱：316642070@qq.com
版    本: 1.0
日    期: 20190311
历史版本: 无
*****************************************************************************/
#include "can1_app.h"

void CAN1_Init(void)
{
    GPIO_InitTypeDef 			GPIO_InitStructure; 
    NVIC_InitTypeDef 			NVIC_InitStructure;
    CAN_InitTypeDef				CAN_InitStructure;
    CAN_FilterInitTypeDef	CAN_FilterInitStructure;

    /***********************打开时钟*******************************************/  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 				//打开GPIOA时钟   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 					//打开CAN1口时钟

	/***********************配置管脚*******************************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       					//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//初始化PA11

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     					//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//初始化PA12
	
	/***********************配置CAN1*******************************************/

	CAN_InitStructure.CAN_TTCM=DISABLE;         									//没有使能时间触发模式
	CAN_InitStructure.CAN_ABOM=DISABLE;         									//没有使能自动离线管理
	CAN_InitStructure.CAN_AWUM=DISABLE;         									//没有使能自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE;         									//没有使能非自动重传模式
	CAN_InitStructure.CAN_RFLM=DISABLE;         									//没有使能接收FIFO锁定模式
	CAN_InitStructure.CAN_TXFP=DISABLE;         									//没有使能发送FIFO优先级
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal; 									//CAN设置为正常模式
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;      									//重新同步跳跃宽度1个时间单位
	CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;      									//时间段1为3个时间单位
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;      									//时间段2为2个时间单位
	CAN_InitStructure.CAN_Prescaler=6;         										//时间单位长度为6  1MBPS
	CAN_Init(CAN1,&CAN_InitStructure);          									//波特率为：72M/2/6(1+3+2)=1 即波特率为1MBPs

	/*******************配置CAN1过滤器只接收任意数据**********************************/
	
	CAN_FilterInitStructure.CAN_FilterNumber=0;                     //指定过滤器为1
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;   //指定过滤器为标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;  //过滤器位宽为32位
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;                //过滤器标识符的高16位值
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;									//过滤器标识符的低16位值
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;            //过滤器屏蔽标识符的高16位值
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;             //过滤器屏蔽标识符的低16位值
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;     //设定了指向过滤器的FIFO为0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;            //使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);                       //按上面的参数初始化过滤器

	/* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);                         //使能FIFO0消息挂号中断
	
	/***********************配置中断*******************************************/  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);									//选择分组0  
	                                                                
	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;        //CAN1中断通道
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //子优先0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //使能CAN1中断
	NVIC_Init(&NVIC_InitStructure);                                 //对NVIC初始化
	
}

/*****************************************************************************
函数名称: CAN1_Write_Data_Array
函数描述: 向CAN1发送数据
调用函数: STM32固件库函数
被调函数：main
传入参数: 
					WriteDataArray			需要发送的数据
					WriteDataLen				需要发送的数据长度
返 回 值: 空
其    他: 无
*****************************************************************************/
void CAN1_Write_Data_Array(uint8_t * WriteDataArray,uint8_t WriteDataLen)
{ 
	uint8_t mbox;
	uint8_t i,j;
	uint8_t onedatalen,datenum; 											//一侦数据长度，总侦数
	uint16_t timei;																		//三侦以后的延时变量
	CanTxMsg TxMessage;                         			//CAN发送包

	TxMessage.StdId=0x0100;      											//主机标准标识符为固定值0x0100
	TxMessage.IDE=CAN_ID_STD;   											//使用标准标识符
	TxMessage.RTR=CAN_RTR_DATA; 											//为数据帧
	
	/***********************对数据进行分侦发送*******************************************/
	datenum = WriteDataLen/8;													//判断需要发送几侦
	if(WriteDataLen%8!=0)datenum++;
	for(i=0;i< datenum;i++)														//循环逐侦发送
	{
		if(i<datenum-1){
			onedatalen = 8;
        }else{
			onedatalen = WriteDataLen-8*(datenum-1);
		}
        
		TxMessage.DLC=onedatalen; 	//设置本侦数据长度
		for(j=0;j<onedatalen;j++){
			TxMessage.Data[j] = WriteDataArray[8*i+j]; 	//放入本侦数据
		}

        mbox= CAN_Transmit(CAN1,&TxMessage);  							//放入邮箱准备发送
        while(CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed);		//等待发送结束
        
		if((i+1)%3==0){//每发送三侦等待邮箱
			for(timei=0;timei<10000;timei++);	
        }            
	}

}


/*****************************************************************************
变量名称: CAN1_READ_DATA_Buffer
变量描述: CAN1接收数据缓存组
其    他: 因为下面有四个子设备，所以有四组数据
*****************************************************************************/
uint8_t CAN1_READ_SR2x_DATA_Buffer[4][200];
/*****************************************************************************
变量名称: CAN1_READ_DATA_Len
变量描述: CAN1接收数据长度组
其    他: 因为下面有四个子设备，所以有四组数据
*****************************************************************************/
uint8_t CAN1_READ_SR2x_DATA_Len[4];

/*****************************************************************************
函数名称: CAN1_Read_Data_Len
函数描述: 获取CAN1缓存区中对应子设备的数据长度
调用函数: 无
被调函数：main
传入参数: SR_Num 子设备标识符 取值(0,1,2,3)
返 回 值: 缓存区对应子设备数据长度
其    他: 无
*****************************************************************************/
uint8_t CAN1_Read_Data_Len(uint8_t SR_Num)
{
	if(SR_Num>3){
        return 0; //判断SR_Num是否合法 不合法返回
    }else{     
        return CAN1_READ_SR2x_DATA_Len[SR_Num];					//返回缓存区对应SR_Num的数据长度
    }
}

/*****************************************************************************
函数名称: CAN1_Read_Data_Array
函数描述: 从CAN1缓存区读取对应子设备的数据 并清空对应的缓存区
调用函数: 无
被调函数：main
传入参数: 
					ReadDataArray	 用来保存数据的地址
					SR_Num 子设备标识符 取值(0,1,2,3)
返 回 值: 空
其    他: 无
*****************************************************************************/
void CAN1_Read_Data_Array(uint8_t * ReadDataArray,uint8_t SR_Num)
{
	uint8_t i;
	if(SR_Num>3){ //判断SR_Num是否合法 不合法返回
        return;
    }       
	for(i=0;i<CAN1_READ_SR2x_DATA_Len[SR_Num];i++)
	{
		ReadDataArray[i] = CAN1_READ_SR2x_DATA_Buffer[SR_Num][i]; //将缓存区对应SR_Num的数据循环放入保存数据地址
	}
	CAN1_READ_SR2x_DATA_Len[SR_Num] = 0;						  //清空SR_Num对应的缓存区
	
}

/*****************************************************************************
函数名称: USB_LP_CAN1_RX0_IRQHandler
函数描述: USB中断和CAN接收中断服务程序
调用函数: 无
被调函数：无
传入参数: 无
返 回 值: 无
其    他: 由于使用了PA11和PA12,USB和CAN共用I/O。 
*****************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    uint8_t i;
    CanRxMsg RxMessage;
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage); 		//接收FIFO0中的数据  

	for(i=0;i<RxMessage.DLC;i++)
	{
		switch(RxMessage.StdId)										//通过标准标识符判断是由哪一台子设备发上来的
		{
			case 0x0101:
				CAN1_READ_SR2x_DATA_Buffer[0][CAN1_READ_SR2x_DATA_Len[0]++]=RxMessage.Data[i];		//循环将数据放入对应的缓存区，长度加1
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
