#include "APP/APP.h"
#ifdef APP
#if (APP==APP10_DEMO1)
/*
 * File Name: Time_DateApp.c
 * Layer	: App
 * Version  : V 0.0
 * Created	:  Apr 8, 2024
 * Author1 	: Yasmin Amr
 * Author2  : Anas Khamees
 */

/**********************************************************************************/
/*										Includes				     			  */
/**********************************************************************************/
#include "MCAL/UART/USART.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/NVIC/NVIC.h"
#include "MCAL/NVIC/STM32F401xx.h"
#include "MCAL/UART/USART.h"
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LCD_Scheduler/LCD.h"
#include "Scheduler/Scheduler.h"

/***********************************************************************************/
/*										Defines									   */
/***********************************************************************************/
#define OK                          '3'
#define START_STOP_STOPWATCH        '5'
#define RESET_STOPWATCH             '6'
#define PAUSE_CONTINUE_STOPWATCH    '7'
#define UP                          '2'
#define DOWN                        '7'
#define RIGHT                       '8'
#define LEFT                        '6'
#define EDIT                        '4'
#define MODE                        '1'

#define MainMenu    0
#define StopWatch   1
#define DateTime    2

#define FirstLine   0
#define SecondLine  1

#define ON          1
#define OFF         0

#define CONSTANT                    0
#define MODIFIED                    1

/***********************************************************************************/
/*										Types									   */
/***********************************************************************************/



/************************************************************************************/
/*									Variables									    */
/************************************************************************************/
uint8_t buffer=0;
uint8_t CurrentMode= MainMenu;
uint8_t MM_CursorLoc=FirstLine;

uint8_t Day=10;
uint8_t Month=4;
uint16_t Year=2024;

uint8_t DayLength=1;
uint8_t MonthLength=1;

uint8_t DateTimePosCounter=0;

// uint8_t Hour=6;
// uint8_t Minutes=27;
// uint8_t Seconds=40;

uint8_t CurrentDisplay=StopWatch;

uint8_t EditMode=OFF;

uint8_t PosX=FirstLine;
uint8_t PosY=DATE_TIME_INDEX;

uint8_t ModeState=CONSTANT;

uint8_t EditUpdate=CONSTANT;

/************************************************************************************/
/*                              Time Variables                                      */
/************************************************************************************/
uint32_t TimeSeconds=30;
uint32_t TimeMinutes=59;
uint32_t TimeHours=0;

/************************************************************************************/
/*                              Stop Watch Variables                                */
/************************************************************************************/
uint32_t StopWatchTensSeconds=30;
uint32_t StopWatchSeconds=30;
uint32_t StopWatchMinutes=59;
uint32_t StopWatchHours=0;
/*************************************************************************************/
/*							Static Functions Prototype								 */
/*************************************************************************************/
 static void LCD_ShiftRight(void);
 static void LCD_ShiftLeft(void);
 static void LCD_DisplayMainMenu(void);
 static void LCD_IncrementDateTime(void);
 static void LCD_DecrementDateTime(void);
 static void USART_ReceiveCbf(void);

/************************************************************************************/
/*                            APIs Implementation								    */
/************************************************************************************/
static void LCD_ShiftRight(void)
{
    if(PosY==7&&PosX==FirstLine)
    {
        PosX=SecondLine;
        PosY=HOURS_INDEX1;
    }
    else if(PosY==YEAR_INDEX2&&PosX==SecondLine)
    {
        PosX=FirstLine;
        PosY=DAY_INDEX1;
    }
    else
    {
        PosY++;
    }
    LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
}

