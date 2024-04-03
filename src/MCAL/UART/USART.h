#ifndef MCAL_UART_UART_H_
#define MCAL_UART_UART_H_

/**********************************************************************************/
/*										Includes				     			  */
/**********************************************************************************/
#include "Std_Lib/Std_Libraries.h"

/***********************************************************************************/
/*										Defines									   */
/***********************************************************************************/
#define USART_READY                 (0UL)
#define USART_BUSY                  (1UL)

/********************************************************************************/
/*                 USART Peripherals in the STM32F401xx                         */                                                                                
/********************************************************************************/
#define USART1      (0UL)
#define USART2      (1UL)
#define USART6      (2UL)
/*******************************************************************************/
/*					  USART Peripheral Over Sample Masks                       */
/*******************************************************************************/
#define USART_OVERSAMPLING_8                 (0x00008000UL)
#define USART_OVERSAMPLING_16                (0x00000000UL)
/*******************************************************************************/
/*					  USART Peripheral Sample Method Masks                     */
/*******************************************************************************/
#define USART_SAMPLE_BIT1               (0x00000800UL)   
#define USART_SAMPLE_BIT3               (0x00000000UL)

/******************************************************************************/
/*					 USART Peripheral Data Bits Masks           	          */
/******************************************************************************/
#define USART_DATA_BITS_8                    (0x00000000UL)
#define USART_DATA_BITS_9                    (0x00001000UL)

/****************************************************************************/
/*  			USART Peripheral Parity Bits Masks             	            */
/****************************************************************************/
#define USART_PARITY_NONE                    (0x00000000UL) 
#define USART_PARITY_EVEN                    (0x00000400UL) 
#define USART_PARITY_ODD                     (0x00000600UL)
/****************************************************************************/
/*				USART Peripheral Stop Bits Masks               	            */
/****************************************************************************/
#define UART_STOP_BITS_HALF                  (0x00001000UL)      
#define UART_STOP_BITS_ONE                   (0x00000000UL)
#define UART_STOP_BITS_ONE_HALF              (0x00003000UL)
#define UART_STOP_BITS_TWO                   (0x00002000UL)

/*****************************************************************************************/
/*                                      Types                                            */
/*****************************************************************************************/
typedef void (*CallBack_t)(void);

typedef struct
{
    uint32_t USART_ID       ; /* Options:
                                    * USART1 
                                    * USART2  
                                    * USART6   
                               */

    uint32_t StopBits       ;  /* Options:
                                    * UART_STOP_BITS_HALF 
                                    * UART_STOP_BITS_ONE  
                                    * UART_STOP_BITS_ONE_HALF
                                    * UART_STOP_BITS_TWO   
                               */

    uint32_t BitSampleMethod;  /* Options:
                                    * USART_SAMPLE_BIT1 
                                    * USART_SAMPLE_BIT3    
                                */

    uint32_t OverSample     ;  /* Options:
                                    * USART_OVERSAMPLING_8 
                                    * USART_OVERSAMPLING_16    
                               */

    uint32_t DataBits       ;  /* Options:
                                    * USART_DATA_BITS_8 
                                    * USART_DATA_BITS_9    
                                */

    uint32_t Parity         ;  /* Options:
                                    * USART_PARITY_NONE 
                                    * USART_PARITY_EVEN
                                    * USART_PARITY_ODD    
                                */
    
    float32_t BaudRate      ;   /* Warning :
                                 * Max BaudRate @ OVER8=0 --> CLK Frequency / 16
                                 * Max BaudRate @ OVER8=1 --> CLK Frequency / 8
                                 */
}USART_CFG_t;



/***********************************************************************************/
/*							APIs ProtoTypes 									   */
/***********************************************************************************/

ErrorStatus_t USART_Init(USART_CFG_t* USART_Config);
ErrorStatus_t USART_SendBufferAsynchZeroCopy(uint8_t USART_ID,uint8_t* Buffer,uint32_t BufferLength,CallBack_t cbFun);
ErrorStatus_t USART_ReceiveBufferAsynchZeroCopy(uint8_t USART_ID,uint8_t* Buffer,uint32_t BufferLength,CallBack_t cbFun);
ErrorStatus_t USART_SendByte(uint8_t USART_ID,uint8_t Byte);
ErrorStatus_t USART_ReceiveByte(uint8_t USART_ID,uint8_t* Byte);






#endif // MCAL_UART_UART_H_