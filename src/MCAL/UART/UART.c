
/**********************************************************************************/
/*										Includes				     			  */
/**********************************************************************************/
#include "Std_Lib/Std_Libraries.h"
#include "USART.h"
#include "USART_Validation.h"
/***********************************************************************************/
/*										Defines									   */
/***********************************************************************************/
#define USART_REG_CLEAR_MASK            (0x0000FFFFUL)
#define USART_TXE_FLAG_MASK             (0x00000080UL)
#define USART_RXNE_FLAG_MASK            (0x00000020UL)
#define _USART_CHANNELS_NUM             (3UL)
#define USART1_FREQ                     (16000000UL)
#define USART2_FREQ                     (16000000UL)
#define USART6_FREQ                     (16000000UL)
#define SHIFT_BY_4                      (4UL)
#define MAX_INT32_VALUE                 (4294967295UL)
/***********************************************************************************/
/*										Types									   */
/***********************************************************************************/
typedef struct 
{
    uint32_t SR  ;
    uint32_t DR  ;
    uint32_t BRR ;
    uint32_t CR1 ;
    uint32_t CR2 ;
    uint32_t CR3 ;
    uint32_t GTPR;
}USART_Registers_t;

typedef struct 
{
    uint8_t* data    ;
    uint32_t position;
    uint32_t Length  ;
    uint8_t ReqState ;
    CallBack_t CB    ;
}Buffer_t;

/************************************************************************************/
/*									Variables									    */
/************************************************************************************/

volatile USART_Registers_t*const USARTs[_USART_CHANNELS_NUM] = { ((volatile USART_Registers_t*const)0x40011000),
                                                                 ((volatile USART_Registers_t*const)0x40004400),
                                                                 ((volatile USART_Registers_t*const)0x40011400)
                                                               };

const float32_t ClockFreq[_USART_CHANNELS_NUM]= {USART1_FREQ,USART2_FREQ,USART6_FREQ};
Buffer_t TxBuffer[_USART_CHANNELS_NUM]={{.ReqState=USART_READY},{.ReqState=USART_READY},{.ReqState=USART_READY}};
Buffer_t RxBuffer[_USART_CHANNELS_NUM]={{.ReqState=USART_READY},{.ReqState=USART_READY},{.ReqState=USART_READY}};

/************************************************************************************/
/*                            APIs Implementation								    */
/************************************************************************************/

