#include "APP/APP.h"
#ifdef APP
#if (APP==APP4_SYSTICK_TEST)
#include "MCAL/SYSTICK/SysTick.h"
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LED/LED.h"

void ToggleLed(void) {
    static uint32_t State = 0;
     ErrorStatus_t RetError;
    State ^= 1; 
    if(State==1)
    {
        RetError = LED_SetState(LED1, LED_ON);
    }
    else
    {
        RetError=LED_SetState(LED1,LED_OFF);
    }
     
}

int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);
	ReturnError=LED_Init();
    ReturnError=SYSTICK_SetTimeMs(1000);
    ReturnError=SYSTICK_SetCallBack(ToggleLed);
    ReturnError=SYSTICK_Start(SYSTICK_ENABLE_INT_AHB,SYSTICK_MODE_PERIODIC);


    while (1)
    {
	
    }
    return 0;
}
#endif
#endif
