/*
 * File Name: App6.h
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
        RetError = LED_SetState(RED_LED, LED_ON);
    }
    else
    {
        RetError=LED_SetState(RED_LED,LED_OFF);
    }
       //     RetError = LED_SetState(RED_LED, LED_ON);
     
}

void AppSWitch_Runnable(void)
{
    ErrorStatus_t ReturnState;
    uint8_t SwitchState[6]={SWITCH_RELEASED,SWITCH_RELEASED,SWITCH_RELEASED,SWITCH_RELEASED,SWITCH_RELEASED,SWITCH_RELEASED};
    /************************Not Work***************************
    for (uint8_t SwitchNum = 0; SwitchNum < _HSWITCHES_NUM; SwitchNum++)
    {
        ReturnState=HSwitch_GetState(SwitchNum,&(SwitchState[SwitchNum]));
        if(SwitchState[SwitchNum]==SWITCH_PRESSED)
        {
            ToggleLed1();
        }
    }*/
     
     ReturnState=HSwitch_GetState(MODE_OK_SWITCH,&(SwitchState[MODE_OK_SWITCH]));
     if(SwitchState[MODE_OK_SWITCH]==SWITCH_PRESSED)
        {
            ToggleLed1();
        }

    ReturnState=HSwitch_GetState(UP_SWITCH,&(SwitchState[UP_SWITCH]));
     if(SwitchState[UP_SWITCH]==SWITCH_PRESSED)
        {
            ToggleLed1();
        }
    ReturnState=HSwitch_GetState(EDIT_SWITCH,&(SwitchState[EDIT_SWITCH]));
     if(SwitchState[EDIT_SWITCH]==SWITCH_PRESSED)
        {
            ToggleLed1();
        }
    ReturnState=HSwitch_GetState(RIGHT_START_STOP_SWITCH,&(SwitchState[RIGHT_START_STOP_SWITCH]));
     if(SwitchState[RIGHT_START_STOP_SWITCH]==SWITCH_PRESSED)
        {
            ToggleLed1();
        }
    ReturnState=HSwitch_GetState(LEFT_RESET_SWITCH,&(SwitchState[LEFT_RESET_SWITCH]));
     if(SwitchState[LEFT_RESET_SWITCH]==SWITCH_PRESSED)
        {
            ToggleLed1();
        }
     ReturnState=HSwitch_GetState(DOWN_PAUSE_CONTINUE_SWITCH,&(SwitchState[DOWN_PAUSE_CONTINUE_SWITCH]));
     if(SwitchState[DOWN_PAUSE_CONTINUE_SWITCH]==SWITCH_PRESSED)
        {
            ToggleLed1();
        }
    


    
   /* uint8_t SwitchState;
    ErrorStatus_t ReturnState=HSwitch_GetState(MODE_OK_SWITCH,&SwitchState);
    if(SwitchState==SWITCH_PRESSED)
    {
        ToggleLed1();
        //ReturnState=LED_SetState(LED1,LED_ON);
    }*/
}
int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOB);
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOC);
	ReturnError=LED_Init();
    ReturnError=HSwitch_Init();
    Scheduler_Init();
    Scheduler_Start();

    return 0;
}
#endif
#endif
