/*
* File:    USART.c
* Author:  Yasmin Amr
* Created: 28/3/2024
* Brief:   USART Driver
* Target:  STM32F401cc
*/

/**
 * Notes for Implementing
 * 1) Falling Edge of idle state is what determines the USART to start listening to the frame
 * 2) Stop bits are essential in order to bring the signal to idle state again
 * 3) If theres a frame error, such as mistaking a data bit as a start bit or so on.. the frame error flag will be set
 * 4) Each USART has only one handler, so you must check on all the flags inside this handler
 * 5) We clear RXNE Interrupt Enable at the beginning of the RxAsync Function in order not to receive any interrupts while configuring a new buffer
 * 6) In a driver, you must give the user all the configurable options in the datasheet, any mistake would be the users fault
 * 7) In TXAsynch function: Enable TXE Interrupt after sending data and incrementing position to avoid interrupts happening between sending data and incrementing position
 * 7.1) TXE Interrupt Flag is initially set to 1 for some reason, so enable the interrupt at the end of the function for the same reason above
*/

/**
 * Defs:
 * 1) RXNE: Receiver Buffer Not Empty (Ready to Read Data)
 * 2) TXE: Transmitter Buffer Empty: No Data in Buffer (Data Shifted to Shift Register), Ready to Transmit Data 
 * 3) TC: Transmit Complete: When All Data in Buffer is Transmitted
*/

#include "USART.h"

/**************************************************************************/
/*					USART Request States                      	          */
/**************************************************************************/
#define busy     (0U)
#define ready    (1U)

/**************************************************************************/
/*					USART Counts in Microcontroller            	          */
/**************************************************************************/
#define USART_COUNT  (3U)

/**************************************************************************/
/*					USART Implementation Masks                 	          */
/**************************************************************************/
#define USART_TC_FLAG_MASK                  0x00000040
#define USART_TXE_FLAG_MASK                 0x00000080
#define USART_RXNE_FLAG_MASK                0x00000020

#define USART_ENABLE_MASK                   0x00002000
#define USART_TXE_INT_ENABLE_MASK           0X00000080
#define USART_RXNE_INT_ENABLE_MASK          0X00000020 
#define USART_TX_ENABLE_MASK                0X00000008
#define USART_RX_ENABLE_MASK                0X00000004

#define USART_WORD_LENGTH_MASK              0x00001000
#define USART_OVERSAMPLING_MASK             0x00008000
#define USART_STOP_BIT_MASK                 0x00003000

#define USART_PARITY_ON                     0x00000400
#define USART_PARITY_SELECT_MASK            0x00000200
#define USART_PARITY_CONTROL_MASK           0x00000400

#define USART_TX_ENABLE                     0x00000008
#define USART_RX_ENABLE                     0x00000004

#define USART_TX_DISABLE                    0x00000000
#define USART_RX_DISABLE                    0x00000000

#define USART_DIV_MANTISSA_MASK              0x0000FFF0
#define USART_DIV_FRACTION_MASK              0x0000000F

#define USART_ENABLE                         0x00002000
#define USART_DISABLE                        0x00000000

#define USART_TXE_INTERRUPT_ENABLE           0x00000080
#define USART_TXE_INTERRUPT_DISABLE          0x00000000

#define USART_TX_COMPLETE_INT_ENABLE         0x00000040
#define USART_TX_COMPLETE_INT_DISABLE        0x00000000

#define USART_RXNE_INT_ENABLE                0x00000020   
#define USART_RXNE_INT_DISABLE               0x00000000

/**************************************************************************/
/*					USART Shifts                               	          */
/**************************************************************************/
#define SHIFT_FOUR                           (4U)

/**************************************************************************/
/*					USART Error Checking                    	          */
/**************************************************************************/

#define IS_USART_OVERSAMPLING(USART_OVERSAMPLING)       ((USART_OVERSAMPLING==USART_OVERSAMPLING_16)    ||\
                                                        (USART_OVERSAMPLING==USART_OVERSAMPLING_8))


#define IS_USART_WORD_LENGTH(USART_WORD_LENGTH)         ((USART_WORD_LENGTH==USART_DATA_BITS_8)         ||\
                                                        (USART_WORD_LENGTH==USART_DATA_BITS_9))

