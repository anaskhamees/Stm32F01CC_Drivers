#ifndef APP_APPRUNNABLES_H_
#define APP_APPRUNNABLES_H_

#include "APP.h"
#if(APP==APP5_SCHEDULER_TEST)
extern void ToggleLed1(void);
extern void ToggleLed2(void);
#elif(APP==APP6_SWITCH_DEBOUNCING)
extern void HSwitch_Runnable(void);
extern void AppSWitch_Runnable(void);
#elif(APP==APP7_TRAFFIC_LIGHT)
extern void TrafficLight(void);
#elif(APP==APP8_LCD_SCHED_APP)
extern void LCD_App(void);
extern void LCD_Task(void);
#endif


#endif // APP_APPRUNNABLES_H_