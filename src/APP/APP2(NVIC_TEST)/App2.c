#include "APP/APP.h"
#ifdef APP
#if (APP==APP2_NVIC_TEST)
#include "MCAL/NVIC/NVIC.h"
#include "MCAL/NVIC/STM32F401xx.h"

int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;
    uint8_t PendingState=0;
    uint8_t ActiveState=0;


   /* 
    ReturnError=NVIC_EnableIRQ(NIVC_EXTERNAL_LINE16);
    ReturnError=NVIC_SetPendingIRQ(NIVC_EXTERNAL_LINE16);
    ReturnError=NVIC_GetActiveState(NIVC_EXTERNAL_LINE16,&ActiveState);
    ReturnError=NVIC_ClearPendingIRQ(NIVC_EXTERNAL_LINE16);
    ReturnError=NVIC_EnableIRQ(NVIC_ADC_INTERRUPT);
    ReturnError=NVIC_SetPendingIRQ(NVIC_ADC_INTERRUPT);
    ReturnError=NVIC_GetPendingIRQ(NVIC_ADC_INTERRUPT,&PendingState);*/
    ReturnError=NVIC_EnableIRQ(NVIC_WINDOW_WATCHDOG_TIMER);
	ReturnError=NVIC_SetPriority(NVIC_WINDOW_WATCHDOG_TIMER,NVIC_PREEMPTION_2_SUBGROUP_EIGHT,0,7);
    ReturnError=NVIC_DisableIRQ(NVIC_WINDOW_WATCHDOG_TIMER);

    /*while (1)
    {
	
    }*/
    return 0;
}
#endif
#endif
