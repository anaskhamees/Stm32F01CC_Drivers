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

  [HSwitchRunnable]={
   .PeriodicityMS=5,
   .FirstDelayMS=100,
   .CallBack=HSwitch_Runnable
 },
 [STOP_WATCH]={
    .FirstDelayMS=100,
    .PeriodicityMS=100,
    .CallBack=LCD_DisplayStopwatch
  },

[DISPLAY_APP]={
    .FirstDelayMS=100,
    .PeriodicityMS=125,
    .CallBack=Display_App
  },

 [AppButtonsRun]={
   .PeriodicityMS=200,
   .FirstDelayMS=150,
   .CallBack=AppButtons_Runnable
 },

  [DATE_TIME]={
    .FirstDelayMS=100,
    .PeriodicityMS=1000,
    .CallBack=LCD_DisplayDateTime 
    },
  
  /*[LCD_APP_RUN]={
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
 
 /*[TRAFFIC_LIGHT]={
  .PeriodicityMS=2000,
  .FirstDelayMS=0,
  .CallBack=TrafficLight
 }*/
};