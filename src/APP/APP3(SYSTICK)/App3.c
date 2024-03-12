#include "APP/APP.h"
#ifdef APP
#if (APP==APP3_SYSTICK_TEST)
#include "MCAL/SYSTICK/SysTick.h"
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LED/LED.h"

void AppHandler(void)
{
    ErrorStatus_t ReturnError;
	static uint32_t state = 1 ;
	state ^= 1 ;
	ReturnError=LED_SetState(LED1, state);
}

int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;
    ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOB);
	ReturnError=LED_Init();
    ReturnError=SYSTICK_SetTimeMs(1000);
    ReturnError=SYSTICK_SetCallBack(AppHandler);
    ReturnError=SYSTICK_Start(SYSTICK_ENABLE_INT_AHB,SYSTICK_MODE_PERIODIC);


    while (1)
    {
	
    }
    return 0;
}
#endif
#endif
