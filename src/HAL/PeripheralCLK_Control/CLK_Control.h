#ifndef HAL_PERIPHERALCLK_CONTROL_CLK_CONTROL_H_
#define HAL_PERIPHERALCLK_CONTROL_CLK_CONTROL_H_



/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Std_Lib/Std_Libraries.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define HAL_GPIOA       (0UL) 
#define HAL_GPIOB       (1UL) 
#define HAL_GPIOC       (2UL) 
#define HAL_GPIOD       (3UL) 
#define HAL_GPIOE       (4UL) 
#define HAL_GPIOH       (5UL) 
#define HAL_USART1      (6UL)
#define HAL_USART2      (7UL)
#define HAL_USART6      (8UL)

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
  * @brief  Enables the peripheral clock for a specific GPIO port.
  * @param  Bus: GPIO port to enable its clock.
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
ErrorStatus_t HAL_PeripheralEnableCLK(uint32_t Bus);



#endif // HAL_PERIPHERALCLK_CONTROL_CLK_CONTROL_H_