ErrorStatus_t USART_Init(USART_CFG_t* USART_Config)
{
    ErrorStatus_t ReturnError=USART_NOK;
    if(USART_Config==NULL)
    {
        ReturnError=USART_NULL_PTR;
    }
    else if((USART_Config->USART_ID)>USART6)
    {
        ReturnError=USART_INVALID_USART_CHANNEL;
    }
    else if(!IS_DATA_BITS_VALID(USART_Config->DataBits))
    {
        ReturnError=USART_INVALID_WORD_LENGTH;
    }
    else if(!IS_OVER_SAMPLE_VALID(USART_Config->OverSample))
    {
        ReturnError=USART_INVALID_SAMPLING;
    }
    else if(!IS_BIT_SAMPLE_METHOD(USART_Config->BitSampleMethod))
    {
        ReturnError=USART_INVALID_SAMPLING;
    }
    else if (!IS_STOP_BITS_VALID(USART_Config->StopBits))
    {
        ReturnError=USART_INVALID_STOP_BITS;
    }
    else
    {
        uint32_t OVER8= (USART_Config->OverSample==USART_OVERSAMPLING_16)?0:1;
        /*
         * @brief : Baud Rate Calculations based on Ref Manual Pg.519 - 520
         *
         *                  Clock Frequency
         *   BaudRate = ---------------------------------
                            8 x (2 - OVER8) x USARTDIV
         *
         *  The Min Value of USARTDIV is 1, So that We can deduce the Max BaudRate is :
         * 
         *                Clock Frequency
         *   BaudRate = ------------------------------ ,, OVER8 can be (0 or 1).
                            8x(2 - OVER8)
         */

        float32_t MaxBaudRate= ((ClockFreq[USART_Config->USART_ID])/(float32_t)(8*(2-OVER8)));
        
        if ((USART_Config->BaudRate) >MaxBaudRate)
        {
            ReturnError= USART_INVALID_BAUDRATE;
        }
        else
        {
            /* 1. Read the Register(Control Register 1)     */
            uint32_t LocRegister= USARTs[USART_Config->USART_ID]->CR1;
            /* 2. Clear all the Register (Reset Value)      */
            LocRegister&=~USART_REG_CLEAR_MASK;
            /* 3. Enable USART Peripheral                   */
            LocRegister|=((1UL)<<UE);
            /* 4. Configure Over Sampling                   */
            LocRegister|=USART_Config->OverSample;
            /* 5. Configure Data (word) Length              */
            LocRegister|=USART_Config->DataBits;    
            /* 6. Parity Configuration                      */
            LocRegister|=USART_Config->Parity; 
            /* 7. Write to Control Register 1 again         */
            USARTs[USART_Config->USART_ID]->CR1= LocRegister;
            /*----------------------------------------------*/
            /* 1. Read Control Register 2                   */
            LocRegister=USARTs[USART_Config->USART_ID]->CR2;
            /* 2. Clear all the Register bits (Reset Value) */
            LocRegister&=~USART_REG_CLEAR_MASK;
            /* 3. Configure Stop Bits                       */
            LocRegister|=USART_Config->StopBits;
            /* 4. Write to Control Register 2 again         */
            USARTs[USART_Config->USART_ID]->CR2=LocRegister;
            /*-----------------------------------------------*/
            /* 1. Read Control Register 3                    */
            LocRegister=USARTs[USART_Config->USART_ID]->CR3;
            /* 2. Clear all the Register bits (Reset Value) */
            LocRegister&=~USART_REG_CLEAR_MASK;
            /* 3. Configure Bit Sampling Method              */
            LocRegister|=USART_Config->BitSampleMethod ;
            /* 4. Write to Control Register 3 again  */
            USARTs[USART_Config->USART_ID]->CR3=LocRegister;
            /*------------- BaudRate Configuration ----------*/

            /*                      Clock Frequency
             *   USARTDIV = ---------------------------------
             *                  8 x (2 - OVER8) x BaudRate
             */
            float32_t USARTDIV=((float32_t)(ClockFreq[USART_Config->USART_ID])/(float32_t)(8*(2-OVER8)*(USART_Config->BaudRate)));
            /* The mantissa is the whole part of number (integer)       */
            uint32_t Mantissa =(uint32_t)USARTDIV;
            float32_t Fraction=USARTDIV-Mantissa;
            /* OverSample16 --> 4 bits (16) ,, OverSample8 --> 3 bits (8)*/
            float32_t MaxFractionBits=(USART_Config->OverSample==USART_OVERSAMPLING_16)?16:8;
            /* +1 for:  if the result =(10.5 float)=(10 int) ~ 11        */
            uint32_t DIVfraction=(MaxFractionBits*Fraction)+1;
            /* The Max Value Can be stored in 4bits or 3 bits            */
            uint32_t MaxFractionBitsValue=(USART_Config->OverSample==USART_OVERSAMPLING_16)?15:7;
            /* If the Value exceed than 7 (3bits) or 15 (4bits)          */
            if(DIVfraction>MaxFractionBitsValue)
            {
                DIVfraction=0;
                Mantissa++;
            }
            /* 1. Read BaudRate Register                 */
            LocRegister=USARTs[USART_Config->USART_ID]->BRR;
            /* 2. Clear the Register                     */
            LocRegister&=~USART_REG_CLEAR_MASK;
            /* 3. Calculate the USARTDIV Register Value  */               
            LocRegister|=((Mantissa<<SHIFT_BY_4)|DIVfraction);
            /* 4. Write to Register again                */
            USARTs[USART_Config->USART_ID]->BRR=LocRegister;

            /* Enable the Transmitter (Idle State)       */
            USARTs[USART_Config->USART_ID]->CR1 |=(1<<TE);
            /* Enable The Receiver waiting for Start Bit */
            USARTs[USART_Config->USART_ID]->CR1 |=(1<<RE);
            ReturnError=USART_OK;
        }
    }

    return ReturnError;
}

