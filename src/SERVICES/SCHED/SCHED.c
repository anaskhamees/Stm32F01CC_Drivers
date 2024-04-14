/*
* File:    SCHED.c
* Author:  Yasmin Amr
* Created: 14/3/2024
* Brief:   SCHED Driver
* Target:  STM32F401cc
*/

#include "MCAL/SYSTICK_Driver/SYSTICK.h" /*Scheduler only depends on systick*/
#include "SCHED.h"

/**************************************************************************/
/*						Implementation Defs               	 			  */
/**************************************************************************/
#define ACTIVATED   0
#define DEACTIVATED 1

/**************************************************************************/
/*						Static Function Prototypes      	 			  */
/**************************************************************************/

/**
 * Function to Execute Runnable CallBack Function After Periodicity Time is Reached
*/
static void SCHED_App(void);

/**
 * Function to Increment 'PendingTicks' variable every SystTick/Scheduler Interrupt
*/
static void SCHED_TickCallBack(void);

/*Extern Runnables Array*/
extern Runnables_t System_Runnables[_MAX_RUNNABLE];

/**
 * Variable to Track Scheduler Interrupt
 * Note: Variable must be declared as volatile, to prevent compiler optimization as variable is modified in ISR
 */
static volatile uint32_t PendingTicks=0;
static uint32_t RemainTimeMs[_MAX_RUNNABLE]={0};

/**
 * Function to Initialize Scheduler (Initializes SysTick Timer)
*/
void SCHED_Init(void)
{
    STK_SetTimeMS(TICK_TIME);
    STK_SetCallBack(SCHED_TickCallBack);
    for(uint32_t Runnable_Index=0;Runnable_Index<_MAX_RUNNABLE;Runnable_Index++)
    {
        RemainTimeMs[Runnable_Index]=System_Runnables[Runnable_Index].FirstDelayMs;
    }
}

/**
 * Function to Start Scheduler (Starts SysTick Timer)
*/
void SCHED_Start(void)
{
    STK_Start(STK_PERIODICITY_INFINITE); 
    while(1)
    { 
        if(PendingTicks)
        {
            PendingTicks--;
            SCHED_App();
        }
    }
}

/**
 * Function to Execute Runnable CallBack Function After Periodicity Time is Reached
*/
static void SCHED_App(void)
{
    for(uint32_t Runnable_Index=0;Runnable_Index<_MAX_RUNNABLE;Runnable_Index++)
    {
        /*Check if Periodicity Time is Reached*/
        if(((RemainTimeMs[Runnable_Index])==0)&&(System_Runnables[Runnable_Index].CallBackFn))
        {
            System_Runnables[Runnable_Index].CallBackFn();
            RemainTimeMs[Runnable_Index]=System_Runnables[Runnable_Index].PeriodicityMs;
        }
        else
        {
            //Empty for MISRA
        }
        RemainTimeMs[Runnable_Index]-=TICK_TIME;
    }
}

/**
 * Function to Increment 'PendingTicks' variable every SystTick/Scheduler Interrupt
*/
static void SCHED_TickCallBack(void)
{
    PendingTicks++;
}