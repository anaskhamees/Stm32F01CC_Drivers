/*
 * File Name: Scheduler.c
 * Layer	: Scheduler
 * Module	: Scheduler
 * Version  : V 0.0
 * Created	:  Mar 13, 2024
 * Author	: Anas Khamees
 */

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Scheduler.h"
#include "MCAL/SYSTICK/SysTick.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define TICK_TIME           10
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct 
{
    Runnable_t* runnable  ;
    uint32_t   RemainTime;
}RunableInfo_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static RunableInfo_t RunInfo[_MAX_RUNNABLE];
extern const Runnable_t UserRunnables[_MAX_RUNNABLE];


volatile static uint32_t SchedulerTicks;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void Scheduler(void)
{
    for(uint32_t priorityIdx=0;priorityIdx<_MAX_RUNNABLE;priorityIdx++)
    {
        if((RunInfo[priorityIdx].runnable->CallBack)&&(RunInfo[priorityIdx].RemainTime==0))
        {
            RunInfo[priorityIdx].runnable->CallBack();
            RunInfo[priorityIdx].RemainTime=RunInfo[priorityIdx].runnable->PeriodicityMS;
        }
        RunInfo->runnable->PeriodicityMS-=TICK_TIME;
    }
  
}

void SchedulerTicksCB(void)
{
    /*This Variable will increament When SysTick Interrupt Occured*/
    SchedulerTicks++;
}

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
void Scheduler_Init(void)
{
    ErrorStatus_t ReturnError;
    ReturnError=SYSTICK_SetTimeMs(TICK_TIME);
    ReturnError=SYSTICK_SetCallBack(SchedulerTicksCB);
    for(uint32_t priorityIdx=0;priorityIdx<_MAX_RUNNABLE;priorityIdx++)
    {
        if((UserRunnables[priorityIdx].CallBack)&&((RunInfo[priorityIdx].runnable->CallBack)==NULL))
        {
            *(RunInfo[priorityIdx].runnable)=UserRunnables[priorityIdx];
            RunInfo[priorityIdx].RemainTime=UserRunnables[priorityIdx].FirstDelayMS;
        }
    }
}
void Scheduler_Start(void)
{
        ErrorStatus_t ReturnError;
        ReturnError=SYSTICK_Start(SYSTICK_ENABLE_INT_AHB,SYSTICK_MODE_PERIODIC);
        while (1)
        {
            if(SchedulerTicks)
            {
                SchedulerTicks--;
                Scheduler();
            }
        }      
} 