#define IS_USART_PARITY_SELECT(USART_PARITY_SELECT)     ((USART_PARITY_SELECT==USART_PARITY_NONE)       ||\
                                                        (USART_PARITY_SELECT==USART_PARITY_EVEN)        ||\
                                                        (USART_PARITY_SELECT==USART_PARITY_ODD))


#define IS_USART_STOP_BITS(USART_STOP_BITS)             ((USART_STOP_BITS==USART_STOP_BITS_HALF)                ||\
                                                        (USART_STOP_BITS==USART_STOP_BITS_ONE)                  ||\
                                                        (USART_STOP_BITS==USART_STOP_BITS_ONE_AND_HALF)         ||\
                                                        (USART_STOP_BITS==USART_STOP_BITS_TWO))

/**
 * @brief:  USART Registers 
*/
typedef struct
{
   uint32_t SR;
   uint32_t DR;
   uint32_t BRR;
   uint32_t CR1;
   uint32_t CR2;
   uint32_t CR3;
   uint32_t GTPR;
}USART_Reg_t;

/**
 * @brief:   Struct for Storing UART TX/RX Request Buffer Info 
*/
typedef struct
{
    volatile uint8_t*data;                 /*Bytes to be Sent/Received Asynchronously*/
    volatile uint16_t len;                 /*Number of Bytes to Be Sent/Received Asynchronously*/
    volatile fnpointer cbf;           /*CallBack Function After End of Operation*/
    volatile uint16_t pos;                  /*Tracker for Bytes*/
    volatile uint8_t state;                 /*State for User Request*/
}USART_Buffer_t;

/**************************************************************************/
/*					USART Peri Addresses                       	          */
/**************************************************************************/
#define USART1        ((volatile USART_Reg_t*const)0x40011000)
#define USART2        ((volatile USART_Reg_t*const)0x40004400)
#define USART6        ((volatile USART_Reg_t*const)0x40011400)

/*Array to store USART TX User Request Buffer Info for each USART Peripheral*/
USART_Buffer_t volatile USART_TxBuffer[3]={{.state=ready},{.state=ready},{.state=ready}};

/*Array to store USART RX User Request Buffer Info for each USART Peripheral*/
USART_Buffer_t volatile USART_RxBuffer[3]={{.state=ready},{.state=ready},{.state=ready}};

/*Array of USART Peripheral Addresses*/
uint32_t USART_Peri_Add[3]={0x40011000,0x40004400,0x40011400};

/*Array of USART Peripheral Configured Frequencies*/
uint32_t USART_Freq[3]={F_USART1,F_USART2,F_USART6};


/**
 * @brief   Initializes a single USART peripheral 
 *
 * @param   - cfg: Pointer to Struct that stores configuration for a UART channel
 *
 * @return  Error Status 
 */
