/*
 * File Name: App5.h
 * Layer	: App
 * Version  : V 0.0
 * Created	:  Mar 14, 2024
 * Author	: Anas Khamees
 */
#include "APP/APP.h"
#ifdef APP
#if (APP==APP6_SWITCH_DEBOUNCING)
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LED/LED.h"
#include "HAL/Switch_Scheduler/SwitchSched.h"
#include "Scheduler/Scheduler.h"

void ToggleLed1(void) {
    static uint32_t State = 0;
     ErrorStatus_t RetError;
    State ^= 1; 
    if(State==1)
    {
        RetError = LED_SetState(LED1, LED_ON);
    }
    else
    {
        RetError=LED_SetState(LED1,LED_OFF);
    }
     
}

void AppSWitch_Runnable(void)
{
    uint8_t SwitchState;
    ErrorStatus_t ReturnState=HSwitch_GetState(HSWITCH_1,&SwitchState);
    if(SwitchState==SWITCH_PRESSED)
    {
        //ToggleLed1();
        ReturnState=LED_SetState(LED1,LED_ON);
    }
}
int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);
	ReturnError=LED_Init();
    ReturnError=HSwitch_Init();
    Scheduler_Init();
    Scheduler_Start();

    return 0;
}
#endif
#endif