ErrorStatus_t USART_SendBufferAsynchZeroCopy(uint8_t USART_ID,uint8_t* Buffer,uint32_t BufferLength,CallBack_t cbFun)
{
    ErrorStatus_t ReturnError=USART_NOK;
    if(Buffer==NULL)
    {
        ReturnError=USART_NULL_PTR;
    }
    /*else if(cbFun==NULL)
    {
        ReturnError=USART_NULL_PTR;
    }*/
    else if((USART_ID)>USART6)
    {
        ReturnError=USART_INVALID_USART_CHANNEL;
    }
   else if (BufferLength>MAX_INT32_VALUE)
   {
        ReturnError=USART_NOK;
   }
   else
   {
        /* Don't enable TXE HERE, Because the Reset value of Data Register is Zero (Empty) to prevent generate useless Interrupt */
        if(TxBuffer[USART_ID].ReqState==USART_READY)
        {
            TxBuffer[USART_ID].ReqState= USART_BUSY  ;
            TxBuffer[USART_ID].data    = Buffer      ;
            TxBuffer[USART_ID].Length  = BufferLength;
            TxBuffer[USART_ID].position= 0           ;
            TxBuffer[USART_ID].CB      = cbFun       ;
            /* To generate Tx Interrupt First Time Only to go to the Handler, We should Write The first byte in Data Register */
            USARTs[USART_ID]->DR= TxBuffer[USART_ID].data[0];
            TxBuffer[USART_ID].position++;
            /* Enable Transmit data register Empty TXE Interrupt */
            USARTs[USART_ID]->CR1 |=(1<<TXEIE);
            /* Enable Transmit Complete TCE Interrupt            */
            USARTs[USART_ID]->CR1 |=(1<<TCIE);
            ReturnError=USART_OK;
        }
        else
        {
            /* No thing to do but for MISRA */
        }
   }
   return ReturnError;
}

ErrorStatus_t USART_ReceiveBufferAsynchZeroCopy(uint8_t USART_ID,uint8_t* Buffer,uint32_t BufferLength,CallBack_t cbFun)
{
    ErrorStatus_t ReturnError=USART_NOK;
     if(Buffer==NULL)
    {
        ReturnError=USART_NULL_PTR;
    }
    /*else if(cbFun==NULL)
    {
        ReturnError=USART_NULL_PTR;
    }*/
    else if((USART_ID)>USART6)
    {
        ReturnError=USART_INVALID_USART_CHANNEL;
    }
   else if (BufferLength>MAX_INT32_VALUE)
   {
        ReturnError=USART_NOK;
   }
   else
   {
        if(RxBuffer[USART_ID].ReqState==USART_READY)
        {
            /* Disable Read Data Register Not Empty RXNE Interrupt */
            USARTs[USART_ID]->CR1 &=~(1<<RXNEIE)  ;
            RxBuffer[USART_ID].ReqState=USART_BUSY;
            RxBuffer[USART_ID].CB=cbFun;
            RxBuffer[USART_ID].data=Buffer;
            RxBuffer[USART_ID].Length=BufferLength;
            RxBuffer[USART_ID].position=0;
            /* Enable Read Data Register Not Empty RXNE Interrupt */ 
            USARTs[USART_ID]->CR1 |=(1<<RXNEIE) ;
            ReturnError=USART_OK;
        }
   }
   return ReturnError;
}

ErrorStatus_t USART_SendByte(uint8_t USART_ID,uint8_t Byte)
{
    ErrorStatus_t ReturnError=USART_NOK;
    if(USART_ID>USART6)
    {
        ReturnError=USART_INVALID_USART_CHANNEL;
    }
    else
    {
       if(TxBuffer[USART_ID].ReqState==USART_READY)
       {
            TxBuffer[USART_ID].ReqState=USART_BUSY;
            /*Write the Byte in Data Register to be transmit*/
            USARTs[USART_ID]->DR=Byte;
            /* Read TXE Bit for Ensure that Interrupt occured or Not */
            //uint8_t volatile TXE_BitValue=(((USARTs[USART_ID]->SR)>>TXE)&(1UL));
            uint32_t volatile TimeOut=3000;
            while (TimeOut&&(!((USARTs[USART_ID]->SR)&(USART_TXE_FLAG_MASK))))
            {
                TimeOut--;
            }
            if(TimeOut==0)
            {
                ReturnError=USART_TX_TIMEOUT;
            }
            else
            {
                /* Transmission Done !*/
                ReturnError=USART_OK;
            }
            
            TxBuffer[USART_ID].ReqState=USART_READY;
        }
    }
    return ReturnError;
}