USART_ErrorStatus_t USART_Init(USART_PostCompileCfg_t *cfg)
{
    USART_ErrorStatus_t RET_ErrorStatus=USART_Ok;
    if(!cfg)
    {
        RET_ErrorStatus=USART_NullPtr; 
    }
    else if(cfg->Channel>USART_CH6)
    {
        RET_ErrorStatus=USART_Nok; 
    }
    else if(!(IS_USART_OVERSAMPLING(cfg->OverSampling)))
    {
        RET_ErrorStatus=USART_Nok; 
    }
    else if(!(IS_USART_WORD_LENGTH(cfg->WordLength)))
    {
        RET_ErrorStatus=USART_Nok; 
    }
    else if(!(IS_USART_PARITY_SELECT(cfg->ParitySelect)))
    {
        RET_ErrorStatus=USART_Nok; 
    }
    else if(!(IS_USART_STOP_BITS(cfg->StopBits)))
    {
        RET_ErrorStatus=USART_Nok; 
    }
    else
    {
        /*Check for Max Baud Rate*/
        float32_t OVER8= (cfg->OverSampling==USART_OVERSAMPLING_16)?0:1;
        uint32_t MaxBaudRate= USART_Freq[cfg->Channel]/(8*(2-OVER8));
        if(cfg->BaudRate>MaxBaudRate)
        {
            RET_ErrorStatus=USART_Nok; 
        }
        else
        {
            volatile USART_Reg_t *const USART=( volatile USART_Reg_t *)USART_Peri_Add[cfg->Channel];
            /*Enable USART*/
            uint32_t Loc_Reg=USART->CR1;
            Loc_Reg|=(USART_ENABLE);
            
            /*Program Word Length*/
            Loc_Reg&=~(USART_WORD_LENGTH_MASK);
            Loc_Reg|=(cfg->WordLength);
            USART->CR1=Loc_Reg;

            /*Program Number of Stop Bits*/
            Loc_Reg=USART->CR2;
            Loc_Reg&=~USART_STOP_BIT_MASK;
            Loc_Reg|=(cfg->StopBits);
            USART->CR2=Loc_Reg;

            /*Program Parity Bits*/
            Loc_Reg=USART->CR1;
            Loc_Reg&=~USART_PARITY_CONTROL_MASK;
            if(cfg->ParitySelect==USART_PARITY_NONE)
            {
               Loc_Reg|=USART_PARITY_NONE;
            }
            else
            {
                Loc_Reg|=USART_PARITY_ON;
                Loc_Reg&=~USART_PARITY_SELECT_MASK;
                Loc_Reg|=cfg->ParitySelect;
            }
            USART->CR1=Loc_Reg;

            /*Select Baud Rate*/
            float32_t USARTDIV=((float32_t)(USART_Freq[cfg->Channel])/((cfg->BaudRate)*8*(2-OVER8)));
            float32_t FracionBoundary=(cfg->OverSampling==USART_OVERSAMPLING_16)?16:8;
            uint32_t DIV_Fraction=(uint32_t)(FracionBoundary*(float32_t)((float32_t)USARTDIV-(uint32_t)USARTDIV))+1;
            uint32_t MAXVALUE=(cfg->OverSampling==USART_OVERSAMPLING_16)?15:7;
            uint32_t DIV_Mantissa=0;
            if(DIV_Fraction>MAXVALUE)
            {
                DIV_Fraction=0;
                DIV_Mantissa=(uint32_t)USARTDIV+1;
            }
            else
            {
                DIV_Mantissa= (uint32_t)USARTDIV;
            }

            Loc_Reg=USART->BRR;
            Loc_Reg&=~USART_DIV_MANTISSA_MASK;
            Loc_Reg|=DIV_Mantissa<<SHIFT_FOUR;

            Loc_Reg&=~USART_DIV_FRACTION_MASK;
            Loc_Reg|=DIV_Fraction;

            USART->BRR=Loc_Reg;

            /*Enable Transmitter to Begin Idle State*/
            Loc_Reg=USART->CR1;
            Loc_Reg|=USART_TX_ENABLE;

            /*Enable Receiver to Start Searching for Start Bit*/
            Loc_Reg|=USART_RX_ENABLE;
            USART->CR1=Loc_Reg;
        } 
    }
    return RET_ErrorStatus;
}

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
USART_ErrorStatus_t USART_TxBufferAsyncZeroCopy(uint8_t USART_Num,uint8_t*buffer, uint16_t len, fnpointer cbf )
{
    USART_ErrorStatus_t RetErrorStatus=USART_Ok;
    if(!buffer)
    {
        RetErrorStatus=USART_NullPtr;
    }
    else if(!((USART_Num==USART_CH1)||(USART_Num==USART_CH2)||(USART_Num==USART_CH6)))
    {
        RetErrorStatus=USART_Nok;
    }
    else
    {        
        if((USART_TxBuffer[USART_Num].state)==ready)
        {
            USART_TxBuffer[USART_Num].state=busy;
            volatile USART_Reg_t *const USART=( volatile USART_Reg_t *)USART_Peri_Add[USART_Num];

            /*Copy User Request Data*/
            USART_TxBuffer[USART_Num].data=buffer;
            USART_TxBuffer[USART_Num].len=len;
            USART_TxBuffer[USART_Num].cbf=cbf;

            /*Reset Byte Position Index*/
            USART_TxBuffer[USART_Num].pos=0;
            
            /*Send First Byte to Trigger Interrupt*/
            USART->DR=(USART_TxBuffer[USART_Num].data[USART_TxBuffer[USART_Num].pos++]);

            /*Enable Transmit Interrupt*/
            uint32_t Loc_Reg=USART->CR1;
            Loc_Reg&=~USART_TXE_INT_ENABLE_MASK;
            Loc_Reg|=USART_TXE_INTERRUPT_ENABLE;
            USART->CR1=Loc_Reg;
        }
        else
        {
            RetErrorStatus=USART_Nok;
        }
    }    
    
    return RetErrorStatus;
}

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
USART_ErrorStatus_t USART_RxBufferAsyncZeroCopy(uint8_t USART_Num,uint8_t*buffer, uint16_t len, fnpointer cbf)
{
    USART_ErrorStatus_t RetErrorStatus=USART_Ok;
    if(!buffer)
    {
        RetErrorStatus=USART_NullPtr;
    }
    else if(!((USART_Num==USART_CH1)||(USART_Num==USART_CH2)||(USART_Num==USART_CH6)))
    {
        RetErrorStatus=USART_Nok;
    }
    else
    {
        if((USART_RxBuffer[USART_Num].state==ready))
        {
            volatile USART_Reg_t *const USART=( volatile USART_Reg_t *)USART_Peri_Add[USART_Num];

            /*Disable Receive Interrupt*/
            uint32_t Loc_Reg=USART->CR1;
            Loc_Reg&=~USART_RXNE_INT_ENABLE_MASK;
            Loc_Reg|=USART_RXNE_INT_DISABLE;
            USART->CR1=Loc_Reg;
            USART_RxBuffer[USART_Num].state=busy;

            /*Copy User Request Data*/
            USART_RxBuffer[USART_Num].data=buffer;
            USART_RxBuffer[USART_Num].len=len;
            USART_RxBuffer[USART_Num].cbf=cbf;

            /*Reset Byte Position Index*/
            USART_RxBuffer[USART_Num].pos=0;
        
            /*Enable Receive Interrupt*/
            Loc_Reg=USART->CR1;
            Loc_Reg&=~USART_RXNE_INT_ENABLE_MASK;
            Loc_Reg|=USART_RXNE_INT_ENABLE;
            USART->CR1=Loc_Reg;
        }
        else
        {
            RetErrorStatus=USART_Nok;
        } 
    }
    return RetErrorStatus;

}

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
USART_ErrorStatus_t USART_SendByte(uint8_t USART_Num,uint8_t byte)
{
    USART_ErrorStatus_t RetErrorStatus=USART_Ok;
    if(!((USART_Num==USART_CH1)||(USART_Num==USART_CH2)||(USART_Num==USART_CH6)))
    {
        RetErrorStatus=USART_Nok;
    }
    else
    {
        volatile USART_Reg_t *const USART=( volatile USART_Reg_t *)USART_Peri_Add[USART_Num];
        uint16_t Timeout=3000;
        if(USART_TxBuffer[USART_Num].state==ready)
        {
            USART_TxBuffer[USART_Num].state=busy;
            USART->DR=byte;
            while((Timeout--)&&(!(USART->SR&USART_TXE_FLAG_MASK)));
            if(USART->SR&USART_TXE_FLAG_MASK)
            {
                //Byte Transferred
            }
            else
            {
                //Timeout Reached
                RetErrorStatus=USART_Nok;
            }
            USART_TxBuffer[USART_Num].state=ready;
        }
    }
    return RetErrorStatus;
}

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
USART_ErrorStatus_t USART_GetByte(uint8_t USART_Num,uint8_t*byte)
{
    USART_ErrorStatus_t RetErrorStatus=USART_Ok;
    if(!byte)
    {
        RetErrorStatus=USART_Nok;
    }
    else if(!(USART_Num==USART_CH1)||(USART_Num==USART_CH2)||(USART_Num==USART_CH6))
    {
        RetErrorStatus=USART_Nok;
    }
    else
    {
        volatile USART_Reg_t *const USART=( volatile USART_Reg_t *)USART_Peri_Add[USART_Num];
        uint16_t Timeout=3000;
        if(USART_RxBuffer[USART_Num].state==ready)
        {
            USART_RxBuffer[USART_Num].state=busy;
            while(Timeout--&&(!(USART->SR&USART_RXNE_FLAG_MASK)));
            if(USART->SR&USART_RXNE_FLAG_MASK)
            {
                *byte=(uint8_t)USART->DR;
            }
            else
            {
                //Timeout Reached
                RetErrorStatus=USART_Nok;
            }
            USART_RxBuffer[USART_Num].state=ready;
        }
        
    }
    return RetErrorStatus;
}

