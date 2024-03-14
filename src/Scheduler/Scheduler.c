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
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct 
{
    UserRunnable_t runnable  ;
    uint32_t   RemainTime;
}RunableInfo_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static RunableInfo_t RunInfo[_MAX_RUNNABLE];
extern const UserRunnable_t UserRunnables[_MAX_RUNNABLE];


volatile static uint32_t SchedulerTicks;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/**
 * @brief   Executes the scheduler.
 * @details This function is called periodically by the scheduler to execute the runnable tasks.
 *          It iterates through all runnable tasks, checks if any task is ready to run based on
 *          its remaining time, and executes the task if it's ready.
 *          After executing a task, it updates its remaining time to its periodicity.
 */
static void Scheduler(void)
{
    for (uint32_t priorityIdx = 0; priorityIdx < _MAX_RUNNABLE; priorityIdx++)
    {
        if ((RunInfo[priorityIdx].runnable.CallBack) && (RunInfo[priorityIdx].RemainTime == 0))
        {
            /* Execute runnable task */
            RunInfo[priorityIdx].runnable.CallBack();
            RunInfo[priorityIdx].RemainTime = RunInfo[priorityIdx].runnable.PeriodicityMS;
        }

        RunInfo[priorityIdx].RemainTime -= TICK_TIME;
    }
}

/**
 * @brief   Callback function for scheduler ticks.
 * @details This function is called when a system tick interrupt occurs, and it increments
 *          the `SchedulerTicks` variable, indicating that a scheduler tick has occurred.
 */
static void SchedulerTicksCB(void)
{
    // Increment scheduler ticks counter
    SchedulerTicks++;
}

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void Scheduler_Init(void)
{
    ErrorStatus_t ReturnError;
    ReturnError = SYSTICK_SetTimeMs(TICK_TIME);
    ReturnError = SYSTICK_SetCallBack(SchedulerTicksCB);

    /* Initialize runnable tasks*/
    for (uint32_t priorityIdx = 0; priorityIdx < _MAX_RUNNABLE; priorityIdx++)
    {
         if (UserRunnables[priorityIdx].CallBack && (RunInfo[priorityIdx].runnable.CallBack == NULL))
        {
            RunInfo[priorityIdx].runnable = UserRunnables[priorityIdx];
            RunInfo[priorityIdx].RemainTime = UserRunnables[priorityIdx].FirstDelayMS;
        }
    }
}


void Scheduler_Start(void)
{
    ErrorStatus_t ReturnError;

    /* Start system tick */
    ReturnError = SYSTICK_Start(SYSTICK_ENABLE_INT_AHB, SYSTICK_MODE_PERIODIC);

    while (1)
    {
        if (SchedulerTicks)
        {
            SchedulerTicks--;
            Scheduler(); 
        }
    }      
}

