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
 * This values is combination of KeyValue of AIRCR Register and value of pr4ioirty grouping 
 *  0xFA05 0000 + ((00xx) in Bit8:Bit10 = 300)
 *  0xFA05 0000 + ((0100) in Bit8:Bit10 = 400)
 *  0xFA05 0000 + ((0101) in Bit8:Bit10 = 500)
 *  0xFA05 0000 + ((0110) in Bit8:Bit10 = 600)
 *  0xFA05 0000 + ((0111) in Bit8:Bit10 = 700)
*/
#define NVIC_PREEMPTION_16_SUBGROUP_NONE        (0x05FA0300UL)
#define NVIC_PREEMPTION_8_SUBGROUP_TWO          (0x05FA0400UL)
#define NVIC_PREEMPTION_4_SUBGROUP_FOUR         (0x05FA0500UL)
#define NVIC_PREEMPTION_2_SUBGROUP_EIGHT        (0x05FA0600UL)
#define NVIC_PREEMPTION_NONE_SUBGROUP_SIXTEEN   (0x05FA0700UL)

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/


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
ErrorStatus_t NVIC_SetPriority(uint8_t IRQ, uint32_t PriorityGrouping, uint8_t Preemption, uint8_t SubGroup);

//ErrorStatus_t NVIC_GetPriority(uint8_t IRQ,uint32_t Priority);




























#endif // MCAL_NVIC_NVIC_H_