/**
 * @brief: USART1 Handler:
 *             1) Transmittion Buffer Empty (Data Transmission of 1 byte)
 *             2) Receiver Buffer Not Empty (1 byte of Data Received)
*/
void USART1_IRQHandler(void)
{
        /*If Data is Transferred to the Shift Register*/
        if((USART1->SR&USART_TXE_FLAG_MASK))
        {
                /*Check on Length( If More Bytes to Send)*/
                if(USART_TxBuffer[USART_CH1].pos<USART_TxBuffer[USART_CH1].len)
                {
                    /*Write Data to Data Register, Flag is Automatically Cleared*/
                    USART1->DR= (USART_TxBuffer[USART_CH1].data[USART_TxBuffer[USART_CH1].pos++]);

                }
                
                /*All Bytes Sent*/
                else if(USART_TxBuffer[USART_CH1].len!=0)
                {
                    /*Disable Transmit Interrupt*/
                    uint32_t Loc_Reg=USART1->CR1;
                    Loc_Reg&=~USART_TXE_INT_ENABLE_MASK;
                    Loc_Reg|=USART_TXE_INTERRUPT_DISABLE;
                    USART1->CR1=Loc_Reg;

                    USART_TxBuffer[USART_CH1].state=ready;
                    USART_TxBuffer[USART_CH1].pos=0;
                    /*Check on NULL*/
                    if(USART_TxBuffer[USART_CH1].cbf)
                    {
                        USART_TxBuffer[USART_CH1].cbf();
                    }
                }
        }

        /*Unread Data in Receive Buffer*/
        if(USART1->SR&USART_RXNE_FLAG_MASK)
        {
                /*Check on Length*/
                if(USART_RxBuffer[USART_CH1].pos<USART_RxBuffer[USART_CH1].len)
                {
                    /*Read data from Buffer, Flag is automatically cleared*/
                    USART_RxBuffer[USART_CH1].data[USART_RxBuffer[USART_CH1].pos++]=(uint8_t)USART1->DR;
                }
                if(USART_RxBuffer[USART_CH1].pos==USART_RxBuffer[USART_CH1].len)
                {
                    /*Disable Receive Interrupt*/
                    uint32_t Loc_Reg=USART1->CR1;
                    Loc_Reg&=~USART_RXNE_INT_ENABLE_MASK;
                    Loc_Reg|=USART_RXNE_INT_DISABLE;
                    USART1->CR1=Loc_Reg;

                    USART_RxBuffer[USART_CH1].state=ready;
                    USART_RxBuffer[USART_CH1].pos=0;
                    /*Check on NULL*/
                    if(USART_RxBuffer[USART_CH1].cbf)
                    {
                        USART_RxBuffer[USART_CH1].cbf();
                    }
                }
        }
   
}

