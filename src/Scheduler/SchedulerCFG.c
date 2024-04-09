/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Scheduler.h"
#include "APP/AppRunnables.h"
/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/


const UserRunnable_t UserRunnables[_MAX_RUNNABLE]=
{
  [LCD_TASK_RUN]={
    .FirstDelayMS=0,
    .PeriodicityMS=2,
    .CallBack=LCD_Task
  },
  [STOP_WATCH]={
    .FirstDelayMS=0,
    .PeriodicityMS=100,
    .CallBack=LCD_DisplayStopWatch  
    },
  [DISPLAY_APP]={
    .FirstDelayMS=0,
    .PeriodicityMS=125,
    .CallBack=Display_App
  },
  /*[LCD_TASK_RUN]={
    .FirstDelayMS=0,
    .PeriodicityMS=2,
    .CallBack=LCD_Task
  },
  [LCD_APP_RUN]={
    .FirstDelayMS=0,
    .PeriodicityMS=1000,
    .CallBack=LCD_App
  }
 /*[Runnable_1]={
    .PeriodicityMS=500,
    .FirstDelayMS=0,
    .CallBack=ToggleLed1
 }, 
 [Runnable_2]={
    .PeriodicityMS=1000,
    .FirstDelayMS=0,
    .CallBack=ToggleLed2
 }*/
 /*[HSwitchRunnable]={
   .PeriodicityMS=5,
   .FirstDelayMS=0,
   .CallBack=HSwitch_Runnable
 },
 [AppSwitchRun]={
   .PeriodicityMS=150,
   .FirstDelayMS=0,
   .CallBack=AppSWitch_Runnable
 }*/
 /*[TRAFFIC_LIGHT]={
  .PeriodicityMS=2000,
  .FirstDelayMS=0,
  .CallBack=TrafficLight
 }*/
};