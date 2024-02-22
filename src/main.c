#include "MCAL/RCC/RCC.h"
#include "HAL/LED/LED.h"
int main(int argc, char* argv[])
{
	ErrorStatus_t ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOCEN,RCC_PERIPHERAL_ON);
	ReturnError=LED_Init();
	ReturnError=LED_SetState(LED1,LED_ON);
	//ReturnError=
/*	while (1)
    {
       // Add your code here.
    }*/
}
