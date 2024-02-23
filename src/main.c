#include "MCAL/RCC/RCC.h"
#include "HAL/LED/LED.h"
#include "HAL/SWITCH/SWITCH.h"
int main(int argc, char* argv[])
{
	
	ErrorStatus_t ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOCEN,RCC_PERIPHERAL_ON);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOBEN,RCC_PERIPHERAL_ON);
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
