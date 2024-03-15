#ifndef APP_APPRUNNABLES_H_
#define APP_APPRUNNABLES_H_

#include "APP.h"
#if(APP==APP5_SCHEDULER_TEST)
extern void ToggleLed1(void);
extern void ToggleLed2(void);
#elif(APP==APP6_SWITCH_DEBOUNCING)
extern void HSwitch_Runnable(void);
extern void AppSWitch_Runnable(void);
#endif


#endif // APP_APPRUNNABLES_H_