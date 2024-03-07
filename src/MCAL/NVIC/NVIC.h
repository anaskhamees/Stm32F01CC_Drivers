#ifndef MCAL_NVIC_NVIC_H_
#define MCAL_NVIC_NVIC_H_


/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Std_Lib/Std_Libraries.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/**
 * This values is combination of KeyValue of AIRCR Register and value of prioirty grouping 
 *  0xFA05 0000 + 00xx
 *  0xFA05 0000 + 0100
 *  0xFA05 0000 + 0101
 *  0xFA05 0000 + 0110
 *  0xFA05 0000 + 0111
*/
#define NVIC_PREEMPTION_16_SUBGROUP_NONE        (0xFA050300UL)
#define NVIC_PREEMPTION_8_SUBGROUP_TWO          (0xFA050400UL)
#define NVIC_PREEMPTION_4_SUBGROUP_FOUR         (0xFA050500UL)
#define NVIC_PREEMPTION_2_SUBGROUP_EIGHT        (0xFA050600UL)
#define NVIC_PREEMPTION_NONE_SUBGROUP_SIXTEEN   (0xFA050700UL)

/********************************************************************************************************/
/************************************************Types***************************************************/
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
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
  * @brief  Enables a specific IRQ.
  * @param  IRQ: The IRQ number to be enabled.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_EnableIRQ(uint8_t IRQ);
/**
  * @brief  Disables a specific IRQ.
  * @param  IRQ: The IRQ number to be disabled.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_DisableIRQ(uint8_t IRQ);
/**
  * @brief  Sets a pending state for a specific IRQ.
  * @param  IRQ: The IRQ number to set its pending state.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_SetPendingIRQ(uint8_t IRQ);
/**
  * @brief  Clears a pending state for a specific IRQ.
  * @param  IRQ: The IRQ number to clear its pending state.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_ClearPendingIRQ(uint8_t IRQ);
/**
  * @brief  Gets the pending state of a specific IRQ.
  * @param  IRQ: The IRQ number to get its pending state.
  * @param  PendingState: Pointer to where the pending state will be stored.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_GetPendingIRQ(uint8_t IRQ, uint8_t *PendingState);
/**
  * @brief  Gets the active state of a specific IRQ.
  * @param  IRQ: The IRQ number to get its active state.
  * @param  ActiveState: Pointer to where the active state will be stored.
  * @retval ErrorStatus_t: Indicates the status of the function.
  */
ErrorStatus_t NVIC_GetActiveState(uint8_t IRQ, uint8_t *ActiveState);
/**
  * @brief  Sets the priority for a given interrupt request (IRQ).
  * @param  IRQ: The IRQ number for which the priority is to be set.
  * @param  PriorityGrouping: The priority grouping configuration value.
  *              This value determines the number of bits used for preemption and subgroup priority.
  *              It should be one of the values:
  *              - NVIC_PREEMPTION_16_SUBGROUP_NONE
  *              - NVIC_PREEMPTION_8_SUBGROUP_TWO
  *              - NVIC_PREEMPTION_4_SUBGROUP_FOUR
  *              - NVIC_PREEMPTION_2_SUBGROUP_EIGHT
  *              - NVIC_PREEMPTION_NONE_SUBGROUP_SIXTEEN
  * 
  * @param  Preemption: The preemption priority value for the IRQ.
  *              This value specifies the priority of the IRQ within its preemption group.
  *              The range of valid values is determined by the number of bits allocated(By PriorityGrouping) for preemption priority.
  * @param  SubGroup: The subgroup priority value for the IRQ.
  *              This value specifies the priority of the IRQ within its subgroup.
  *              The range of valid values is determined by the number of bits (By PriorityGrouping) allocated for subgroup priority.
  * @retval ErrorStatus_t: Indicates the status of the function execution.
  *              - NVIC_OK: The operation was successful.
  *              - NVIC_WRONG_PARAMETER: One or more parameters are invalid.
  */
ErrorStatus_t NVIC_SetPriority(uint8_t IRQ, uint32_t PriorityGrouping, uint8_t Preemption, uint8_t SubGroup)

//ErrorStatus_t NVIC_GetPriority(uint8_t IRQ,uint32_t Priority);




























#endif // MCAL_NVIC_NVIC_H_