#include "APP/APP.h"
#ifdef APP
#if (APP==APP10_DEMO1)
/*
 * File Name: Time_DateApp.c
 * Layer	: App
 * Version  : V 0.0
 * Created	: Apr 8, 2024
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

/*============================ Control Switches ============================*/
/* Anas change the switchs order and values to optimize Switchs number*/

#define OK                          '1' 
#define MODE                        '1'

#define UP                          '2'
#define EDIT                        '3'

#define RIGHT                       '4'
#define START_STOP_STOPWATCH        '4' 

#define LEFT                        '5'
#define RESET_STOPWATCH             '5'

#define PAUSE_CONTINUE_STOPWATCH    '6'
#define DOWN                        '6'

/*==================== LCD Screen Current Mode ============================*/
#define MainMenu                    0
#define StopWatch                   1
#define DateTime                    2
/*==================  Defines Represent LCD Lines (Rows) ==================*/
#define FirstLine                   0
#define SecondLine                  1
/*================= Defines Represent EditMode is ON/OFF ==================*/
#define ON                          1
#define OFF                         0
/*=========================================================================*/
#define CONSTANT                    0
#define MODIFIED                    1

#define DATE_TIME_INDEX 6
/*======================== Time Digits on LCD ===========================*/
#define HOURS_INDEX1    6    
#define HOURS_INDEX2    7
#define MINUTES_INDEX1  9
#define MINUTES_INDEX2  10
#define SECONDS_INDEX1  12
#define SECONDS_INDEX2  13
/*======================== Date Digits on LCD ===========================*/
#define DAY_INDEX1      6
#define DAY_INDEX2      7
#define MONTH_INDEX1    9
#define MONTH_INDEX2    10
#define YEAR_INDEX1     12
#define YEAR_INDEX2     13
#define YEAR_INDEX3     14
#define YEAR_INDEX4     15
/*========================= StopWatch Modes =============================*/
#define STOP_WATCH_PAUSE            0
#define STOP_WATCH_CONTINUE         1
#define STOP_WATCH_RESET_ON         2
#define STOP_WATCH_RESET_OFF        3
#define STOP_WATCH_START            4
#define STOP_WATCH_STOP             5
/************************************************************************************/
/*									Variables									    */
/************************************************************************************/

uint8_t buffer=0;                   /* Var to Save Received Data From UART                           */
uint8_t CurrentMode= MainMenu;      /* Var represent the Current mode to Display on LCD              */
uint8_t MM_CursorLoc=FirstLine;     /* Var to store the Current location of Cursor in Main Menu Mode */
/*======================= Variables of Date =========================*/
uint8_t Day=10;
uint8_t Month=4;
uint16_t Year=2024;
/*============================????===================================*/
uint8_t DayLength=1;
uint8_t MonthLength=1;
uint8_t DateTimePosCounter=0;
/*===================== LCD Operation States =======================*/
uint8_t CurrentDisplay=StopWatch;
uint8_t EditMode=OFF;

uint8_t PosX=FirstLine;
uint8_t PosY=DATE_TIME_INDEX-1; /* Anas Edit This, should be (DATE_TIME_INDEX-1)
                               * because (PosY++;) execute first then 
                               * LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
                               */

uint8_t ModeState=CONSTANT;
uint8_t EditUpdate=CONSTANT;
/*============================ Time Variables ==============================*/
uint32_t TimeSeconds=30;
uint32_t TimeMinutes=59;
uint32_t TimeHours=0;
/*========================= StopWatch Variables ============================*/
uint8_t StopWatchTensSeconds=0;
uint8_t StopWatchSeconds=0;
uint8_t StopWatchMinutes=0;
uint8_t StopWatchHours=0;

uint8_t StopWatchStartStop=STOP_WATCH_STOP;
uint8_t StopWatchPauseContinue=STOP_WATCH_CONTINUE;
uint8_t StopWatchReset=STOP_WATCH_RESET_OFF;

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
    /* If the Cursor now,In the most Right of First Line and ShiftRight is Triggered, 
     * The cursor will move to the most Right in the Second Line.
     * For Ease and flexibility to move between the FirstLine and SecondLine in EditMode (Time and Date).
     */
     if(PosY==SECONDS_INDEX2&&PosX==FirstLine)
    {
        PosX=SecondLine;
        PosY=YEAR_INDEX4; /* Anas Edit this */
    }
    /* If the Cursor now,In the most Right of SecondLine and ShiftRight is Triggered, 
     * The cursor will move to the most Right in the First Line.
     * For Ease and flexibility to move between the FirstLine and SecondLine in EditMode (Time and Date).
     */
    else if(PosY==YEAR_INDEX4&&PosX==SecondLine)
    {
        PosX=FirstLine;
        PosY=SECONDS_INDEX2;  /* Anas Edit this */
    }
    else
    {
        PosY++;
    }
    LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
}

