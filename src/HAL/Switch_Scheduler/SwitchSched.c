/*
 * File Name: SwitchSched.c
 * Layer	: HAL
 * Module	: Switch without bouncing
 * Version  : V 0.1
 * Created	: 15/3/2024 
 * Author	: Anas Khamees
 */

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "SwitchSched.h"
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

extern HSwitchCFG_t  HSwitches[_HSWITCHES_NUM];
static uint32_t HSwitchState[_HSWITCHES_NUM]={0};

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
ErrorStatus_t HSwitch_Init(void)
{
    ErrorStatus_t   ReturnState;
   
    GPIO_CFG_t HSwitch;
  
    uint8_t SW_Num;
    
    HSwitch.GPIO_Speed=GPIO_HIGH_SPEED;
    HSwitch.GPIO_AF=GPIO_AF_DISABLED;
    for (SW_Num=0;SW_Num<_HSWITCHES_NUM;SW_Num++)
    {
        HSwitch.GPIO_Port=HSwitches[SW_Num].SW_Port;
        HSwitch.GPIO_Pin=HSwitches[SW_Num].SW_Pin;
        HSwitch.GPIO_Mode=HSwitches[SW_Num].SW_Connection;
 
        ReturnState=GPIO_InitPin(&HSwitch);
    }
   
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
   ErrorStatus_t HSwitch_GetState(uint32_t SwitchNum ,uint32_t* SwitchState)
   {
    ErrorStatus_t ReturnError;
    if(SwitchState==NULL)
    {
        ReturnError=NULL_POINTER;
    }
    else if(SwitchNum>(uint32_t)_HSWITCHES_NUM)
    {
        ReturnError=WRONG_PARAMETER;
    }
    else
    {
       *SwitchState=HSwitchState[SwitchNum]^(((HSwitches[SwitchNum].SW_Connection)>>MASK_SHIF_SW_CONNECTION)&(MASK_GET_SW_CONNECTION_LSB));
       ReturnError=OK;
    }
    return ReturnError;
   }

/**
 * @brief  Runnable function to handle the Bouncing of hardware switches.
 */
   void HSwitch_Runnable(void)
   {
         uint32_t CurrentState=0;
         static uint32_t PrevoiusState[_HSWITCHES_NUM]={0};
         static uint32_t Counts[_HSWITCHES_NUM]={0};
         ErrorStatus_t ReturnError;
         for(uint8_t SwitchNum=0;SwitchNum<_HSWITCHES_NUM;SwitchNum++)
         {
             ReturnError=GPIO_GetPinValue(HSwitches[SwitchNum].SW_Port,HSwitches[SwitchNum].SW_Pin,&CurrentState);
            if(CurrentState==PrevoiusState[SwitchNum])
            {
                Counts[SwitchNum]++;
            }
            else
            {
                Counts[SwitchNum]=0;
            }
            if(Counts[SwitchNum]%5==0)
            {
                HSwitchState[SwitchNum]=CurrentState;
            }
            PrevoiusState[SwitchNum]=CurrentState;
         }

   }

