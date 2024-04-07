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

/* Each 1000mSec */
void LCD_App(void)
{
    static uint32_t time=0;
     ErrorStatus_t ReturnError;
    // switch (time)
    // {
    // case 1:ReturnError=LCD_SetCursorPosAsynch(LCD1,0,6,NULL);break;
    // case 2:ReturnError=LCD_WriteStringAsynch(LCD1,"Anas ",5,NULL);break;
    // case 3:ReturnError=LCD_SetCursorPosAsynch(LCD1,1,9,NULL);break;
    // case 5:ReturnError=LCD_WriteStringAsynch(LCD1,"Belal",5,NULL);break;
    // case 6:ReturnError=LCD_SetCursorPosAsynch(LCD1,0,1,NULL);break;
    
    // case 7:ReturnError=LCD_WriteStringAsynch(LCD1,"ali",3,NULL);break;
    // case 8:ReturnError=LCD_SetCursorPosAsynch(LCD1,1,1,NULL);break;
    // case 9: ReturnError=LCD_WriteStringAsynch(LCD1,"mohamed",7,NULL);break;
    // default: 
    //     break;
    // }    
    // ReturnError=LCD_SetCursorPosAsynch(LCD1,0,6,NULL);
    // ReturnError=LCD_WriteNumAsynch(LCD1,time,NULL);
    
//  ReturnError=LCD_SetCursorPosAsynch(LCD1,0,6,NULL);
//  ReturnError=LCD_WriteStringAsynch(LCD1,"Anas ",5,NULL);
//  ReturnError=LCD_SetCursorPosAsynch(LCD1,1,6,NULL);
//  ReturnError=LCD_WriteStringAsynch(LCD1,"Belal",5,NULL);
//  ReturnError=LCD_SetCursorPosAsynch(LCD1,0,1,NULL);
 
//  ReturnError=LCD_WriteStringAsynch(LCD1,"mohamed",7,NULL);
//  ReturnError=LCD_SetCursorPosAsynch(LCD1,1,1,NULL);
//  ReturnError=LCD_WriteStringAsynch(LCD1,"ali",3,NULL);

/*-------------------------- Stop Watch ------------------------*/
    static uint32_t seconds=0;
    static uint32_t minutes=59;
    static uint32_t hours=0;
    ReturnError=LCD_SetCursorPosAsynch(LCD1,0,3,NULL);
    ReturnError=LCD_WriteStringAsynch(LCD1,"Stop Watch",10,NULL);
    ReturnError=LCD_SetCursorPosAsynch(LCD1,1,4,NULL);
        seconds++;
        if (seconds > 59) 
        {
            seconds = 0;
            minutes++;
            if (minutes > 59) 
            {
                minutes = 0;
                hours++;
                if (hours > 23) 
                {
                    hours = 0;
                }
            }
        }
    
   // ReturnError=LCD_WriteNumAsynch(LCD1,0,NULL);
  
    
        ReturnError=LCD_WriteNumAsynch(LCD1,hours,NULL); 
        ReturnError=LCD_SetCursorPosAsynch(LCD1,1,6,NULL);
        
        ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);
        
        ReturnError=LCD_SetCursorPosAsynch(LCD1,1,7,NULL);
        ReturnError=LCD_WriteNumAsynch(LCD1,minutes,NULL);
        ReturnError=LCD_SetCursorPosAsynch(LCD1,1,9,NULL);
        
        ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);
        
        ReturnError=LCD_WriteNumAsynch(LCD1,seconds,NULL);
        ReturnError=LCD_SetCursorPosAsynch(LCD1,1,13,NULL);
    
  //  ReturnError=LCD_WriteNumAsynch(LCD1,seconds,NULL);
 
time++;
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
