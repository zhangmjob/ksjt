/*
 * FreeModbus Libary: BARE Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START ((uint16_t)0x0000)
#define REG_INPUT_NREGS 25
#define REG_HOLD_START  ((uint16_t)0x0000)
#define REG_HOLD_NREGS  30
/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
static USHORT   usRegHoldStart  = REG_HOLD_START;
static USHORT   usRegHoldBuf[REG_HOLD_NREGS];
extern uint8_t  g_Meter_Data[];
extern uint8_t  g_Check_Data[];

//modbus初始化
void mb_Modbus_Init(void){
    
    uint8_t address = 0x0000;//本从机地址，可通过函数去获取
    (void)eMBInit(MB_RTU,address,0,9600,MB_PAR_NONE);
    (void)eMBEnable();
}

//modbus进程函数
void user_mb_app(void){
    
    (void)eMBPoll();
}

//将计量数据结构体中的数据刷新到Modbus模拟寄存器中
static void _mb_Fresh_Input_Reg(void){
    
    uint8_t i;
    USHORT *pmeter_data = (USHORT*)g_Meter_Data;
    for(i = 0; i<REG_INPUT_NREGS; i++){
        usRegInputBuf[i] = *pmeter_data++;
    }
}

//将Modbus模拟寄存器中的数据刷新到校表数据结构体中
static void _mb_Fresh_Check_Struct(void){
    
    uint8_t i;
    USHORT *pcheck_data = (USHORT*)g_Check_Data;
    for(i = 0; i<REG_HOLD_NREGS; i++){
        *pcheck_data++ = usRegHoldBuf[i];
    }
}

//读输入寄存器
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

//写保持寄存器 -- 未定义
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    eMBErrorCode  eStatus = MB_ENOERR;
    int iRegIndex;
    
    if((usAddress >= REG_HOLD_START) && (usAddress + usNRegs <= REG_HOLD_START + REG_HOLD_NREGS)){
        iRegIndex = (int)(usAddress - usRegHoldStart);
        if(eMode == MB_REG_WRITE){
            while(usNRegs > 0){
                usRegHoldBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
        _mb_Fresh_Check_Struct();
    }else{
        eStatus = MB_ENOREG;
    }
    
    return MB_ENOREG;
}

//不使用此功能
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

//不使用此功能
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}