static void LCD_ShiftLeft(void)
{
    if(PosY==HOURS_INDEX1&&PosX==FirstLine)
    {
        PosX=SecondLine;
        PosY=YEAR_INDEX2;
    }
    else if(PosY==6&&PosX==SecondLine)
    {
        PosX=FirstLine;
        PosY=SECONDS_INDEX2;
    }
    else
    {
        PosY--;
    }
    LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
}
static void LCD_DisplayMainMenu(void)
{
    /*Display "Date and Time" */
    LCD_SetCursorPosAsynch(LCD1,FirstLine,0,NULL);
    LCD_WriteStringAsynch(LCD1,"1.Date and Time",15,NULL);
    /*Display "Stopwatch" */
    LCD_SetCursorPosAsynch(LCD1,SecondLine,0,NULL);
    LCD_WriteStringAsynch(LCD1,"2.Stopwatch",11,NULL);
    /*Return Cursor to Beginning*/
    LCD_SetCursorPosAsynch(LCD1,FirstLine,0,NULL);
}

 void LCD_DisplayDateTime(void)
 {
    static uint8_t First_Time=0;
    ErrorStatus_t ReturnError;

        if(EditMode==OFF)
        {
            TimeSeconds++;
            if (TimeSeconds > 59) 
            {
                TimeSeconds = 0;

                TimeMinutes++;
                if (TimeMinutes > 59) 
                {
                    TimeMinutes = 0;
                    TimeHours++;
                    if (TimeHours > 23) 
                    {
                        TimeHours = 0;
                    }
                }
            }
       


        //if((MM_CursorLoc==FirstLine)&&(buffer==OK))
        if(CurrentMode==DateTime)
        {
            /*First Function Entry*/
            if(First_Time==0)
            {
                LCD_ClearScreenAsynch(LCD1,NULL);
                First_Time++;
            }
            /*Mode Change: Stopwatch/DateTime*/
            else if(ModeState==MODIFIED)
            {
                LCD_ClearScreenAsynch(LCD1,NULL);
                ModeState=CONSTANT; 
            }
            else
            {
                //Do Nothing
            }

            /*Don't Update in Edit Mode*/
            if((EditMode==OFF)||(EditUpdate==MODIFIED))
            {   

                ReturnError=LCD_SetCursorPosAsynch(LCD1,FirstLine,0,NULL);
                ReturnError=LCD_WriteStringAsynch(LCD1,"Time: ",6,NULL);
                //ReturnError=LCD_SetCursorPosAsynch(LCD1,1,2,NULL);

                ReturnError=LCD_WriteNumAsynch(LCD1,TimeHours/10,NULL);
                ReturnError=LCD_WriteNumAsynch(LCD1,TimeHours%10,NULL); 
        
                ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);
                
                ReturnError=LCD_WriteNumAsynch(LCD1,TimeMinutes/10,NULL);
                ReturnError=LCD_WriteNumAsynch(LCD1,TimeMinutes%10,NULL);

                ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);
                
                ReturnError=LCD_WriteNumAsynch(LCD1,TimeSeconds/10,NULL);
                ReturnError=LCD_WriteNumAsynch(LCD1,TimeSeconds%10,NULL);

                ReturnError=LCD_SetCursorPosAsynch(LCD1,SecondLine,0,NULL);
                ReturnError=LCD_WriteStringAsynch(LCD1,"Date: ",6,NULL);

                ReturnError=LCD_WriteNumAsynch(LCD1,Day/10,NULL);
                ReturnError=LCD_WriteNumAsynch(LCD1,Day%10,NULL); 
        
                ReturnError=LCD_WriteStringAsynch(LCD1,"/",1,NULL);
                
                ReturnError=LCD_WriteNumAsynch(LCD1,Month/10,NULL);
                ReturnError=LCD_WriteNumAsynch(LCD1,Month%10,NULL);

                ReturnError=LCD_WriteStringAsynch(LCD1,"/",1,NULL);
                
                ReturnError=LCD_WriteNumAsynch(LCD1,Year/10,NULL);
                ReturnError=LCD_WriteNumAsynch(LCD1,Year%10,NULL);

                if(EditUpdate==MODIFIED)
                {
                    EditUpdate=CONSTANT;
                    /*Set Cursor Back to Sme Position (No Incrementing)*/
                    LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
                }
            }
        }
 }
 
