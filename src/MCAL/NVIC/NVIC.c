/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "NVIC.h"
#include "STM32F401xx.h"
#include "NVIC_Validation.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define NVIC_PREPHERAL_BASE_ADDRESS     (0xE000E100UL)
#define SCB_PREPHERAL_BASE_ADDRESS      (0xE000ED00UL)
#define MASK_SHIFT_8                    (0x00000008UL)
#define MASK_READ_3BITS                 (0x00000007UL)
#define MASK_NONE_SUBGROUP              (0x03U)
/********************************************************************************************************/
/************************************************ Types *************************************************/
/********************************************************************************************************/
typedef struct
{
    uint32_t ISER[8];
    uint32_t RESERVED0[24];
    uint32_t ICER[8];
    uint32_t RESERVED1[24];
    uint32_t ISPR[8];
    uint32_t RESERVED2[24];
    uint32_t ICPR[8];
    uint32_t RESERVED3[24];
    uint32_t IABR[8];
    uint32_t RESERVED4[56];
    uint8_t  IPR[240]; /*Byte Accessible: 60*4=240*/
    uint32_t RESERVED5[580];
    uint32_t STIR;
}NVIC_Reg_t;

typedef struct 
{
    uint32_t CPUID;
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint32_t SHPR1;
    uint32_t SHPR2;
    uint32_t SHPR3;
    uint32_t SHCSR;
    uint32_t CFSR;
    uint32_t HFSR;
    uint32_t MMAR;
    uint32_t BFAR;
    uint32_t AFSR;

}SCB_Reg_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile NVIC_Reg_t*const NVIC=(volatile NVIC_Reg_t*)(NVIC_PREPHERAL_BASE_ADDRESS);
volatile SCB_Reg_t*const SCB=(volatile SCB_Reg_t*)(SCB_PREPHERAL_BASE_ADDRESS);
/********************************************************************************************************/
/**************************************** Static Function ***********************************************/
/********************************************************************************************************/
static uint8_t TwoPowerBitsNum(uint8_t BitsNum)
{
    uint8_t MaxValue=1;
    for (uint8_t i = 0; i < BitsNum; i++)
    {
        MaxValue*=2;
    }
    return MaxValue;
}
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
/**
  * @brief  Enables a specific IRQ.
  * @param  IRQ: The IRQ number to be enabled.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_EnableIRQ(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if (IRQ > MAX_IRQ_VALUE)
    {
        Return_State = NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum = IRQ / 32;
        uint8_t BitNum = IRQ % 32;
        NVIC->ISER[RegisterNum] |= ((1UL) << BitNum);
        Return_State = NVIC_OK;
    }
    return Return_State;
}

/**
  * @brief  Disables a specific IRQ.
  * @param  IRQ: The IRQ number to be disabled.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_DisableIRQ(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if (IRQ > MAX_IRQ_VALUE)
    {
        Return_State = NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum = IRQ / 32;
        uint8_t BitNum = IRQ % 32;
        NVIC->ICER[RegisterNum] |= ((1UL) << BitNum);
        Return_State = NVIC_OK;
    }
    return Return_State;
}

/**
  * @brief  Sets a pending state for a specific IRQ.
  * @param  IRQ: The IRQ number to set its pending state.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_SetPendingIRQ(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if (IRQ > MAX_IRQ_VALUE)
    {
        Return_State = NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum = IRQ / 32;
        uint8_t BitNum = IRQ % 32;
        NVIC->ISPR[RegisterNum] |= ((1UL) << BitNum);
        Return_State = NVIC_OK;
    }
    return Return_State;
}

/**
  * @brief  Clears a pending state for a specific IRQ.
  * @param  IRQ: The IRQ number to clear its pending state.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_ClearPendingIRQ(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if (IRQ > MAX_IRQ_VALUE)
    {
        Return_State = NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum = IRQ / 32;
        uint8_t BitNum = IRQ % 32;
        NVIC->ICPR[RegisterNum] = ((1UL) << BitNum);
        Return_State = NVIC_OK;
    }
    return Return_State;
}

/**
  * @brief  Gets the pending state of a specific IRQ.
  * @param  IRQ: The IRQ number to get its pending state.
  * @param  PendingState: Pointer to where the pending state will be stored.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_GetPendingIRQ(uint8_t IRQ, uint8_t *PendingState)
{
    ErrorStatus_t Return_State;
    if (IRQ > MAX_IRQ_VALUE)
    {
        Return_State = NVIC_WRONG_PARAMETER;
    }
    else if (!PendingState)
    {
        Return_State = NULL_POINTER;
    }
    else
    {
        uint8_t RegisterNum = IRQ / 32;
        uint8_t BitNum = IRQ % 32;
        *PendingState = (((NVIC->ISPR[RegisterNum]) >> BitNum) & (1UL));
        Return_State = NVIC_OK;
    }
    return Return_State;
}

/**
  * @brief  Gets the active state of a specific IRQ.
  * @param  IRQ: The IRQ number to get its active state.
  * @param  ActiveState: Pointer to where the active state will be stored.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_GetActiveState(uint8_t IRQ, uint8_t *ActiveState)
{
    ErrorStatus_t Return_State;
    if (IRQ > MAX_IRQ_VALUE)
    {
        Return_State = NVIC_WRONG_PARAMETER;
    }
     else if (!ActiveState)
    {
        Return_State = NULL_POINTER;
    }
    else
    {
        uint8_t RegisterNum = IRQ / 32;
        uint8_t BitNum = IRQ % 32;
        *ActiveState = (((NVIC->IABR[RegisterNum]) >> BitNum) & (1UL));
        Return_State = NVIC_OK;
    }
    return Return_State;
}

/**
  * @brief  Generates a software interrupt for a specific IRQ.
  * @param  IRQ: The IRQ number to generate a software interrupt for.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_GenerateSoftwareInterrupt(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if (IRQ > MAX_IRQ_VALUE)
    {
        Return_State = NVIC_WRONG_PARAMETER;
    }
    else
    {
        NVIC->STIR = IRQ;
        Return_State = NVIC_OK;
    }
    return Return_State;
}

ErrorStatus_t NVIC_SetPriority(uint8_t IRQ,uint32_t PriorityGrouping,uint8_t Preemption,uint8_t SubGroup)
{
      ErrorStatus_t Return_State;
      /*Get the number of bits of SubGroup in PriorityGrouping value*/
      uint8_t SubGroupBits=((((uint8_t)(PriorityGrouping>>MASK_SHIFT_8))&MASK_READ_3BITS)-MASK_NONE_SUBGROUP);
      uint8_t PreemptionBits=IMPLEMENTED_IPR_BITS-SubGroupBits;
      uint8_t MaxSubGroupValue=(TwoPowerBitsNum(SubGroupBits)-1);
      uint8_t MaxPreemptionValue=(TwoPowerBitsNum(PreemptionBits)-1);
      if(IRQ>MAX_IRQ_VALUE) 
      {
        Return_State=NVIC_WRONG_PARAMETER;
      }
      else if(!IS_PRIORITY_GROUPING_VALID(PriorityGrouping))
      {
        Return_State=NVIC_WRONG_PARAMETER;
      }
      else if (Preemption>MaxPreemptionValue)
      {
        Return_State=NVIC_WRONG_PARAMETER;
      }
       else if (SubGroup>MaxSubGroupValue)
      {
        Return_State=NVIC_WRONG_PARAMETER;
      }
      else
      {
        SCB->AIRCR=PriorityGrouping;
        NVIC->IPR[IRQ]=((SubGroup|(Preemption<<SubGroupBits))<<IMPLEMENTED_IPR_BITS);
        Return_State=NVIC_OK;
      }
      return Return_State;
}