/**
 * @brief: USART2 Handler:
 *             1) Transmittion Buffer Empty (Data Transmission of 1 byte)
 *             2) Receiver Buffer Not Empty (1 byte of Data Received)
*/
void USART2_IRQHandler(void)
{
    /*If Data is Transferred to the Shift Register*/
    if((USART2->SR & USART_TXE_FLAG_MASK))
    {
        /*Check on Length( If More Bytes to Send)*/
        if(USART_TxBuffer[USART_CH2].pos < USART_TxBuffer[USART_CH2].len)
        {
            /*Write Data to Data Register, Flag is Automatically Cleared*/
            USART2->DR = (USART_TxBuffer[USART_CH2].data[USART_TxBuffer[USART_CH2].pos++]);
        }
        /*All Bytes Sent*/
        else if(USART_TxBuffer[USART_CH2].len!=0)
        {
            /*Disable Transmit Interrupt*/
            uint32_t Loc_Reg=USART2->CR1;
            Loc_Reg&=~USART_TXE_INT_ENABLE_MASK;
            Loc_Reg|=USART_TXE_INTERRUPT_DISABLE;
            USART2->CR1=Loc_Reg;

            USART_TxBuffer[USART_CH2].state = ready;
            USART_TxBuffer[USART_CH2].pos = 0;
            /*Check on NULL*/
            if(USART_TxBuffer[USART_CH2].cbf)
            {
                USART_TxBuffer[USART_CH2].cbf();
            }
        }
    }

    /*Unread Data in Receive Buffer*/
    if(USART2->SR & USART_RXNE_FLAG_MASK)
    {
        /*Check on Length*/
        if(USART_RxBuffer[USART_CH2].pos < USART_RxBuffer[USART_CH2].len)
        {
            /*Read data from Buffer, Flag is automatically cleared*/
            USART_RxBuffer[USART_CH2].data[USART_RxBuffer[USART_CH2].pos++] = (uint8_t)USART2->DR;
        }
        if(USART_RxBuffer[USART_CH2].pos == USART_RxBuffer[USART_CH2].len)
        {
            /*Disable Receive Interrupt*/
            uint32_t Loc_Reg=USART2->CR1;
            Loc_Reg&=~USART_RXNE_INT_ENABLE_MASK;
            Loc_Reg|=USART_RXNE_INT_DISABLE;
            USART2->CR1=Loc_Reg;

            USART_RxBuffer[USART_CH2].state = ready;
            USART_RxBuffer[USART_CH2].pos = 0;
            /*Check on NULL*/
            if(USART_RxBuffer[USART_CH2].cbf)
            {
                USART_RxBuffer[USART_CH2].cbf();
            }
        }
    }

}

