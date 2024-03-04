/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "SWITCH.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/GPIO/GPIO_Validation.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define MASK_SHIF_SW_CONNECTION        (3UL)
#define MASK_GET_SW_CONNECTION_LSB     (1UL)


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

extern SwitchCFG_t  Switches[_SWITCHES_NUM];

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

/**
 * @brief Initializes the switches by Provided Configuration.
 * 
 * This function initializes the switches by configuring their GPIO pins according to the settings
 * specified in the SWITCHES array.
 * @return ErrorStatus_t Returns SUCCESS if the switches are successfully initialized, otherwise returns an error code.
 */
ErrorStatus_t Switch_Init(void)
{
    ErrorStatus_t   ReturnState;
   
    GPIO_CFG_t Switch;
  
    uint8_t SW_Num;
    for (SW_Num=0;SW_Num<_SWITCHES_NUM;SW_Num++)
    {
        Switch.GPIO_Port=Switches[SW_Num].SW_Port;
        Switch.GPIO_Pin=Switches[SW_Num].SW_Pin;
        Switch.GPIO_Mode=Switches[SW_Num].SW_Connection;
    }
    Switch.GPIO_Speed=GPIO_HIGH_SPEED;
    Switch.GPIO_AF=GPIO_AF_DISABLED;
    ReturnState=GPIO_InitPin(&Switch);
    return ReturnState;
   }

/**
 * @brief Gets the state of a switch Pressed or Released. 
 * @param SwitchNum The switch number to get the state of.
 * @param SwitchState Pointer to store the state of the switch (1 for pressed, 0 for released).
 * @return ErrorStatus_t Returns SUCCESS if the state is successfully retrieved, otherwise returns an error code:
 *         - NULL_POINTER if the SwitchState pointer is NULL.
 *         - WRONG_PARAMETER if SwitchNum is out of range.
 */
   ErrorStatus_t Switch_GetState(uint32_t SwitchNum ,uint32_t* SwitchState)
   {
    ErrorStatus_t ReturnError;
    if(SwitchState==NULL)
    {
        ReturnError=NULL_POINTER;
    }
    else if(SwitchNum>(uint32_t)_SWITCHES_NUM)
    {
        ReturnError=WRONG_PARAMETER;
    }
    else
    {
        ReturnError=GPIO_GetPinValue(Switches[SwitchNum].SW_Port,Switches[SwitchNum].SW_Pin,SwitchState);

        /**
         * ((SWITCHES[SwitchNum].SW_Connection)>>MASK_SHIF_SW_CONNECTION) : 
         * @brief : I used tha same Mask of GPIO , so I want to shift it by 3 to get the value of connection
         *  &(MASK_GET_SW_CONNECTION_LSB):
         * @brief : Then I masked the Output of the previous MASK by 0x00000001 to Get the LSB bit to know the Switch State.
         * @brief : (*Switch State) is the result of XORing operation as follow  :
         * 
         *   +----------------------+--------------------+--------+
             |   Switch State       | Switch Connection  | Result |
             +----------------------+--------------------+--------+
             | SWITCH_PRESSED  (1)  |    GPIO_IN_PU (1)  |   0    |
             | SWITCH_PRESSED  (1)  |    GPIO_IN_PD (0)  |   1    |
             | SWITCH_RELEASED (0)  |    GPIO_IN_PU (1)  |   1    |
             | SWITCH_RELEASED (0)  |    GPIO_IN_PD (0)  |   0    |
             +----------------------+--------------------+--------+           
         */
        *SwitchState=(*SwitchState)^(((Switches[SwitchNum].SW_Connection)>>MASK_SHIF_SW_CONNECTION)&(MASK_GET_SW_CONNECTION_LSB));
    }
    return ReturnError;
   }

