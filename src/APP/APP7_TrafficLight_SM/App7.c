/*
 * File Name: App7.h
 * Layer	: App
 * Version  : V 0.0
 * Created	:  Mar 16, 2024
 * Author	: Anas Khamees
 */
#include "APP/APP.h"
#ifdef APP
#if (APP==APP7_TRAFFIC_LIGHT)
/******************************************************************************************/
/*************************************** Includes *****************************************/
/******************************************************************************************/
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LED/LED.h"
#include "Scheduler/Scheduler.h"
/******************************************************************************************/
/**************************************** Defines *****************************************/
/******************************************************************************************/
#define LED_RED         (0)
#define LED_YELLOW      (1)
#define LED_GREEN       (2)
#define MS_TO_SECOND    (1000)
/* Time in Seconds*/
#define GREEN_TO_YELLOW    (6)
#define YELLOW_TO_GREEN    (2)
#define YELLOW_TO_RED      (2)
#define RED_TO_YELLOW      (10)
/******************************************************************************************/
/*************************************** Variables ****************************************/
/******************************************************************************************/
extern const UserRunnable_t UserRunnables[_MAX_RUNNABLE];
/******************************************************************************************/
/************************************ Static Function *************************************/
/******************************************************************************************/
static void RED_LedON(void)
{
    ErrorStatus_t RetError = LED_SetState(RED_LED, LED_ON);
     RetError = LED_SetState(YELLOW_LED, LED_OFF);
     RetError = LED_SetState(GREEN_LED, LED_OFF);
}
static void Yellow_LedON(void)
{
    ErrorStatus_t RetError = LED_SetState(YELLOW_LED, LED_ON);
     RetError = LED_SetState(RED_LED, LED_OFF);
     RetError = LED_SetState(GREEN_LED, LED_OFF);
}
static void Green_LedON(void)
{
    ErrorStatus_t RetError = LED_SetState(GREEN_LED, LED_ON);
     RetError = LED_SetState(YELLOW_LED, LED_OFF);
     RetError = LED_SetState(RED_LED, LED_OFF);
}

/******************************************************************************************/
/******************************** Application Function ************************************/
/******************************************************************************************/
//Runnable Each 2000ms
void TrafficLight(void)
{
    static uint32_t Seconds;
    Seconds+=((UserRunnables[TRAFFIC_LIGHT].PeriodicityMS)/MS_TO_SECOND);
    static uint8_t PreviousState;
    static uint8_t State=LED_GREEN;
    switch (State)
    {
    case LED_GREEN:
        Green_LedON();
        if(Seconds==GREEN_TO_YELLOW)
        {
            Seconds=0;
            PreviousState=LED_GREEN;
            State=LED_YELLOW;
        }
        break;
    case LED_YELLOW:
        Yellow_LedON();
        if((PreviousState==LED_GREEN)&&(Seconds==YELLOW_TO_RED))
        {
            PreviousState=LED_YELLOW;
            State=LED_RED;
            Seconds=0;    
        }
        else if((PreviousState==LED_RED)&&(Seconds==YELLOW_TO_GREEN))
        {
            PreviousState=LED_YELLOW;
            State=LED_GREEN;
            Seconds=0;  
        }
        break;
    case LED_RED:
        RED_LedON();
        if(Seconds==RED_TO_YELLOW)
        {
            PreviousState=LED_RED;
            State=LED_YELLOW;
            Seconds=0;
        }
        break;
    default: 
        break;
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