void LCD_DisplayStopwatch()
{
      ErrorStatus_t ReturnError;
    static uint8_t First_Time=0;
    if(((StopWatchPauseContinue==STOP_WATCH_CONTINUE)&&(StopWatchStartStop==STOP_WATCH_START))&&(StopWatchReset==STOP_WATCH_RESET_OFF))
    {
        StopWatchTensSeconds++;

       if(StopWatchTensSeconds>9) 
       {
            StopWatchTensSeconds=0;
            StopWatchSeconds++;
            if (StopWatchSeconds > 59) 
            {
                StopWatchSeconds = 0;
                StopWatchMinutes++;
                if (StopWatchMinutes > 59) 
                {
                    StopWatchMinutes = 0;
                    StopWatchHours++;
                    if (StopWatchHours > 23) 
                    {
                        StopWatchHours = 0;
                    }
                }
            }
       }
    }
    else if (StopWatchReset==STOP_WATCH_RESET_ON)
    {
        StopWatchTensSeconds=0;
        StopWatchSeconds=0;
        StopWatchMinutes=0;
        StopWatchHours=0;
        StopWatchReset=STOP_WATCH_RESET_OFF;
    }
        
        if(CurrentMode==StopWatch)
        {
            if(First_Time==0)
            {
                LCD_ClearScreenAsynch(LCD1,NULL);
                First_Time++;
            } 
            else if(ModeState==MODIFIED)
            {
                LCD_ClearScreenAsynch(LCD1,NULL);
                ModeState=CONSTANT;             
            }     

            ReturnError=LCD_SetCursorPosAsynch(LCD1,FirstLine,3,NULL);
            ReturnError=LCD_WriteStringAsynch(LCD1,"Stop Watch",10,NULL);
            ReturnError=LCD_SetCursorPosAsynch(LCD1,1,2,NULL);

            ReturnError=LCD_WriteNumAsynch(LCD1,StopWatchHours/10,NULL);
            ReturnError=LCD_WriteNumAsynch(LCD1,StopWatchHours%10,NULL); 
    
            ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);
            
            ReturnError=LCD_WriteNumAsynch(LCD1,StopWatchMinutes/10,NULL);
            ReturnError=LCD_WriteNumAsynch(LCD1,StopWatchMinutes%10,NULL);

            ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);
            
            ReturnError=LCD_WriteNumAsynch(LCD1,StopWatchSeconds/10,NULL);
            ReturnError=LCD_WriteNumAsynch(LCD1,StopWatchSeconds%10,NULL);

            ReturnError=LCD_WriteStringAsynch(LCD1,":",1,NULL);

            ReturnError=LCD_WriteNumAsynch(LCD1,StopWatchTensSeconds/10,NULL);
            ReturnError=LCD_WriteNumAsynch(LCD1,StopWatchTensSeconds%10,NULL);
        }
}
static void LCD_IncrementDateTime(void)
{
   
    /*-------------------------- Date ------------------------*/

    /*-------------------------- Time ------------------------*/
    uint8_t TimeHoursIndex1= (TimeHours/10);
    uint8_t TimeHoursIndex2= (TimeHours%10);
    uint8_t TimeMinutesIndex1= (TimeMinutes/10);
    uint8_t TimeMinutesIndex2= (TimeMinutes%10);  
    uint8_t TimeSecondsIndex1= (TimeSeconds/10);
    uint8_t TimeSecondsIndex2= (TimeSeconds%10);  

    /*Cursor Hour Index1*/
    if(PosX==FirstLine&&PosY==HOURS_INDEX1)
    {
        /*Condition to Not Exceed Hours=23*/
        if((TimeHoursIndex1==1)&&(TimeHoursIndex2>3))
        {
            //Do Nothing
        }
        else if(TimeHoursIndex1==2)
        {
            TimeHoursIndex1=0;
        }
        else
        {
            TimeHoursIndex1++;
        }
    }

    /*Cursor Hour Index2*/
    if(PosX==FirstLine&&PosY==HOURS_INDEX2)
    {
        if(TimeHoursIndex1==2&&TimeHoursIndex2==3)
        {
            TimeHoursIndex2=0;
        }
        else
        {
            if(TimeHoursIndex2==9)
            {
                TimeHoursIndex2=0;
            }
            else
            {
                TimeHoursIndex2++;
            }
        }

    }
    /*Cursor Minute Index1*/
    if(PosX==FirstLine&&PosY==MINUTES_INDEX1)
    {
        if(TimeMinutesIndex1==5)
        {
            TimeMinutesIndex1=0;
        }
        else
        {
            TimeMinutesIndex1++;
        }
    }
    /*Cursor Minute Index2*/
    if(PosX==FirstLine&&PosY==MINUTES_INDEX2)
    {
        if(TimeMinutesIndex2==9)
        {
            TimeMinutesIndex2=0;
        }
        else
        {
            TimeMinutesIndex2++;
        }
    }

    /*Cursor Seconds Index1*/
    if(PosX==FirstLine&&PosY==SECONDS_INDEX1)
    {
        if(TimeSecondsIndex1==5)
        {
            TimeSecondsIndex1=0;
        }
        else
        {
            TimeSecondsIndex1++;
        }
    }
    /*Cursor Seconds Index2*/
    if(PosX==FirstLine&&PosY==SECONDS_INDEX2)
    {
        if(TimeSecondsIndex2==9)
        {
            TimeSecondsIndex2=0;
        }
        else
        {
            TimeSecondsIndex2++;
        }
    }

    TimeHours= (TimeHoursIndex1*10)+TimeHoursIndex2;
    TimeMinutes= (TimeMinutesIndex1*10)+TimeMinutesIndex2;
    TimeSeconds= (TimeSecondsIndex1*10)+TimeSecondsIndex2;
    EditUpdate=MODIFIED;
}

