#include "MCAL/RCC/RCC.h"
#include "MCAL/GPIO/GPIO.h"
int main(int argc, char* argv[])
{

	GPIO_CFG_t pin0 ={
		.GPIO_Port=GPIO_PORTB,
		.GPIO_Pin=GPIO_PIN0,
		.GPIO_Mode=GPIO_OUT_PP_PD,
		.GPIO_Speed=GPIO_LOW_SPEED
	};
	GPIO_CFG_t pin1 ={
		.GPIO_Port=GPIO_PORTB,
		.GPIO_Pin=GPIO_PIN4,
		.GPIO_Mode=GPIO_OUT_PP_NO_PUPD,
		.GPIO_Speed=GPIO_LOW_SPEED
	};
	GPIO_CFG_t pin2 ={
		.GPIO_Port=GPIO_PORTB,
		.GPIO_Pin=GPIO_PIN2,
		.GPIO_Mode=GPIO_OUT_PP_NO_PUPD,
		.GPIO_Speed=GPIO_LOW_SPEED
	};

	ErrorStatus_t ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOBEN,RCC_PERIPHERAL_ON);
	ReturnError=GPIO_InitPin(&pin0);
	ReturnError=GPIO_InitPin(&pin1);
	ReturnError=GPIO_InitPin(&pin2);
	ReturnError=GPIO_SetPinValue(GPIO_PORTB,GPIO_PIN2,GPIO_SET_PIN_HIGH);
	 ReturnError=GPIO_SetPinValue(GPIO_PORTB,GPIO_PIN1,GPIO_SET_PIN_HIGH);
	//ReturnError=GPIO_SetPinValue(GPIO_PORTB,GPIO_PIN2,GPIO_SET_PIN_HIGH);

	//ReturnError=
/*	while (1)
    {
       // Add your code here.
    }*/
}
