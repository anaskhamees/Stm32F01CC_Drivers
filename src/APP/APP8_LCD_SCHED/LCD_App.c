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


/******************************************************************************************/
/*                                Application Function                                    */
/******************************************************************************************/

/* Each 10mSec */
void LCD_App(void)
{
    static uint32_t time=0;
    time++;
     ErrorStatus_t ReturnError;
    switch (time)
    {
    case 1:ReturnError=LCD_SetCursorPosAsynch(LCD1,0,6,NULL);break;
    case 2:ReturnError=LCD_WriteStringAsynch(LCD1,"Anas ",5,NULL);break;
    case 3:ReturnError=LCD_SetCursorPosAsynch(LCD1,1,9,NULL);break;
    case 5:ReturnError=LCD_WriteStringAsynch(LCD1,"Belal",5,NULL);break;
    case 6:ReturnError=LCD_SetCursorPosAsynch(LCD1,0,1,NULL);break;
    
    case 7:ReturnError=LCD_WriteStringAsynch(LCD1,"ali",3,NULL);break;
    case 8:ReturnError=LCD_SetCursorPosAsynch(LCD1,1,1,NULL);break;
    case 9: ReturnError=LCD_WriteStringAsynch(LCD1,"mohamed",7,NULL);break;
    default: 
        break;
    }    
    
//  ReturnError=LCD_SetCursorPosAsynch(LCD1,0,6,NULL);
//  ReturnError=LCD_WriteStringAsynch(LCD1,"Anas ",5,NULL);
//  ReturnError=LCD_SetCursorPosAsynch(LCD1,1,6,NULL);
//  ReturnError=LCD_WriteStringAsynch(LCD1,"Belal",5,NULL);
//  ReturnError=LCD_SetCursorPosAsynch(LCD1,0,1,NULL);
 
//  ReturnError=LCD_WriteStringAsynch(LCD1,"mohamed",7,NULL);
//  ReturnError=LCD_SetCursorPosAsynch(LCD1,1,1,NULL);
//  ReturnError=LCD_WriteStringAsynch(LCD1,"ali",3,NULL);

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
