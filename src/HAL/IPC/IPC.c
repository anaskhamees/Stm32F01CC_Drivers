/**********************************************************************************/
/*										Includes				     			  */
/**********************************************************************************/
#include "MCAL/USART_Driver/USART.h"
#include "IPC.h"


/************************************************************************************/
/*                            APIs Implementation								    */
/************************************************************************************/

void IPC_USART_Init(IPC_USART_CFG_t* IPC_USART_Config)
{
    USART_PostCompileCfg_t USART;
    USART.BaudRate          = IPC_USART_Config->IPC_USART_BaudRate       ;
    //USART.BitSampleMethod = IPC_USART_Config->IPC_USART_BitSampleMethod;
    USART.WordLength        = IPC_USART_Config->IPC_USART_DataBits       ;
    USART.OverSampling      = IPC_USART_Config->IPC_USART_OverSample     ;
    USART.ParitySelect      = IPC_USART_Config->IPC_USART_Parity         ;
    USART.StopBits          = IPC_USART_Config->IPC_USART_StopBits       ;
    USART.Channel           = IPC_USART_Config->IPC_USART_ID             ;

    ErrorStatus_t ReturnError= USART_Init(&USART);
}

void IPC_USART_SendBufferAsynchZeroCopy(uint8_t USART_ID,uint8_t* Buffer,uint16_t BufferLength,CallBack_t cbFun)
{
    ErrorStatus_t ReturnError = USART_TxBufferAsyncZeroCopy(USART_ID,Buffer,BufferLength,cbFun);

}
void IPC_USART_ReceiveBufferAsynchZeroCopy(uint8_t USART_ID,uint8_t* Buffer,uint16_t BufferLength,CallBack_t cbFun)
{
    ErrorStatus_t ReturnError = USART_RxBufferAsyncZeroCopy(USART_ID,Buffer,BufferLength,cbFun);
}

void IPC_USART_SendByte(uint8_t USART_ID,uint8_t Byte)
{
    ErrorStatus_t ReturnError = USART_SendByte(USART_ID,Byte);
}

void IPC_USART_ReceiveByte(uint8_t USART_ID,uint8_t* Byte)
{
    ErrorStatus_t ReturnError = USART_GetByte(USART_ID,Byte);
}
