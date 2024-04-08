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

/*************************************************************************************/
/*							Static Functions Prototype								 */
/*************************************************************************************
 

/************************************************************************************/
/*                            APIs Implementation								    */
/************************************************************************************/
void LCD_DisplayMainMenu(void)
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


void USART_ReceiveCbf(void)
{
    switch(CurrentMode)
    {
        case MainMenu:
        {
            switch(buffer)
            {
                case UP:
                {
                   LCD_SetCursorPosAsynch(LCD1,0,FirstLine,NULL);  
                   MM_CursorLoc=FirstLine;
                }
                break;
                case DOWN:
                {
                   LCD_SetCursorPosAsynch(LCD1,0,SecondLine,NULL);
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
            switch(buffer)
            {
                case UP:
                {

                }
                break;
                case DOWN:
                {

                }
                break;
                case RIGHT:
                {
                    
                }
                break;
                case LEFT:
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
    }
}

/* Each 125mSec */
void Display_App(void)
{
    USART_ReceiveBufferAsynchZeroCopy(USART1,&buffer,1,USART_ReceiveCbf);
}

void APP_Init(void)
{
    LCD_DisplayMainMenu();
}
