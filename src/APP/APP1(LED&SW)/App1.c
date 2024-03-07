#include "APP/APP.h"
#ifdef APP
#if (APP==APP1_LED_SWITCH)
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LED/LED.h"
#include "HAL/SWITCH/SWITCH.h"

int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;

	ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOB);
	ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOC);

	uint32_t SwitchValue;
	uint32_t counter=0;
	ReturnError=LED_Init();
	ReturnError=Switch_Init();
	while (1)
    {
		ReturnError=Switch_GetState(SWITCH_1,&SwitchValue);
		if(SwitchValue==SWITCH_PRESSED)
		{
			ReturnError=LED_SetState(LED1,LED_ON);
			while (counter<100000)
			{
				counter++;
			}
			ReturnError=LED_SetState(LED1,LED_OFF);
			
			
		}	
    }
}
#endif
#endif