/**
 * @brief: USART6 Handler:
 *             1) Transmittion Buffer Empty (Data Transmission of 1 byte)
 *             2) Receiver Buffer Not Empty (1 byte of Data Received)
*/
void USART6_IRQHandler(void)
{
    /*If Data is Transferred to the Shift Register*/
    if((USART6->SR & USART_TXE_FLAG_MASK))
    {
        /*Check on Length( If More Bytes to Send)*/
        if(USART_TxBuffer[USART_CH6].pos < USART_TxBuffer[USART_CH6].len)
        {
            /*Write Data to Data Register, Flag is Automatically Cleared*/
            USART6->DR = (USART_TxBuffer[USART_CH6].data[USART_TxBuffer[USART_CH6].pos++]);
        }
        /*All Bytes Sent*/
        else if(USART_TxBuffer[USART_CH6].len!=0)
        {
            /*Disable Transmit Interrupt*/
            uint32_t Loc_Reg=USART6->CR1;
            Loc_Reg&=~USART_TXE_INT_ENABLE_MASK;
            Loc_Reg|=USART_TXE_INTERRUPT_DISABLE;
            USART6->CR1=Loc_Reg;

            USART_TxBuffer[USART_CH6].state = ready;
            USART_TxBuffer[USART_CH6].pos = 0;
            /*Check on NULL*/
            if(USART_TxBuffer[USART_CH6].cbf)
            {
                USART_TxBuffer[USART_CH6].cbf();
            }
        }
    }

    /*Unread Data in Receive Buffer*/
    if(USART6->SR & USART_RXNE_FLAG_MASK)
    {
        /*Check on Length*/
        if(USART_RxBuffer[USART_CH6].pos < USART_RxBuffer[USART_CH6].len)
        {
            /*Read data from Buffer, Flag is automatically cleared*/
            USART_RxBuffer[USART_CH6].data[USART_RxBuffer[USART_CH6].pos++] = (uint8_t)USART6->DR;
        }
        if(USART_RxBuffer[USART_CH6].pos == USART_RxBuffer[USART_CH6].len)
        {
            /*Disable Receive Interrupt*/
            uint32_t Loc_Reg=USART6->CR1;
            Loc_Reg&=~USART_RXNE_INT_ENABLE_MASK;
            Loc_Reg|=USART_RXNE_INT_DISABLE;
            USART6->CR1=Loc_Reg;

            USART_RxBuffer[USART_CH6].state = ready;
            USART_RxBuffer[USART_CH6].pos = 0;
            /*Check on NULL*/
            if(USART_RxBuffer[USART_CH6].cbf)
            {
                USART_RxBuffer[USART_CH6].cbf();
            }
        }
    }
}
