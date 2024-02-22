#ifndef HAL_SWITCH_SWITCH_H_
#define HAL_SWITCH_SWITCH_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Std_Lib/Std_Libraries.h"
#include "SWITCH_CFG.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define SWITCH_PRESSED      (1UL)
#define SWITCH_RELEASED     (0UL)

#define SW_PULL_UP          (0x00000008UL)
#define SW_PULL_DOWN        (0x00000010UL)

   // *SwitchState=(*SwitchState)^SWITCHES[SwitchNum].SW_Connection;

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct {
    void *SW_Port;          /**< The SW_Port member represents the GPIO port to which the switch is connected.*/ 
                               /* Available options:
                                * - GPIO_PORTA: GPIO port A.
                                * - GPIO_PORTB: GPIO port B.
                                * - GPIO_PORTC: GPIO port C.
                                * - GPIO_PORTD: GPIO port D.
                                * - GPIO_PORTE: GPIO port E.
                                * - GPIO_PORTH: GPIO port H.
                                */
    uint32_t SW_Pin;        /**<  The SW_Pin member represents the pin number to which the switch is connected.*/
                               /*Available options: GPIO_PIN0 to GPIO_PIN15 .*/
    uint32_t SW_Connection; /**< The SW_Connection member represents the connection configuration of the switch.*/
                              /* - SW_PULL_UP  : Pull-up configuration for the switch.
                               * - SW_PULL_DOWN: Pull-down configuration for the switch.
                               */
} SwitchCFG_t;



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

ErrorStatus_t Switch_Init(void);
ErrorStatus_t Switch_GetState(uint32_t SwitchNum ,uint32_t* SwitchState);













#endif // HAL_SWITCH_SWITCH_H_