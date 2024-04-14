/**
 * @ Author: Your name
 * @ Create Time: 2024-04-09 22:16:33
 * @ Modified by: Your name
 * @ Modified time: 2024-04-15 00:37:47
 * @ Description:
 */

#include "APP/APP.h"
#ifdef APP
#if (APP==APP10_DEMO1)

#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/NVIC_Driver/NVIC.h"
#include "MCAL/NVIC_Driver/STM32F401xx.h"
#include "HAL/IPC/IPC.h"
#include "HAL/LCD_Scheduler/LCD.h"
#include "SERVICES/SCHED/SCHED.h"

/*------------------- Configure PA2 As TX pin ------------------*/
GPIO_CFG_t UART_TX_PIN=
{
    .GPIO_AF   = GPIO_AF07,
    .GPIO_Mode = GPIO_AF_PP_NO_PUPD,
    .GPIO_Pin  = GPIO_PIN2,
    .GPIO_Port = GPIO_PORTA,
    .GPIO_Speed= GPIO_HIGH_SPEED   
};

/*------------------ Configure PA3 As RX pin ------------------*/
GPIO_CFG_t UART_RX_PIN=
{
    .GPIO_AF   = GPIO_AF07,
    .GPIO_Mode = GPIO_AF_PP_NO_PUPD,
    .GPIO_Pin  = GPIO_PIN3,
    .GPIO_Port = GPIO_PORTA,
    .GPIO_Speed= GPIO_HIGH_SPEED   
};


/*--------------- Configure USART2 Peripheral ------------------*/
// USART_CFG_t UART2_CFG=
// {
//     .BaudRate=9600,
//     .BitSampleMethod=USART_SAMPLE_BIT3,
//     ..=USART_DATA_BITS_8,
//     .OverSample=USART_OVERSAMPLING_16,
//     .Parity=USART_PARITY_NONE,
//     .StopBits=UART_STOP_BITS_ONE,
//     .USART_ID=USART2
// };


IPC_USART_CFG_t IPC_UART2_CFG =
{
    .IPC_USART_BaudRate        = 9600,
    //.IPC_USART_BitSampleMethod = USART_SAMPLE_BIT3,
    .IPC_USART_DataBits        = USART_DATA_BITS_8,
    .IPC_USART_ID              = USART_CH2,
    .IPC_USART_OverSample      = USART_OVERSAMPLING_16,
    .IPC_USART_Parity          = USART_PARITY_NONE,
    .IPC_USART_StopBits        = UART_STOP_BITS_ONE
};


int main(int argc, char* argv[])
{

    ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOB);
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOC);
    ReturnError=HAL_PeripheralEnableCLK(HAL_USART2);
    ReturnError=GPIO_InitPin(&UART_TX_PIN);
    ReturnError=GPIO_InitPin(&UART_RX_PIN);
    ReturnError=NVIC_EnableInterrupt(NVIC_USART2_INTERRUPT);
    //ReturnError=NVIC_EnableIRQ(NVIC_USART2_INTERRUPT);
    
    IPC_USART_Init(&IPC_UART2_CFG);
    
    ReturnError=HSwitch_Init();

    LCD_Init(LCD1);
    APP_Init();

    SCHED_Init();
    SCHED_Start();
    
    return 0;
}
#endif
#endif