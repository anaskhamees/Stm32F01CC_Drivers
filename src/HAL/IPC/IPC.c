/**********************************************************************************/
/*										Includes				     			  */
/**********************************************************************************/
#include "MCAL/UART/USART.h"
#include "IPC.h"


/************************************************************************************/
/*                            APIs Implementation								    */
/************************************************************************************/

void IPC_USART_Init(IPC_USART_CFG_t* IPC_USART_Config)
{
    USART_CFG_t USART;
    USART.BaudRate        = IPC_USART_Config->IPC_USART_BaudRate       ;
    USART.BitSampleMethod = IPC_USART_Config->IPC_USART_BitSampleMethod;
    USART.DataBits        = IPC_USART_Config->IPC_USART_DataBits       ;
    USART.OverSample      = IPC_USART_Config->IPC_USART_OverSample     ;
    USART.Parity          = IPC_USART_Config->IPC_USART_Parity         ;
    USART.StopBits        = IPC_USART_Config->IPC_USART_StopBits       ;
    USART.USART_ID        = IPC_USART_Config->IPC_USART_ID             ;

    ErrorStatus_t ReturnError= USART_Init(&USART);
}

void IPC_USART_SendBufferAsynchZeroCopy(uint8_t USART_ID,uint8_t* Buffer,uint32_t BufferLength,CallBack_t cbFun)
{
    ErrorStatus_t ReturnError = USART_SendBufferAsynchZeroCopy(USART_ID,Buffer,BufferLength,cbFun);

}
void IPC_USART_ReceiveBufferAsynchZeroCopy(uint8_t USART_ID,uint8_t* Buffer,uint32_t BufferLength,CallBack_t cbFun)
{
    ErrorStatus_t ReturnError = USART_ReceiveBufferAsynchZeroCopy(USART_ID,Buffer,BufferLength,cbFun);
}

void IPC_USART_SendByte(uint8_t USART_ID,uint8_t Byte)
{
    ErrorStatus_t ReturnError = USART_SendByte(USART_ID,Byte);
}

void IPC_USART_ReceiveByte(uint8_t USART_ID,uint8_t* Byte)
{
    ErrorStatus_t ReturnError = USART_ReceiveByte(USART_ID,Byte);
}
