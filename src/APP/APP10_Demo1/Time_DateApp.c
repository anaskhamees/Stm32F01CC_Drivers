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

#define ON                           1
#define OFF                          0

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

uint8_t Day=9;
uint8_t Month=4;
uint16_t Year=24;

uint8_t DayLength=1;
uint8_t MonthLength=1;

uint8_t DateTimePosCounter=0;

// uint8_t Hour=6;
// uint8_t Minutes=27;
// uint8_t Seconds=40;

uint8_t CurrentDisplay=StopWatch;

uint8_t EditMode=OFF;

uint8_t PosX=FirstLine;
uint8_t PosY=0;

uint8_t ModeState=CONSTANT;

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
        PosY=0;
    }
    else if(PosY==7&&PosX==SecondLine)
    {
        PosX=FirstLine;
        PosY=0;
    }
    else
    {
        PosY++;
    }
    LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
}

static void LCD_ShiftLeft(void)
{
    if(PosY==0&&PosX==FirstLine)
    {
        PosX=SecondLine;
        PosY=7;
    }
    else if(PosY==0&&PosX==SecondLine)
    {
        PosX=FirstLine;
        PosY=7;
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
    LCD_WriteStringAsynch(LCD1,"Date and Time",13,NULL);
    /*Display "Stopwatch" */
    LCD_SetCursorPosAsynch(LCD1,SecondLine,0,NULL);
    LCD_WriteStringAsynch(LCD1,"Stopwatch",9,NULL);
    /*Return Cursor to Beginning*/
    LCD_SetCursorPosAsynch(LCD1,FirstLine,0,NULL);
}


 void LCD_DisplayDateTime(void)
 {
    static uint8_t First_Time=0;
    ErrorStatus_t ReturnError;
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
        }
 }
 
void LCD_DisplayStopwatch()
{
   
    static uint8_t First_Time=0;
    ErrorStatus_t ReturnError;

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
        

        //if((MM_CursorLoc==SecondLine)&&(buffer==OK))
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

    /**************************************/
    /*      ___________________________   */
    /*     |                           |  */
    /*     |       08:27:40            |  */
    /*     |       HH:MM:SS            |  */
    /*     |___________________________|  */
    /*                                    */
    /**************************************/

    /*Cursor H1*/
    if(PosX==SecondLine&&PosY==0)
    {
        // if((Hour<10)&&(Hour==9))
        // {
        //     Hour=0;
        // }
        // else
        // {
        //     Hour++;
        // }
    }
    /*Cursor H2*/
    if(PosX==SecondLine&&PosY==1)
    {

    }
    /*Cursor M1*/
    if(PosX==SecondLine&&PosY==3)
    {
        
    }
    /*Cursor M2*/
    if(PosX==SecondLine&&PosY==4)
    {
        
    }
    /*Cursor S1*/
    if(PosX==SecondLine&&PosY==6)
    {
        
    }
    /*Cursor S2*/
    if(PosX==SecondLine&&PosY==7)
    {
        
    }
}

static void LCD_DecrementDateTime(void)
{
    /*-------------------------- Date ------------------------*/

    /*-------------------------- Time ------------------------*/
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

                }
                break;
                case RESET_STOPWATCH:
                {

                }
                break;
                case PAUSE_CONTINUE_STOPWATCH:
                {
                    
                }
                break;
                case EDIT:
                {

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
            LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
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
                        LCD_SetCursorPosAsynch(LCD1,FirstLine,0,NULL);  
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