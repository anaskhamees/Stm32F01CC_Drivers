#include "APP/APP.h"
#ifdef APP
#if (APP==APP11_DEMO1_STOPWATCH_TEST)
/*
 * File Name: StopWatchApp.h
 * Layer	: App
 * Version  : V 0.0
 * Created	:  Apr 8, 2024
 * Author 1	: Yasmin Amr
 * Author 2 : Anas Khamees
 */

/**********************************************************************************/
/*										Includes				     			  */
/**********************************************************************************/
#include "Std_Lib/Std_Libraries.h"
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LCD_Scheduler/LCD.h"
#include "Scheduler/Scheduler.h"
/***********************************************************************************/
/*										Defines									   */
/***********************************************************************************/



/***********************************************************************************/
/*										Types									   */
/***********************************************************************************/



/************************************************************************************/
/*									Variables									    */
/************************************************************************************/



/*************************************************************************************/
/*							Static Functions Prototype								 */
/*************************************************************************************/



/************************************************************************************/
/*                            APIs Implementation								    */
/************************************************************************************/

void LCD_DisplayStopWatch()
{
    static uint32_t Tens_Seconds=0;
    static uint32_t seconds=0;
    static uint32_t minutes=0;
    static uint32_t hours=0;
    ErrorStatus_t ReturnError;
    ReturnError=LCD_SetCursorPosAsynch(LCD1,0,3,NULL);
    ReturnError=LCD_WriteStringAsynch(LCD1,"Stop Watch",10,NULL);
    ReturnError=LCD_SetCursorPosAsynch(LCD1,1,2,NULL);
    Tens_Seconds++;

       if(Tens_Seconds>9) 
       {
            Tens_Seconds=0;
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
       }

        ReturnError=LCD_WriteNumAsynch(LCD1,hours/10,NULL);
        ReturnError=LCD_WriteNumAsynch(LCD1,hours%10,NULL); 
 
        ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);
        
        ReturnError=LCD_WriteNumAsynch(LCD1,minutes/10,NULL);
        ReturnError=LCD_WriteNumAsynch(LCD1,minutes%10,NULL);

        ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);
        
        ReturnError=LCD_WriteNumAsynch(LCD1,seconds/10,NULL);
        ReturnError=LCD_WriteNumAsynch(LCD1,seconds%10,NULL);

        ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);

        ReturnError=LCD_WriteNumAsynch(LCD1,Tens_Seconds/10,NULL);
        ReturnError=LCD_WriteNumAsynch(LCD1,Tens_Seconds%10,NULL);
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