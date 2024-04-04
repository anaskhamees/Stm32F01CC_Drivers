/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/RCC/RCC.h"
#include "CLK_Control.h"

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


/**
  * @brief  Enables the peripheral clock for a specific GPIO port.
  * @param  Port: GPIO port to enable its clock.
  *          This parameter can be one of the following values:
  *            - HAL_GPIOA: GPIO port A
  *            - HAL_GPIOB: GPIO port B
  *            - HAL_GPIOC: GPIO port C
  *            - HAL_GPIOD: GPIO port D
  *            - HAL_GPIOE: GPIO port E
  *            - HAL_GPIOH: GPIO port H
  * @retval ErrorStatus_t: Status of the operation.
  *          This parameter can be one of the following values:
  *            - OK: Operation completed successfully
  *            - NOK: Operation failed
  *            - WRONG_PARAMETER: Invalid GPIO port specified
  */
ErrorStatus_t HAL_PeripheralEnableCLK(uint32_t Bus)
{
    ErrorStatus_t RetError;
    if(Bus >HAL_USART6)  
    {
        RetError=WRONG_PARAMETER;
    }
    else
    {
        switch (Bus)
        {
        case HAL_GPIOA:
	        RetError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOAEN,RCC_PERIPHERAL_ON);
            break;
        case HAL_GPIOB:
	        RetError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOBEN,RCC_PERIPHERAL_ON);
            break;
        case HAL_GPIOC:
	        RetError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOCEN,RCC_PERIPHERAL_ON);
            break;
         case HAL_GPIOD:
	        RetError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIODEN,RCC_PERIPHERAL_ON);
            break;
         case HAL_GPIOE:
	        RetError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOEEN,RCC_PERIPHERAL_ON);
            break;
         case HAL_GPIOH:
	        RetError=RCC_ControlPeripheralCLK(RCC_AHB1_BUS_REGISTER,RCC_AHB1ENR_GPIOHEN,RCC_PERIPHERAL_ON);
            break;
         case HAL_USART1:
	        RetError=RCC_ControlPeripheralCLK(RCC_APB2_BUS_REGISTER,RCC_APB2ENR_USART1EN,RCC_PERIPHERAL_ON);
            break; 
        case HAL_USART2:
	        RetError=RCC_ControlPeripheralCLK(RCC_APB1_BUS_REGISTER,RCC_APB1ENR_USART2EN,RCC_PERIPHERAL_ON);
            break; 
         case HAL_USART6:
	        RetError=RCC_ControlPeripheralCLK(RCC_APB2_BUS_REGISTER,RCC_APB2ENR_USART6EN,RCC_PERIPHERAL_ON);
            break;
        default:
            RetError=NOK;
            break;
        }
        RetError=OK;
    }
    return RetError;

}