static void LCD_DecrementDateTime(void)
{
    /*-------------------------- Date ------------------------*/

    /*-------------------------- Time ------------------------*/
    uint8_t TimeHoursIndex1= (TimeHours/10);
    uint8_t TimeHoursIndex2= (TimeHours%10);
    uint8_t TimeMinutesIndex1= (TimeMinutes/10);
    uint8_t TimeMinutesIndex2= (TimeMinutes%10);  
    uint8_t TimeSecondsIndex1= (TimeSeconds/10);
    uint8_t TimeSecondsIndex2= (TimeSeconds%10);  
    /*Cursor Hour Index1*/
    if(PosX==FirstLine&&PosY==HOURS_INDEX1)
    {
        /*Condition to Not Exceed Hours=23*/
        if((TimeHoursIndex1==0)&&(TimeHoursIndex2>3))
        {
            //Do Nothing
        }
        else if(TimeHoursIndex1==0)
        {
            TimeHoursIndex1=2;
        }
        else
        {
            TimeHoursIndex1--;
        }
        
    }
    /*Cursor Hour Index2*/
    if(PosX==FirstLine&&PosY==HOURS_INDEX2)
    {
        if(TimeHoursIndex1==2&&TimeHoursIndex2==0)
        {
            TimeHoursIndex2=3;
        }
        else
        {
            if(TimeHoursIndex2==0)
            {
                TimeHoursIndex2=9;
            }
            else
            {
                TimeHoursIndex2--;
            }
        }

    }
    /*Cursor Minute Index1*/
    if(PosX==FirstLine&&PosY==MINUTES_INDEX1)
    {
        if(TimeMinutesIndex1==0)
        {
            TimeMinutesIndex1=5;
        }
        else
        {
            TimeMinutesIndex1--;
        }
    }
    /*Cursor Minute Index2*/
    if(PosX==FirstLine&&PosY==MINUTES_INDEX2)
    {
        if(TimeMinutesIndex2==0)
        {
            TimeMinutesIndex2=9;
        }
        else
        {
            TimeMinutesIndex2--;
        }
    }

    /*Cursor Seconds Index1*/
    if(PosX==FirstLine&&PosY==SECONDS_INDEX1)
    {
        if(TimeSecondsIndex1==0)
        {
            TimeSecondsIndex1=5;
        }
        else
        {
            TimeSecondsIndex1--;
        }
    }
    /*Cursor Seconds Index2*/
    if(PosX==FirstLine&&PosY==SECONDS_INDEX2)
    {
        if(TimeSecondsIndex2==0)
        {
            TimeSecondsIndex2=9;
        }
        else
        {
            TimeSecondsIndex2--;
        }
    }

    TimeHours= (TimeHoursIndex1*10)+TimeHoursIndex2;
    TimeMinutes= (TimeMinutesIndex1*10)+TimeMinutesIndex2;
    TimeSeconds= (TimeSecondsIndex1*10)+TimeSecondsIndex2;
    EditUpdate=MODIFIED;
}
 

