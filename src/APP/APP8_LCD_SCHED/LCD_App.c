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
#define CLEAR   1
#define SET_POS 2
#define WRITE   3
/******************************************************************************************/
/*************************************** Variables ****************************************/
/******************************************************************************************/
extern const UserRunnable_t UserRunnables[_MAX_RUNNABLE];
/******************************************************************************************/
/************************************ Static Function *************************************/
/******************************************************************************************/

/******************************************************************************************/
/******************************** Application Function ************************************/
/******************************************************************************************/
//Runnable Each 100ms
ErrorStatus_t LCD_SetCursorPosAsynch(uint8_t LCD_Name, uint8_t PosX,uint8_t PosY,ReqCallBack_t CB);
ErrorStatus_t LCD_WriteStringAsynch(uint8_t LCD_Name, char_t* string,uint8_t length,ReqCallBack_t CB);

void LCD_App(void)
{
    static uint8_t appState=SET_POS;
    ErrorStatus_t ReturnError;

    switch (appState)
    {
    case SET_POS:
       ReturnError=LCD_SetCursorPosAsynch(LCD1,0,6,NULL);
        if(LCD_GetState(LCD1)==LCD_READY)
        {
            appState=WRITE;
        }
        break;
    case WRITE:
       ReturnError=LCD_WriteStringAsynch(LCD1,"Anas ",5,NULL);
        break;
    case CLEAR:
        LCD_ClearScreenAsynch(LCD1,NULL);
        if(LCD_GetState(LCD1)==LCD_READY)
        {
            appState=SET_POS;
        }
        break;
    default: 
        break;
    } 
/*uint8_t static counter=0;
counter+=TICK_TIME;
ReturnError=LCD_SetCursorPosAsynch(LCD1,1,5,NULL);
if(counter==40)
{
ReturnError=LCD_WriteStringAsynch(LCD1,"Anas",5,NULL);
}
if(counter==45)
{
    LCD_ClearScreenAsynch(LCD1,NULL);
}*/
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
