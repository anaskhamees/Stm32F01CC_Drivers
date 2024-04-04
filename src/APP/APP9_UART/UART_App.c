/*
 * File Name: App7.h
 * Layer	: App
 * Version  : V 0.0
 * Created	:  Mar 16, 2024
 * Author	: Anas Khamees
 */
#include "APP/APP.h"
#ifdef APP
#if (APP==APP9_UART_TEST)
/******************************************************************************************/
/*                                       Includes                                         */
/******************************************************************************************/
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/NVIC/NVIC.h"
#include "MCAL/NVIC/STM32F401xx.h"
#include "MCAL/UART/USART.h"
#include "HAL/LED/LED.h"
/******************************************************************************************/
/*                                        Defines                                         */
/******************************************************************************************/


/******************************************************************************************/
/*                                Variables                                               */
/******************************************************************************************/

/**
 * GPIO Configurations As Alternative Function
 *      - USART1 ---> Tx Pin PA9  ,, Rx Pin PA10 ,, AF07 .
 *      - USART2 ---> Tx Pin PA2  ,, Rx Pin PA3  ,, AF07 .
 *      - USART6 ---> Tx Pin PA11 ,, Rx Pin PA12 ,, AF08 .
 */

/*------------------- Configure PA9 As TX pin ------------------*/
GPIO_CFG_t UART_TX_PIN=
{
    .GPIO_AF   = GPIO_AF07,
    .GPIO_Mode = GPIO_AF_PP_NO_PUPD,
    .GPIO_Pin  = GPIO_PIN6,
    .GPIO_Port = GPIO_PORTB,
    .GPIO_Speed= GPIO_HIGH_SPEED   
};

/*------------------ Configure PA10 As RX pin ------------------*/
GPIO_CFG_t UART_RX_PIN=
{
    .GPIO_AF   = GPIO_AF07,
    .GPIO_Mode = GPIO_AF_PP_NO_PUPD,
    .GPIO_Pin  = GPIO_PIN7,
    .GPIO_Port = GPIO_PORTB,
    .GPIO_Speed= GPIO_HIGH_SPEED   
};

/*--------------- Configure USART1 Peripheral ------------------*/
USART_CFG_t UART1_CFG=
{
    .BaudRate=9600,
    .BitSampleMethod=USART_SAMPLE_BIT3,
    .DataBits=USART_DATA_BITS_8,
    .OverSample=USART_OVERSAMPLING_16,
    .Parity=USART_PARITY_NONE,
    .StopBits=UART_STOP_BITS_ONE,
    .USART_ID=USART1
};

uint8_t Buffer[5]={'A','B','C','D','E'};

/*------------------- USART1 CallBack Fun --------------------*/
void TurnLedON(void)
{
    ErrorStatus_t RetError=LED_SetState(RED_LED,LED_ON);
}

int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOB);
    ReturnError=HAL_PeripheralEnableCLK(HAL_USART1);
    ReturnError=GPIO_InitPin(&UART_TX_PIN);
    ReturnError=GPIO_InitPin(&UART_RX_PIN);
    ReturnError=NVIC_EnableIRQ(NVIC_USART1_INTERRUPT);
    ReturnError=ReturnError=LED_Init();
    ReturnError=USART_Init(&UART1_CFG);
    //ReturnError=USART_SendBufferAsynchZeroCopy(USART1,Buffer,5,TurnLedON);
    
    while (1)
    {
        ReturnError=USART_SendByte(USART1,'A');
    }
     
    return 0;
}
#endif
#endif