ErrorStatus_t USART_ReceiveByte(uint8_t USART_ID,uint8_t* Byte)
{
    ErrorStatus_t ReturnError=USART_NOK;
    if(USART_ID>USART6)
    {
        ReturnError=USART_INVALID_USART_CHANNEL;
    }
    else if (Byte ==NULL)
    {
        ReturnError=USART_NULL_PTR;
    }
    else
    {
        if(RxBuffer[USART_ID].ReqState==USART_READY)
        {
            RxBuffer[USART_ID].ReqState=USART_BUSY;
           // uint8_t RXNE_BitValue=(((USARTs[USART_ID]->SR)>>RXNE)&(1UL));
            uint32_t TimeOut=2000;
            while (TimeOut&&(!((USARTs[USART_ID]->SR)&(USART_RXNE_FLAG_MASK))))
            {
                TimeOut--;
            }
            if(TimeOut==0)
            {
                ReturnError=USART_TX_TIMEOUT;
            }
            else
            {
                *Byte=USARTs[USART_ID]->DR;
                 ReturnError=USART_OK;
            }

            RxBuffer[USART_ID].ReqState=USART_READY;
        }
        else
        {
            /* No thing to do but For MISAR */
        }
    }
    return ReturnError;
}

void USART1_IRQHandler(void)
{
    static uint32_t counter=0;
    // volatile uint8_t RXNE_BitValue=(((USARTs[USART1]->SR)>>RXNE)&(1UL));
    // volatile uint8_t TXE_BitValue =(((USARTs[USART1]->SR)>>TXE)&(1UL));

    /* If the Transmission is the source of interrupt */
    if((USARTs[USART1]->SR)&(USART_TXE_FLAG_MASK))
    {
        /* If There is data to be sent */
        if(TxBuffer[USART1].position<TxBuffer[USART1].Length)
        {
            /* Write the next byte in Data register to be send */
            USARTs[USART1]->DR=TxBuffer[USART1].data[TxBuffer[USART1].position];
            /* Update the Byte Index in the Buffer             */
            TxBuffer[USART1].position++;
        }
        else
        {
           
            /* Disable Transmit data register Empty TXE Interrupt   */
            USARTs[USART1]->CR1 &=~(1<<TXEIE);
            /* Disable Transmit Complete TCE Interrupt              */
            USARTs[USART1]->CR1 &=~(1<<TCIE);
            /* Set the Request state to be ready to receive requests*/
            TxBuffer[USART1].ReqState=USART_READY;
            /* Check Call Back Function                             */
            if(TxBuffer[USART1].CB)
            {
                TxBuffer[USART1].CB();
            }
        }
    }
    else
    {
        /* Nothing to Do But for MISRA */
    }

    if((USARTs[USART1]->SR)&(USART_RXNE_FLAG_MASK))
    {
        /* Check the Buffer is Full or Not */
        if(RxBuffer[USART1].position<RxBuffer[USART1].Length)
        {
            /* RXNE Flag is Cleared Automatic when read data register */
            RxBuffer[USART1].data[RxBuffer[USART1].position]=USARTs[USART1]->DR;
            RxBuffer[USART1].position++;
            if (RxBuffer[USART1].position==RxBuffer[USART1].Length)
            {
                /*------------- Receive a Buffer is DONE ! ----------------*/
                /* Disable RXNE Interrupt to prevent triggering useless interrupt */
                USARTs[USART1]->CR1 &=~(1<<RXNEIE)  ;
                /* Set the Request state to be ready to receive requests          */
                RxBuffer[USART1].ReqState=USART_READY;
                /* Check Call Back  */
                if(RxBuffer[USART1].CB)
                {
                    RxBuffer[USART1].CB();
                }
                else
                {
                    /* Nothing to do but for MISRA */
                }
            }
            else
            {
                /* Nothing to do but for MISRA */
            }
            
        }
        else
        {
            /* Nothing to do but for MISRA */
        }
    }
    else
    {
        /* Nothing to do but for MISRA */
    }
    counter++;
}


