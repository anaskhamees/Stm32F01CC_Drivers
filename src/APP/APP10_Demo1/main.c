#include "APP/APP.h"
#ifdef APP
#if (APP==APP10_DEMO1)

#include "MCAL/UART/USART.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/NVIC/NVIC.h"
#include "MCAL/NVIC/STM32F401xx.h"
#include "MCAL/UART/USART.h"
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LCD_Scheduler/LCD.h"
#include "Scheduler/Scheduler.h"

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
USART_CFG_t UART2_CFG=
{
    .BaudRate=9600,
    .BitSampleMethod=USART_SAMPLE_BIT3,
    .DataBits=USART_DATA_BITS_8,
    .OverSample=USART_OVERSAMPLING_16,
    .Parity=USART_PARITY_NONE,
    .StopBits=UART_STOP_BITS_ONE,
    .USART_ID=USART2
};

int main(int argc, char* argv[])
{
    ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOB);
    ReturnError=HAL_PeripheralEnableCLK(HAL_USART2);
    ReturnError=GPIO_InitPin(&UART_TX_PIN);
    ReturnError=GPIO_InitPin(&UART_RX_PIN);
    ReturnError=NVIC_EnableIRQ(NVIC_USART2_INTERRUPT);
    ReturnError=USART_Init(&UART2_CFG);

    LCD_Init(LCD1);
    APP_Init();

    Scheduler_Init();
    Scheduler_Start();

    return 0;
}
#endif
#endif