static void USART_ReceiveCbf(void)
{
    switch(CurrentMode)
    {
        case MainMenu:
        {
            switch(buffer)
            {
                case UP:
                {
                   LCD_SetCursorPosAsynch(LCD1,FirstLine,0,NULL);  
                   MM_CursorLoc=FirstLine;
                }
                break;
                case DOWN:
                {
                   LCD_SetCursorPosAsynch(LCD1,SecondLine,0,NULL);
                   MM_CursorLoc=SecondLine;
                }
                break;
                case OK:
                {
                    LCD_DisableCursorAsynch(LCD1,NULL);
                    if(MM_CursorLoc==FirstLine)
                    {
                        CurrentMode=DateTime;
                    }
                    else if(MM_CursorLoc==SecondLine)
                    {
                        CurrentMode=StopWatch;
                        //buffer=OK;
                    }
                }
                break;
                default:
                {
                    //Do Nothing
                }
                break;
            }
        }
        break;
        case StopWatch:
        {
             switch(buffer)
            {
                case START_STOP_STOPWATCH:
                {
                    if(StopWatchStartStop==STOP_WATCH_STOP)
                    {
                        StopWatchStartStop=STOP_WATCH_START;
                    }
                    else if(StopWatchStartStop==STOP_WATCH_START)
                    {
                        StopWatchStartStop=STOP_WATCH_STOP;
                    }         
                }
                break;
                case RESET_STOPWATCH:
                {
                    StopWatchReset=STOP_WATCH_RESET_ON;
                }
                break;
                case PAUSE_CONTINUE_STOPWATCH:
                {
                    if(StopWatchPauseContinue==STOP_WATCH_CONTINUE)
                    {
                        StopWatchPauseContinue=STOP_WATCH_PAUSE;
                    }
                    else if(StopWatchPauseContinue==STOP_WATCH_PAUSE)
                    {
                        StopWatchPauseContinue=STOP_WATCH_CONTINUE;
                    }
                }
                break;
                case EDIT:
                {
                    /* No Edits in StopWatch */
                }
                break;
                case MODE:
                {
                    CurrentMode=DateTime;
                    ModeState=MODIFIED;
                }
                break;
            }       
        }
        break;
        case DateTime:
        {
            //LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
            switch(buffer)
            {
                case UP:
                {
                    if(EditMode==ON)
                    {
                        LCD_IncrementDateTime();
                    }
                }
                break;
                case DOWN:
                {
                    if(EditMode==ON)
                    {
                        LCD_DecrementDateTime();
                    }
                }
                break;
                case RIGHT:
                {
                    LCD_ShiftRight();
                }
                break;
                case LEFT:
                {
                    LCD_ShiftLeft();
                }
                break;
                case EDIT:
                {
                    if(EditMode==OFF)
                    {
                        EditMode=ON;
                        LCD_SetCursorPosAsynch(LCD1,FirstLine,DAY_INDEX1,NULL);  
                        LCD_EnableCursorAsynch(LCD1,NULL);
                    }
                    else if(EditMode==ON)
                    {
                        EditMode=OFF;
                        LCD_DisableCursorAsynch(LCD1,NULL);
                    }                   
                }
                break;
                case MODE:
                {
                    CurrentMode=StopWatch;
                    ModeState=MODIFIED;
                    /*Disable Editing Mode*/
                    LCD_DisableCursorAsynch(LCD1,NULL);
                    EditMode=OFF;
                }
                break;

            }
        }
        break;
    }
   // buffer=0;
}

/* Each 125mSec */
void Display_App(void)
{
    USART_ReceiveBufferAsynchZeroCopy(USART2,&buffer,1,USART_ReceiveCbf);
}

void APP_Init(void)
{
    LCD_DisplayMainMenu();
}
#endif
#endif