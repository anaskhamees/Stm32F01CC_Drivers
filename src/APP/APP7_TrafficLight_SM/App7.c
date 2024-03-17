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
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LED/LED.h"
#include "Scheduler/Scheduler.h"
#define TASK_PERIODICITY    
void RED_LedON(void)
{
    ErrorStatus_t RetError = LED_SetState(RED_LED, LED_ON);
}
void Yellow_LedON(void)
{
    ErrorStatus_t RetError = LED_SetState(YELLOW_LED, LED_ON);
}
void Green_LedON(void)
{
    ErrorStatus_t RetError = LED_SetState(GREEN_LED, LED_ON);
}

typedef enum{
    Green,
    Yellow,
    Red
}LED_Color_t

static uint32_t Seconds;
Seconds+=

void TrafficLight(void)
{

}
int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOC);
	ReturnError=LED_Init();
    ReturnError=HSwitch_Init();
    Scheduler_Init();
    Scheduler_Start();

    return 0;
}
#endif
#endif
