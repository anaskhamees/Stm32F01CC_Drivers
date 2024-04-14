/*
* File:    SCHED.h
* Author:  Yasmin Amr
* Created: 14/3/2024
* Brief:   SCHED Driver
* Target:  STM32F401cc
*/

#ifndef SCHED_H_
#define SCHED_H_

/**************************************************************************/
/*						Includes                			 			  */
/**************************************************************************/
#include "SCHED_cfg.h"

/*Runnable Call Back Function Type - Called when Runnable Periodicity is Reached*/
typedef void (*runnablecb)(void);

/**************************************************************************/
/*						Single Runnable Info.    	    	 			  */
/**************************************************************************/
typedef struct 
{
    char*name;                  /*Runnable Name*/
    uint32_t PeriodicityMs;          /*Runnable Periodicity*/
    runnablecb CallBackFn;      /*Runnable CallBack Function*/
    uint32_t FirstDelayMs;             /*First Time Runnable Delay*/
}Runnables_t;

/**
 * Function to Initialize Scheduler (Initializes SysTick Timer)
*/
void SCHED_Init(void);

/**
 * Function to Start Scheduler (Starts SysTick Timer)
*/
void SCHED_Start(void);

#endif