/*
* File:    SCHED_cfg.h
* Author:  Yasmin Amr
* Created: 14/3/2024
* Brief:   SCHED Driver
* Target:  STM32F401cc
*/

#ifndef SCHED_CFG_H_
#define SCHED_CFG_H_

#include "Std_Lib/Std_Libraries.h"
#include "SCHED.h"

/*Modify Tick Time of Scheduler - Done by ARCHITECT */
#define TICK_TIME                1
/*Modify your Runnables Count*/
// #define SYSTEM_RUNNABLES_COUNT      2
/*Define Your Runnable Name According to Array Index*/
// #define LCD_Runnable                         0
//#define APP_LCD_Runnable                     1

typedef enum
{
    LCD_TASK_RUN,
    HSwitchRunnable,
    STOP_WATCH,
    DISPLAY_APP,
    AppButtonsRun,
    DATE_TIME,
    _MAX_RUNNABLE
}runnable_list_t;

#endif // SCHED_CFG_H_

