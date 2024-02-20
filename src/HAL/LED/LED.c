// /*
//  * File Name: LED.c
//  * Layer	: HAL
//  * Module	: LED
//  * Version  : V 0.0
//  * Created	:  Feb 19, 2024
//  * Target   : Stm32F401CC (BlackBill)
//  * Author	: Anas Khamees
//  */

// #include "MCAL/GPIO/GPIO.h"
// #include"LED.h"

// extern LedCFG_t LEDs[_Led_Number];

// ErrorStatus_t LED_Init(void)
// {
//     ErrorStatus_t ReturnError;
//     GPIO_CFG_t Led;
//     Led.GPIO_Mode=GPIO_OUT_PP_NO_PUPD;
//     Led.GPIO_Speed=GPIO_LOW_SPEED;
//     uint8_t LedNum;
//     for(LedNum=0;LedNum<_Led_Number;LedNum++)
//     {
//         Led.GPIO_Pin=LEDs[LedNum].Led_Pin;
//         Led.GPIO_Port=LEDs[LedNum].Led_Port;
//         ReturnError=GPIO_InitPin(&Led);
//         ReturnError=GPIO_SetPinValue(LEDs[LedNum].Led_Port,LEDs[LedNum].Led_Pin,(LEDs[LedNum].Led_InitialState)^(LEDs[LedNum].Led_Connection));
//     }

//     return ReturnError;
// }

// ErrorStatus_t LED_SetState(uint32_t LedNum,uint32_t LedState)
// {
//     ErrorStatus_t ReturnState;
//     if((LedNum<0 || LedNum>_Led_Number)||(LedState !=LED_ON && LedState!=LED_OFF))
//     {
//         ReturnState=WRONG_PARAMETER;
//     }
//     else
//     {
//         ReturnState=GPIO_SetPinValue(LEDs[LedNum].Led_Port,LEDs[LedNum].Led_Pin,LedState^(LEDs[LedNum].Led_Connection));
//     }
//     return ReturnState
// }