#include "MCAL/RCC/RCC.h"
int main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.
	ErrorStatus_t ReturnError;
	PLLCFG_t PLL_Config={
			.PLLSRC=RCC_PLLSRC_HSI,
			.PLLM=10,
			.PLLN=100,
			.PLLP=RCC_PLLP_DIV6,
			.PLLQ=7
	};
	
//	ReturnError=RCC_EnableClock(RCC_HSI_HSE_PLL_PLLI2S_REGISTER,MASK_HSI_ON_OFF);
    ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOAEN,RCC_PERIPHERAL_ON);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOBEN,RCC_PERIPHERAL_ON);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOCEN,RCC_PERIPHERAL_ON);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIODEN,RCC_PERIPHERAL_ON);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOEEN,RCC_PERIPHERAL_ON);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOHEN,RCC_PERIPHERAL_ON);
    
    ReturnError=RCC_ConfigurePLL(&PLL_Config);
	ReturnError=RCC_EnableClock(RCC_HSI_HSE_PLL_PLLI2S_REGISTER,MASK_PLL_ON_OFF);
	
	ReturnError=RCC_CheckReadyCLK(MASK_PLL_READY);
	if(ReturnError==OK)
	{
		ReturnError=RCC_Select_SYSCLK(MASK_SYSCLK_PLL);
	}
    
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOAEN,RCC_PERIPHERAL_OFF);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOBEN,RCC_PERIPHERAL_OFF);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOCEN,RCC_PERIPHERAL_OFF);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIODEN,RCC_PERIPHERAL_OFF);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOEEN,RCC_PERIPHERAL_OFF);
	ReturnError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOHEN,RCC_PERIPHERAL_OFF);
	// Infinite loop
 /* while (1)
    {
       // Add your code here.
    }*/
}
