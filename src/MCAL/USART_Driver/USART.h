/*
* File:    USART.h
* Author:  Yasmin Amr
* Created: 28/3/2024
* Brief:   USART Driver
* Target:  STM32F401cc
*/

#ifndef USART_H_
#define USART_H_

#include "USART_cfg.h"

/**************************************************************************/
/*					UART Channel Options      		    	 			  */
/**************************************************************************/
#define USART_CH1         (0U)
#define USART_CH2         (1U)
#define USART_CH6         (2U)

/**************************************************************************/
/*					UART Channel Oversampling Options           	      */
/**************************************************************************/
#define USART_OVERSAMPLING_16                0x00000000
#define USART_OVERSAMPLING_8                 0x00008000

/**************************************************************************/
/*					UART Channel Data Bits Options           	          */
/**************************************************************************/
#define USART_DATA_BITS_8                    0x00000000
#define USART_DATA_BITS_9                    0x00001000

/**************************************************************************/
/*					UART Channel Parity Bits Options           	          */
/**************************************************************************/
#define USART_PARITY_NONE                    0x00000000  
#define USART_PARITY_ODD                     0x00000200
#define USART_PARITY_EVEN                    0x00000000

/**************************************************************************/
/*					UART Stop Bits Options                  	          */
/**************************************************************************/
#define USART_STOP_BITS_HALF                 0x00001000
#define USART_STOP_BITS_ONE                  0x00000000
#define USART_STOP_BITS_ONE_AND_HALF         0x00003000
#define USART_STOP_BITS_TWO                  0x00002000

/**
  * @brief:  "fnpointer" type: pointer to function that takes void and returns void
  */
typedef void (*fnpointer)(void);

/**
 * @brief   Struct for Storing UART Post Compile Initilization Info
*/
typedef struct
{
    uint8_t Channel;             /**
                             * 1) USART_CH1    
                             * 2) USART_CH2   
                             * 3) USART_CH6  
                             */

    uint8_t OverSampling;        /**
                             * 1) USART_OVERSAMPLING_16
                             * 2) USART_OVERSAMPLING_8
                             */
    
    uint8_t WordLength;          /**
                             * 1) USART_DATA_BITS_8
                             * 2) USART_DATA_BITS_9
                             */
    
    uint8_t ParitySelect;        /**
                             * 1) USART_PARITY_NONE
                             * 2) USART_PARITY_ODD
                             * 3) USART_PARITY_EVEN
                             */

    uint8_t StopBits;            /**
                             * 1) USART_STOP_BITS_HALF
                             * 2) USART_STOP_BITS_ONE
                             * 3) USART_STOP_BITS_ONE_AND_HALF
                             * 4) USART_STOP_BITS_TWO
                             */

    float32_t BaudRate;           /**
                             * @note Baud Rate Number Shouldn't Exceed:
                             *      1) F_UART/8 for Oversampling=8
                             *      2) F_UART/16 for Oversampling=16
                            */
}USART_PostCompileCfg_t;

/**
 * @brief:  UART Error Status Type
*/
typedef enum
{
    USART_Ok,
    USART_NullPtr,
    USART_Nok
}USART_ErrorStatus_t;


/**
 * @brief   Initializes a single USART peripheral 
 *
 * @param   - cfg: Pointer to Struct that stores configuration for a UART channel
 *
 * @return  Error Status 
 */
USART_ErrorStatus_t USART_Init(USART_PostCompileCfg_t* cfg);

/**
 * @brief   - Takes a Buffer of Bytes to Transmit Asynchronously via a USART Peripheral
 *          - Jumps to USART ISR after every byte successfully sent
 *
 * @param   - 1) USART_Num:
 *                     1) USART_CH1    
 *                     2) USART_CH2   
 *                     3) USART_CH6  
 * 
 *          - 2) buffer: Pointer to a character/string to transmit asynchronously via USART
 * 
 *          - 3) len: Length of buffer
 * 
 *          - 4) cbf: Callback Function to call after transmission of buffer
 *                            
 * @return  Error Status 
 */
USART_ErrorStatus_t USART_TxBufferAsyncZeroCopy(uint8_t USART_Num,uint8_t*buffer, uint16_t len, fnpointer cbf );

/**
 * @brief   - Takes a Buffer of Bytes to Store received Bytes (of size "len") Asynchronously via a USART Peripheral 
 *          - Jumps to USART ISR after every byte successfully received
 *
 * @param   - 1) USART_Num:
 *                     1) USART_CH1    
 *                     2) USART_CH2   
 *                     3) USART_CH6  
 * 
 *          - 2) buffer: Pointer to a character/string to store received buffer 
 * 
 *          - 3) len: Length of buffer to receive
 * 
 *          - 4) cbf: Callback Function to call after receiving of buffer
 *                            
 * @return  Error Status 
 */
USART_ErrorStatus_t USART_RxBufferAsyncZeroCopy(uint8_t USART_Num,uint8_t*buffer, uint16_t len, fnpointer cbf);

/**
 * @brief   Sends a Byte over a USART Channel 
 *
 * @param   - 1) USART_Num:
 *                     1) USART_CH1    
 *                     2) USART_CH2   
 *                     3) USART_CH6  
 * 
 *          - 2) byte: Data to send
 * 
 * @return  Error Status: Returns if Data is successfully transmitted or not
 */
USART_ErrorStatus_t USART_SendByte(uint8_t USART_Num,uint8_t byte);

/**
 * @brief   Received a byte over a USART Channel 
 *
 * @param   - 1) USART_Num:
 *                     1) USART_CH1    
 *                     2) USART_CH2   
 *                     3) USART_CH6  
 * 
 *          - 2) byte: Pointer to variable to store received data
 * 
 * @return  Error Status: Returns if Data is successfully received or not
 */
USART_ErrorStatus_t USART_GetByte(uint8_t USART_Num,uint8_t*byte);

//uint8_t USART_TxDone(void);
//uint8_t USART_IsRx(void);


#endif