void USART2_IRQHandler(void)
{
    // uint8_t RXNE_BitValue=(((USARTs[USART2]->SR)>>RXNE)&(1UL));
    // uint8_t TXE_BitValue =(((USARTs[USART2]->SR)>>TXE)&(1UL));

    /* If the Transmission is the source of interrupt */
    if((USARTs[USART1]->SR)&(USART_TXE_FLAG_MASK))
    {
        /* If There is data to be sent */
        if(TxBuffer[USART2].position<TxBuffer[USART2].Length)
        {
            /* Write the next byte in Data register to be send */
            USARTs[USART2]->DR=TxBuffer[USART2].data[TxBuffer[USART2].position];
            /* Update the Byte Index in the Buffer             */
            TxBuffer[USART2].position++;
        }
        else
        {
           
            /* Disable Transmit data register Empty TXE Interrupt   */
            USARTs[USART2]->CR1 &=~(1<<TXEIE);
            /* Disable Transmit Complete TCE Interrupt              */
            USARTs[USART2]->CR1 &=~(1<<TCIE);
            /* Set the Request state to be ready to receive requests*/
            TxBuffer[USART2].ReqState=USART_READY;
            /* Check Call Back Function                             */
            if(TxBuffer[USART2].CB)
            {
                TxBuffer[USART2].CB();
            }
        }
    }
    else
    {
        /* Nothing to Do But for MISRA */
    }

    if((USARTs[USART1]->SR)&(USART_RXNE_FLAG_MASK))
    {
        /* Check the Buffer is Full or Not */
        if(RxBuffer[USART2].position<RxBuffer[USART2].Length)
        {
            /*RXNE Flag is Cleared Automatic when read data register*/
            RxBuffer[USART2].data[RxBuffer[USART2].position]=USARTs[USART2]->DR;
            RxBuffer[USART2].position++;
            if (RxBuffer[USART2].position==RxBuffer[USART2].Length)
            {
                /*------------- Receive a Buffer is DONE ! ----------------*/
                /* Disable RXNE Interrupt to prevent triggering useless interrupt */
                USARTs[USART2]->CR1 &=~(1<<RXNEIE)  ;
                /* Set the Request state to be ready to receive requests          */
                RxBuffer[USART2].ReqState=USART_READY;
                /* Check Call Back  */
                if(RxBuffer[USART2].CB)
                {
                    RxBuffer[USART2].CB();
                }
                else
                {
                    /* Nothing to do but for MISRA */
                }
            }
            else
            {
                /* Nothing to do but for MISRA */
            }
            
        }
        else
        {
            /* Nothing to do but for MISRA */
        }
    }
    else
    {
        /* Nothing to do but for MISRA */
    }
}

void USART6_IRQHandler(void)
{
    // uint8_t RXNE_BitValue=(((USARTs[USART6]->SR)>>RXNE)&(1UL));
    // uint8_t TXE_BitValue =(((USARTs[USART6]->SR)>>TXE)&(1UL));

    /* If the Transmission is the source of interrupt */
    if((USARTs[USART1]->SR)&(USART_TXE_FLAG_MASK))
    {
        /* If There is data to be sent */
        if(TxBuffer[USART6].position<TxBuffer[USART6].Length)
        {
            /* Write the next byte in Data register to be send */
            USARTs[USART6]->DR=TxBuffer[USART6].data[TxBuffer[USART6].position];
            /* Update the Byte Index in the Buffer             */
            TxBuffer[USART6].position++;
        }
        else
        {
           
            /* Disable Transmit data register Empty TXE Interrupt   */
            USARTs[USART6]->CR1 &=~(1<<TXEIE);
            /* Disable Transmit Complete TCE Interrupt              */
            USARTs[USART6]->CR1 &=~(1<<TCIE);
            /* Set the Request state to be ready to receive requests*/
            TxBuffer[USART6].ReqState=USART_READY;
            /* Check Call Back Function                             */
            if(TxBuffer[USART6].CB)
            {
                TxBuffer[USART6].CB();
            }
        }
    }
    else
    {
        /* Nothing to Do But for MISRA */
    }

    if((USARTs[USART1]->SR)&(USART_RXNE_FLAG_MASK))
    {
        /* Check the Buffer is Full or Not */
        if(RxBuffer[USART6].position<RxBuffer[USART6].Length)
        {
            /*RXNE Flag is Cleared Automatic when read dara register*/
            RxBuffer[USART6].data[RxBuffer[USART6].position]=USARTs[USART6]->DR;
            RxBuffer[USART6].position++;
            if (RxBuffer[USART6].position==RxBuffer[USART6].Length)
            {
                /*------------- Receive a Buffer is DONE ! ----------------*/
                /* Disable RXNE Interrupt to prevent triggering useless interrupt */
                USARTs[USART6]->CR1 &=~(1<<RXNEIE)  ;
                /* Set the Request state to be ready to receive requests          */
                RxBuffer[USART6].ReqState=USART_READY;
                /* Check Call Back  */
                if(RxBuffer[USART6].CB)
                {
                    RxBuffer[USART6].CB();
                }
                else
                {
                    /* Nothing to do but for MISRA */
                }
            }
            else
            {
                /* Nothing to do but for MISRA */
            }
            
        }
        else
        {
            /* Nothing to do but for MISRA */
        }
    }
    else
    {
        /* Nothing to do but for MISRA */
    }
}