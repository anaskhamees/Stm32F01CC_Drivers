/*
* File:    SCHED_cfg.c
* Author:  Yasmin Amr
* Created: 14/3/2024
* Brief:   SCHED Driver
* Target:  STM32F401cc
*/

#include "SCHED_cfg.h"
#include "APP/AppRunnables.h"

// extern void Traffic_Lights (void);
// extern void Toggle_LED (void);
// extern void SW_Runnable (void);
// extern void Switch_Led_Runnable (void);
// extern void LCD_Task(void);
// extern void LCD_APP_Runnable(void);

/**
 * 1) Extern Your Runnable Here
 * 2) Add Your Runnables Information Here
 *      Example: 
 *          Runnables_t System_Runnables[]=
 *            {
 *             [0]={
 *             .name="FirstRunnable",
 *             .PeriodicityMs=100,
 *             .CallBackFn=Toggle_LED,
 *             .FirstDelayMs=5000
 *            }
 *          };
 * 
 * Add Your Runnable in order of PRIORITY
 */
 

const Runnables_t System_Runnables[_MAX_RUNNABLE]=
{
  [LCD_TASK_RUN]={
    .FirstDelayMs=3,
    .PeriodicityMs=2,
    .CallBackFn=LCD_Task
  },
  [HSwitchRunnable]={
   .PeriodicityMs=5,
   .FirstDelayMs=6,
   .CallBackFn=HSwitch_Runnable
 },
 [STOP_WATCH]={
    .FirstDelayMs=100,
    .PeriodicityMs=100,
    .CallBackFn=LCD_DisplayStopwatch
  },
[DISPLAY_APP]={
    .FirstDelayMs=131,
    .PeriodicityMs=125,
    .CallBackFn=Display_App
  },

 [AppButtonsRun]={
   .PeriodicityMs=150,
   .FirstDelayMs=152,
   .CallBackFn=AppButtons_Runnable
 },
  [DATE_TIME]={
    .FirstDelayMs=1004,
    .PeriodicityMs=1000,
    .CallBackFn=LCD_DisplayDateTime 
    }
};