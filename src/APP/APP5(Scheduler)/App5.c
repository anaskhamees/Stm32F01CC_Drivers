/*
 * File Name: App5.h
 * Layer	: App
 * Version  : V 0.0
 * Created	:  Mar 14, 2024
 * Author	: Anas Khamees
 */
#include "APP/APP.h"
#ifdef APP
#if (APP==APP5_SCHEDULER_TEST)
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LED/LED.h"
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
void ToggleLed2(void) {
    static uint32_t State = 0;
     ErrorStatus_t RetError;
    State ^= 1; 
    if(State==1)
    {
        RetError = LED_SetState(LED2, LED_ON);
    }
    else
    {
        RetError=LED_SetState(LED2,LED_OFF);
    }    
}

int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);
	ReturnError=LED_Init();
    Scheduler_Init();
    Scheduler_Start();

    return 0;
}
#endif
#endif
