#include "APP/APP.h"
#ifdef APP
#if (APP==APP10_DEMO1)
/*
 * File Name: Time_DateApp.c
 * Layer	: App
 * Version  : V 0.0
 * Created	:  Apr 8, 2024
 * Author 	: Yasmin Amr
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

/***********************************************************************************/
/*										Types									   */
/***********************************************************************************/



/************************************************************************************/
/*									Variables									    */
/************************************************************************************/
uint8_t buffer=0;
uint8_t CurrentMode= MainMenu;
uint8_t MM_CursorLoc=FirstLine;

uint8_t Day=7;
uint8_t Month=4;
uint16_t Year=24;

uint8_t DayLength=1;
uint8_t MonthLength=1;

uint8_t DateTimePosCounter=0;

uint8_t Hour=8;
uint8_t Minutes=27;
uint8_t Seconds=40;

uint8_t CurrentDisplay=StopWatch;

uint8_t EditMode=OFF;

uint8_t PosX=FirstLine;
uint8_t PosY=0;

/*************************************************************************************/
/*							Static Functions Prototype								 */
/*************************************************************************************/
 static void LCD_ShiftRight(void);
 static void LCD_ShiftLeft(void);
 static void LCD_DisplayDateTime(void);
 static void LCD_DisplayStopWatch(void);
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

static void LCD_DisplayDateTime(void)
{
    if(CurrentDisplay!=DateTime)
    {
        /*Update Current Display*/
        CurrentDisplay=DateTime;

        LCD_ClearScreenAsynch(LCD1,NULL);

        /*Display Date*/
        LCD_SetCursorPosAsynch(LCD1,FirstLine,0,NULL);
        LCD_WriteNumAsynch(LCD1,Day,NULL);

        LCD_SetCursorPosAsynch(LCD1,FirstLine,2,NULL);
        LCD_WriteStringAsynch(LCD1,"/",1,NULL); 

        LCD_SetCursorPosAsynch(LCD1,FirstLine,3,NULL);
        LCD_WriteNumAsynch(LCD1,Month,NULL);

        LCD_SetCursorPosAsynch(LCD1,FirstLine,5,NULL);
        LCD_WriteStringAsynch(LCD1,"/",1,NULL);

        LCD_SetCursorPosAsynch(LCD1,FirstLine,6,NULL);
        LCD_WriteNumAsynch(LCD1,Year,NULL);

        /*Display Time*/
        LCD_SetCursorPosAsynch(LCD1,SecondLine,0,NULL);
        LCD_WriteNumAsynch(LCD1,Hour,NULL);

        LCD_SetCursorPosAsynch(LCD1,SecondLine,2,NULL);
        LCD_WriteStringAsynch(LCD1,":",1,NULL);

        LCD_SetCursorPosAsynch(LCD1,SecondLine,3,NULL);
        LCD_WriteNumAsynch(LCD1,Minutes,NULL);

        LCD_SetCursorPosAsynch(LCD1,SecondLine,5,NULL);
        LCD_WriteStringAsynch(LCD1,":",1,NULL);

        LCD_SetCursorPosAsynch(LCD1,SecondLine,6,NULL);
        LCD_WriteNumAsynch(LCD1,Seconds,NULL);

        /*Disable Cursor*/
        LCD_DisableCursorAsynch(LCD1,NULL);
        
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
                        LCD_DisplayDateTime();
                    }
                    else if(MM_CursorLoc==SecondLine)
                    {
                        CurrentMode=StopWatch;
                        //LCD_DisplayStopWatch();
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
                    
                }
                break;

            }
        }
        break;
    }
    buffer=0;
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