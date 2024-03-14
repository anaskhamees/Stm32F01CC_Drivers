#include "APP/APP.h"
#ifdef APP
#if (APP==APP1_LED)
#include "HAL/PeripheralCLK_Control/CLK_Control.h"
#include "HAL/LED/LED.h"


int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError;

	ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOB);
	ReturnError=HAL_PeripheralEnableCLK(HAL_GPIOA);

	
	uint32_t counter=0;
	ReturnError=LED_Init();

	while (1)
    {
            counter=0;
			ReturnError=LED_SetState(LED1,LED_ON);
			while (counter<10000000)
			{
				counter++;
			}
             counter=0;
			ReturnError=LED_SetState(LED1,LED_OFF);
            while (counter<10000000)
			{
				counter++;
			}		
	}	       
}
#endif
#endif