static void LCD_ShiftLeft(void)
{
    /* If the Cursor now,In the most Left of First Line and ShiftLeft is Triggered, 
     * The cursor will move to the most Left in the Second Line.
     * For Ease and flexibility to move between the FirstLine and SecondLine in EditMode (Time and Date).
     */
    if(PosY==HOURS_INDEX1&&PosX==FirstLine)
    {
        PosX=SecondLine;
        PosY=DAY_INDEX1; /* Anas Edit This */
    }
    /* If the Cursor now,In the most Left of Second Line and ShiftLeft is Triggered, 
     * The cursor will move to the most Left in the First Line.
     * For Ease and flexibility to move between the FirstLine and SecondLine in EditMode (Time and Date).
     */
    else if(PosY==DAY_INDEX1&&PosX==SecondLine)
    {
        PosX=FirstLine;
        PosY=HOURS_INDEX1; /* Anas Edit This */
    }
    else
    {
        PosY--;
    }
    LCD_SetCursorPosAsynch(LCD1,PosX,PosY,NULL);
}

static void LCD_DisplayMainMenu(void)
{
    /* Display "Date and Time"    */
    LCD_SetCursorPosAsynch(LCD1,FirstLine,0,NULL);
    LCD_WriteStringAsynch(LCD1,"1.Date and Time",15,NULL);
    /* Display "Stopwatch"        */
    LCD_SetCursorPosAsynch(LCD1,SecondLine,0,NULL);
    LCD_WriteStringAsynch(LCD1,"2.Stopwatch",11,NULL);
    /* Return Cursor to Beginning */
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
        }

        if(CurrentMode==DateTime)
        {
            /* First Function Entry            */
            if(First_Time==0)
            {
                LCD_ClearScreenAsynch(LCD1,NULL);
                First_Time++;
            }
            /* Mode Change: Stopwatch/DateTime */
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
            if((EditMode==OFF)||(EditUpdate==MODIFIED)) /* Is (EditUpdate==MODIFIED) for if In Edit mode and Time/Date Updated ?*/
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
                
                uint8_t YearThousandsDigit=Year/1000      ;  /* Thousands place */
                uint8_t YearHundredsDigit =(Year%1000)/100;  /* Hundreds place  */
                uint8_t YearTensDigit     =(Year%100)/10  ;  /* Tens place      */
                uint8_t YearOnesDigit     =Year%10        ;  /* Ones place      */
           
                ReturnError=LCD_WriteNumAsynch(LCD1,YearThousandsDigit,NULL); 
                ReturnError=LCD_WriteNumAsynch(LCD1,YearHundredsDigit,NULL);
                ReturnError=LCD_WriteNumAsynch(LCD1,YearTensDigit,NULL);
                ReturnError=LCD_WriteNumAsynch(LCD1,YearOnesDigit,NULL);
                

                if(EditUpdate==MODIFIED) /* Edit mode and Modification is Done*/
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
    
    if(((StopWatchPauseContinue==STOP_WATCH_CONTINUE)&&
        (StopWatchStartStop==STOP_WATCH_START))&&(StopWatchReset==STOP_WATCH_RESET_OFF))
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
    /**************************************/
    /*      ___________________________   */
    /*     |                           |  */
    /*     |       08:27:40            |  */
    /*     |       HH:MM:SS            |  */
    /*     |___________________________|  */
    /*                                    */
    /**************************************/

/*------------------------------------------------------------*/
/*---------------------- Edit in Date ------------------------*/
/*------------------------------------------------------------*/
    
    uint8_t DayIndex1  =(Day/10)       ;
    uint8_t DayIndex2  =(Day%10)       ;
    uint8_t MonthIndex1=(Month/10)     ;
    uint8_t MonthIndex2=(Month%10)     ;
    uint8_t YearIndex1 =Year/1000      ;  /* Thousands place */
    uint8_t YearIndex2 =(Year%1000)/100;  /* Hundreds place  */
    uint8_t YearIndex3 =(Year%100)/10  ;  /* Tens place      */
    uint8_t YearIndex4 =Year%10        ;  /* Ones place      */
    
    /*-------------------- Edit DayIndex1 -------------------*/
    if((PosX== SecondLine)&&(PosY==DAY_INDEX1))
    {
        /* Ex: If Day is 30 or 31 and digit (3) triggered to Increment to be 4,5..(Not Allowed: Meaningless) */
        if(DayIndex1==3)
        {
           /* CornerCase: If I Edit Index2 First before editing Index1 (ex:DayIndex1=3,,DayIndex2=0) 
            * and DayIndex1 triggered to increment.. DayIndex1 will be (1).
            * beacause There is no day called 00,,, (30 ---> 10)
            */
            if(DayIndex2==0)
            {
                DayIndex1=1;
            }
            /* Ex: If Month is 31 ----will be----> 01 */
            else
            {
                DayIndex1=0;
            }
        }
         
        else  
        {
            DayIndex1++;
            /* CornerCase: If User Edit DayIndex2 Before DayIndex1 
             * Ex: Day = 31, and user increment DayIndex2 (1) to be 2,3,4....
             * DayIndex1 Must be Cleared (0) because Day: 32, 33,34 Not Allowed.
             * But Day will be ------------------->  Day: 02, 03,04 .
             */
            if((DayIndex1==3)&&(DayIndex2>1))
            {
                DayIndex2=0;
            }
        }
    }
    /*-------------------- Edit DayIndex2 -------------------*/
    if((PosX== SecondLine)&&(PosY==DAY_INDEX2))
    {
        /**
         * If the Day is 31, and DayIndex2 triggered to increment (32,33), Meaningless.
         */
        if((DayIndex1==3)&&(DayIndex2==1)) 
        {
            DayIndex2=0;
        }
        if(DayIndex2==9)
        {
           /* CornerCase: If I Edit Index1 First before editing Index2 (ex:DayIndex1=0,,DayIndex2=9) 
            * and DayIndex2 triggered to increment.. DayIndex2 will be (1) NOT (0).
            * beacause There is no day called 00,,, (09 ---> 01)
            */
            if(DayIndex1==0)
            {
            DayIndex2=1;
            }
            /* Ex: Day is 29 -----Will be------> 20 */
            else
            {
                DayIndex2=0;
            }
        }
        else 
        {
            DayIndex2++;
        }
    }
    /*---------------- Edit MonthIndex1 -----------------*/
    if((PosX==SecondLine)&&(PosY==MONTH_INDEX1))
    {
        if(MonthIndex1==1)
        {
           /* CornerCase: If I Edit Index2 First before editing Index1 (ex:MonthIndex1=1,,MonthIndex2=0) 
            * and DayIndex1 triggered to increment.. MonthIndex1 will remain (1) Not (9 or 0).
            * beacause There is no Month called 00 or 90 ,,, (10 ---> 10).
            */
            if(MonthIndex2==0)
            {
                MonthIndex1=1;
            }
            /* Ex: Month is 12 ----will be----> 02 */
            else
            {
                MonthIndex1=0;
            }
        }
        else if(MonthIndex2<=2)  /* Months 10 - 11 - 12  Only */
        {
            MonthIndex1++;
        }
    }
    /*------------------ Edit MonthIndex2 ------------------*/
    if((PosX==SecondLine)&&(PosY==MONTH_INDEX2))
    {
        if(MonthIndex2==9)
        {
           /* CornerCase: If I Edit Index1 First before editing Index2 (ex:MonthIndex1=0,,MonthIndex2=9) 
            * and MonthIndex2 triggered to increment.. MonthIndex2 will be (1) NOT (0).
            * beacause There is no Month called 00,,, (09 ---> 01)
            */
         MonthIndex2=1;

        }
        /* CornerCase:
         * Ex: Months: 10,11,12.. If MonthIndex2 Triggered to Increments to be 13,14...Meaningless.
         * So, MonthIndex2 must be cleared (0).
         */
        else if((MonthIndex2==2) &&(MonthIndex1==1))
        {
            MonthIndex2=0;            
        }
        else
        {
            MonthIndex2++;
        }
    }

    /*---------------------------------------------------------------*/
    /*------- Don't Edit in YearIndex1, YearIndex2  Meaningless -----*/
    /*---------------------------------------------------------------*/

    /*------------------- Edit YearIndex3 ---------------------*/
    if((PosX==SecondLine)&&(PosY==YEAR_INDEX3))
    {
        if(YearIndex3==9)
        {
            YearIndex3=0;
        }
        else
        {
            YearIndex3++;
        }
    }
    /*------------------- Edit YearIndex4 ---------------------*/
     if((PosX==SecondLine)&&(PosY==YEAR_INDEX4))
    {
        if(YearIndex4==9)
        {
            YearIndex4=0;
        }
        else
        {
            YearIndex4++;
        }
    }

/*======================= Date Updated Values =========================*/
    Day  =((DayIndex1*10)+DayIndex2);
    Month=((MonthIndex1*10)+MonthIndex2);
    Year =((YearIndex1*1000)+(YearIndex2*100)+(YearIndex3*10)+(YearIndex4));
/*=====================================================================*/
    
/*----------------------------------------------------------------*/
/*-------------------------- Edit in Time ------------------------*/
/*----------------------------------------------------------------*/
    uint8_t TimeHoursIndex1  = (TimeHours/10)  ;
    uint8_t TimeHoursIndex2  = (TimeHours%10)  ;
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
    uint8_t DayIndex1  =(Day/10)       ;
    uint8_t DayIndex2  =(Day%10)       ;
    uint8_t MonthIndex1=(Month/10)     ;
    uint8_t MonthIndex2=(Month%10)     ;
    uint8_t YearIndex1 =Year/1000      ;  /* Thousands place */
    uint8_t YearIndex2 =(Year%1000)/100;  /* Hundreds place  */
    uint8_t YearIndex3 =(Year%100)/10  ;  /* Tens place      */
    uint8_t YearIndex4 =Year%10        ;  /* Ones place      */
    
    /*-------------------- Edit DayIndex1 -------------------*/
    if((PosX== SecondLine)&&(PosY==DAY_INDEX1))
    {
        /**
         * IF Day is 01, and DayIndex1 (0) triggered to decrement,
         * The DayIndex1 will Be 3, So the day Will be 31. 
         */
        if((DayIndex1==0)&&(DayIndex2<2))
        {
            DayIndex1=3;
        }
        /* CornerCase: If I Edit Index2 First before editing Index1 (ex:DayIndex1=0 ,,, DayIndex2=5) 
         * and DayIndex1 triggered to decrement.. DayIndex1 will be (2) NOT (3).
         * beacause There is no day called 35,,, (05 ---> 25)
         */
        else if((DayIndex1==0)&&(DayIndex2>1))
        {
            DayIndex1=2;
        }
        /* CornerCase: If I Edit Index2 First (ex: DayIndex1=1 ,, DayIndex2=0) 
         * and DayIndex1 triggered to decrement.. DayIndex1 will be (3) NOT (9).
         * beacause There is no day called 90,,, (10 -----will be-----> 30).
         */
        else if((DayIndex1==1)&&(DayIndex2==0))
        {
            DayIndex1=3;            
        }
        else 
        {
            DayIndex1--;
        }
    }
    /*-------------------- Edit DayIndex2 -------------------*/
    if((PosX== SecondLine)&&(PosY==DAY_INDEX2))
    {
        /* Ex: If Day is 10 or 20, and DayIndex2(0) triggered to Decrement, 
         * DayIndex2 will be 9 ,Day 10 ---> Day 19 and Day 20 -----> 29.
         */
        if((DayIndex2==0)&&(DayIndex1<3))
        {
            DayIndex2=9;
        }
        /* Ex: If Day is 30 , and DayIndex2(0) triggered to Decrement, 
         * DayIndex2 will be 1 ,Day 30 ---> 31.
         */
        else if((DayIndex2==0)&&(DayIndex1==3))
        {
            DayIndex2=1;
        }
        /* CornerCase: If I Edit Index1 First (ex:DayIndex1=0,,DayIndex2=1) 
         * and DayIndex2 triggered to decrement.. DayIndex2 will be (9).
         * beacause There is no day called 00,,, (01 ---> 09)
         */
        else if((DayIndex1==0)&&(DayIndex2==1))
        {
            DayIndex2=9;
        }
        else
        {
            DayIndex2--;
        }
    }
    /*---------------------- Edit MonthIndex1 -------------------*/
   
    if((PosX==SecondLine)&&(PosY==MONTH_INDEX1))
    {
        /* Ex: If Month :01 or 02, and MonthIndex1 triggered to decrement
         * MonthIndex1 will be 1.... 01 ---> 11 / 02 ---> 12.
         */
        if((MonthIndex1==0)&&(MonthIndex2<3))
        {
            MonthIndex1=1;
        }
        /* Ex: If Month :03,04,05..... and MonthIndex1 triggered to decrement
         * MonthIndex1 will be 0 .... 03 ---> 03 / 04 ---> 04 .
         * Because Month 13, 14, 15 ... Not Allowed.
         */
        else if((MonthIndex1==0)&&(MonthIndex2>=3))
        {
            MonthIndex1=0;
        }
        /* CornerCase: If I Edit Index2 First before editing Index1 (ex: MonthIndex1=1 ,, MonthIndex2=0) 
         * and MonthIndex1 triggered to decrement.. MonthIndex1 will be (1) NOT (0 or 9).
         * beacause There is no Month called 00 or 90,,, (10 ---> 10)
         */
        else if ((MonthIndex1==1)&&(MonthIndex2==0))
        {
            MonthIndex1=1;
        }
        else 
        {
            MonthIndex1--;
        }
    }
    /*-------------------- Edit MonthIndex2 ------------------*/
    if((PosX==SecondLine)&&(PosY==MONTH_INDEX2))
    {
        /* Ex: If Month is 01 and MonthIndex1 triggered to decrement
         * Month will be 00 (Not Allowed), if triggered to decrement again.
         * MonthIndex2 will be 9... 00 -----> 09
         */
        if((MonthIndex1==0)&&(MonthIndex2==0))
        {
            MonthIndex2=9;
        }
        /**
         * Ex : If Month is 10 and  MonthIndex2 triggered to decrement 
         * MonthIndex2 will be 2,,,, 10 -----> 12
         */
        else if((MonthIndex2==0)&&(MonthIndex1==1))
        {
            MonthIndex2=2; /*Month 12*/
        }
        /* CornerCase: If I Edit Index1 First before editing Index2 (ex:MonthIndex1=0,,MonthIndex2=1) 
         * and MonthIndex2 triggered to decrement.. MonthIndex2 will be (9) NOT (0).
         * beacause There is no Month called 00,,, (01 ---> 09)
         */
        else if((MonthIndex2==1)&&(MonthIndex1==0))
        {
            MonthIndex2=9;
        }
        else
        {
            MonthIndex2--;
        }
    }

    /*------- Don't Edit in YearIndex1, YearIndex2  Meaningless -----*/
    /*------------------- Edit YearIndex3 ---------------------*/
    if((PosX==SecondLine)&&(PosY==YEAR_INDEX3))
    {
        if(YearIndex3==0)
        {
            YearIndex3=9;
        }
        else
        {
            YearIndex3--;
        }
    }
    /*------------------- Edit YearIndex4 ---------------------*/
     if((PosX==SecondLine)&&(PosY==YEAR_INDEX4))
    {
        if(YearIndex4==0)
        {
            YearIndex4=9;
        }
        else
        {
            YearIndex4--;
        }
    }

/*============================ Date Updated Values ===================================*/
    Day  =((DayIndex1*10)+DayIndex2);
    Month=((MonthIndex1*10)+MonthIndex2);
    Year =((YearIndex1*1000)+(YearIndex2*100)+(YearIndex3*10)+(YearIndex4));
/*====================================================================================*/
    /*-------------------------- Time ------------------------*/
    uint8_t TimeHoursIndex1  = (TimeHours/10)  ;
    uint8_t TimeHoursIndex2  = (TimeHours%10)  ;
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
                    /*Disable Editing Mode*/
                    LCD_DisableCursorAsynch(LCD1,NULL);
                     EditMode=OFF;
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
    USART_ReceiveBufferAsynchZeroCopy(USART2,&buffer,1,USART_ReceiveCbf);
}

void APP_Init(void)
{
    LCD_DisplayMainMenu();
}
#endif
#endif