/*
 * File Name: App7.h
 * Layer	: App
 * Version  : V 0.0
 * Created	:  Mar 16, 2024
 * Author	: Anas Khamees
 */
#include "APP/APP.h"
#ifdef APP
#if (APP==APP8_LCD_SCHED_APP)
/******************************************************************************************/
/*************************************** Includes *****************************************/
/******************************************************************************************/
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LCD_Scheduler/LCD.h"
#include "Scheduler/Scheduler.h"
/******************************************************************************************/
/**************************************** Defines *****************************************/
/******************************************************************************************/
#define SET_POS   1
#define WAIT1     2
#define WRITE     3
#define WAIT2     4
#define CLEAR     5
#define WAIT3     6

/******************************************************************************************/
/*                                Application Function                                    */
/******************************************************************************************/
//Runnable Each 100ms
void LCD_App(void)
{
    static uint8_t appState=SET_POS;
    static uint8_t counter=0;
    counter++;
    ErrorStatus_t ReturnError;


    switch (appState)
    {
    case SET_POS:
       ReturnError=LCD_SetCursorPosAsynch(LCD1,0,6,NULL);
       appState=WAIT1;
        break;
    case WAIT1:
        if((LCD_GetState(LCD1)==LCD_READY))
        {
            appState=WRITE;
        }
        break;
    case WRITE:
       ReturnError=LCD_WriteStringAsynch(LCD1,"Anas ",5,NULL);
       appState=WAIT2;
       break;
    case WAIT2:
       if((LCD_GetState(LCD1)==LCD_READY)&&(counter==10)) /* Each 1000 mSec Clear the LCD */
        {
            appState=CLEAR;
            counter=0;
        }
        break;
    case CLEAR:
        LCD_ClearScreenAsynch(LCD1,NULL);
        appState=WAIT3;
        break;
    case WAIT3:
        if(LCD_GetState(LCD1)==LCD_READY)
        {
            appState=SET_POS;
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
    LCD_Init(LCD1);
    Scheduler_Init();
    Scheduler_Start();

    return 0;
}
#endif
#endif
