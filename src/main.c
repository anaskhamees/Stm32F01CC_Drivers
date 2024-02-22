#include "MCAL/RCC/RCC.h"
#include "HAL/LED/LED.h"
#include "HAL/SWITCH/SWITCH.h"
int main(int argc, char* argv[])
{
	
	ErrorStatus_t ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOBEN,RCC_PERIPHERAL_ON);
	uint32_t SwitchValue;
	//ReturnError=LED_Init();
	ReturnError=Switch_Init();
	while (1)
    {
		ReturnError=Switch_GetState(SWITCH_1,&SwitchValue);
		
    }
}
