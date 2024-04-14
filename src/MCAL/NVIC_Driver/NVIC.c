#include "NVIC.h"

/**
 * Note 1: For Handler Debugging: Add a breakpoint inside the Handler & use Step Over.
 * 
 * Note 2: Use "SetPriority" Function to Set Priorities as all ISRs have the same SW Priority after Reset
*/

/**************************************************************************/
/*						NVIC-SCB Implementation Macros 				 	  */
/**************************************************************************/
#define SCB_PRIGROUP_MASK       0x00000700
#define EIGHT_BIT_MASK          0x000000FF
#define SCB_VECTKEY_MASK        0xFFFF0000
#define SCR_AIRCR_WRITE         0x05FA0000
#define FOUR_BIT_SHIFT          0x00000004
#define BYTE                    (8U)

/**************************************************************************/
/*						NVIC-SCB BASE ADDRESSES     				 	  */
/**************************************************************************/
#define NVIC_BASE_ADDRESS       0xE000E100
#define SCB_BASE_ADDRESS        0xE000ED00

/**************************************************************************/
/*						NVIC Registers Struct	 					 	  */
/**************************************************************************/
typedef struct 
{
    volatile uint32_t ISER[8];
    volatile uint32_t Reserved[24];
    volatile uint32_t ICER[8];
    volatile uint32_t Reserved1[24];
    volatile uint32_t ISPR[8];
    volatile uint32_t Reserved2[24];
    volatile uint32_t ICPR[8];
    volatile uint32_t Reserved3[24];
    volatile uint32_t IABR[8];
    volatile uint32_t Reserved4[56];
    volatile uint32_t IPR[60];
    volatile uint32_t Reserved5[644];
    volatile uint32_t STIR;
}NVIC_Peri_t;

/**************************************************************************/
/*						SCB Registers Struct	 					 	  */
/**************************************************************************/
typedef struct
{
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint32_t SHPR[3];
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t HFSR;
    volatile uint32_t Reserved2;
    volatile uint32_t MMAR;
    volatile uint32_t BFAR;
    volatile uint32_t AFSR;
}SCB_Peri_t;

/**************************************************************************/
/*						NVIC-SCB Pointer to Registers  				 	  */
/**************************************************************************/
volatile NVIC_Peri_t* const NVIC=( volatile NVIC_Peri_t*)NVIC_BASE_ADDRESS;
volatile SCB_Peri_t* const SCB=( volatile SCB_Peri_t*)SCB_BASE_ADDRESS;

/**
 * @brief  		 Function to get Two power input number
 *
 * @param   	 num
 *
 * @return		 Result: Two power num
 */
uint8_t two_power (uint8_t num)
{
    uint8_t result=1;
    for (uint8_t i=0;i<num;i++)
    {
        result*=2;
    }
    return result;
}

/**
 * @brief  		 Function to Enable Any Interrupt in the System
 *
 * @param   	 IRQn(Interrupt Number)
 *               - MACRO Begins with NVIC_
 *               - Options listed in STM32F401xx.h
 *
 * @return		 Error Status
 *         		 - Returns Error if Interrupt Number is out of range 
 * 
 * @constraint   Must Disable Pending Bit before Enabling to avoid Handling Interrupts from Disabled Time!
 */
NVIC_ErrorStatus_t NVIC_EnableInterrupt(uint8_t IRQn)
{
    NVIC_ErrorStatus_t RET_ErrorStatus= NVIC_Ok;
    uint8_t Reg_Num;
    uint8_t Bit_Num;
    if(IRQn>=IRQn_MAX)
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else
    {
        Reg_Num=IRQn/32;
        Bit_Num=IRQn%32;
        NVIC->ISER[Reg_Num]|=(1<<Bit_Num);
    }
    return RET_ErrorStatus;
}

/**
 * @brief  		 Function to Disable Any Interrupt in the System
 *
 * @param   	 IRQn(Interrupt Number)
 *               - MACRO Begins with NVIC_
 *               - Options listed in STM32F401xx.h
 *
 * @return		  Error Status
 *         		 - Returns Error if Interrupt Number is out of range 
 */
NVIC_ErrorStatus_t NVIC_DisableInterrupt(uint8_t IRQn)
{
    NVIC_ErrorStatus_t RET_ErrorStatus= NVIC_Ok;
    uint8_t Reg_Num;
    uint8_t Bit_Num;
    if(IRQn>=IRQn_MAX)
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else
    {
        Reg_Num=IRQn/32;
        Bit_Num=IRQn%32;
        NVIC->ICER[Reg_Num]=(1<<Bit_Num);
    }
    return RET_ErrorStatus;  
}

/**
 * @brief  		 Function to Set Pending Bit for Any Interrupt in the System
 *
 * @param   	 IRQn(Interrupt Number)
 *               - MACRO Begins with NVIC_
 *               - Options listed in STM32F401xx.h
 *
 * @return		  Error Status
 *         		 - Returns Error if Interrupt Number is out of range 
 */
NVIC_ErrorStatus_t NVIC_SetPending(uint8_t IRQn)
{
    NVIC_ErrorStatus_t RET_ErrorStatus= NVIC_Ok;
    uint8_t Reg_Num;
    uint8_t Bit_Num;
    if(IRQn>=IRQn_MAX)
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else
    {
        Reg_Num=IRQn/32;
        Bit_Num=IRQn%32;
        NVIC->ISPR[Reg_Num]|=(1<<Bit_Num);
    }
    return RET_ErrorStatus;
}

/**
 * @brief  		 Function to Clear Pending Bit for Any Interrupt in the System
 *
 * @param   	 IRQn(Interrupt Number)
 *               - MACRO Begins with NVIC_
 *               - Options listed in STM32F401xx.h
 *
 * @return		  Error Status
 *         		 - Returns Error if Interrupt Number is out of range 
 */
NVIC_ErrorStatus_t NVIC_ClearPending(uint8_t IRQn)
{
    NVIC_ErrorStatus_t RET_ErrorStatus= NVIC_Ok;
    uint8_t Reg_Num;
    uint8_t Bit_Num;
    if(IRQn>=IRQn_MAX)
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else
    {
        Reg_Num=IRQn/32;
        Bit_Num=IRQn%32;
        NVIC->ICPR[Reg_Num]=(1<<Bit_Num);
    }
    return RET_ErrorStatus;
}

/**
 * @brief  		 Function to Check if Any Interrupt in the System is Active/Not Active
 *
 * @param   	 1) IRQn(Interrupt Number)
 *                  - MACRO Begins with NVIC_
 *                  - Options listed in STM32F401xx.h
 * 
 *               2) Pointer to ActiveStatus Variable
 *                  - Returns (INTERRUPT_NOT_ACTIVE/INTERRUPT_ACTIVE) through pointer
 * 
 * @return		 Error Status
 *         		 - Returns Error if:
 *                  * Interrupt Number is out of range 
 *                  * Pointer is NULL
 */
NVIC_ErrorStatus_t NVIC_GetActiveStatus(uint8_t IRQn, uint8_t*ActiveStatus)
{
    NVIC_ErrorStatus_t RET_ErrorStatus= NVIC_Ok;
    uint8_t Reg_Num;
    uint8_t Bit_Num;
    if(IRQn>=IRQn_MAX)
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else
    {
        Reg_Num=IRQn/32;
        Bit_Num=IRQn%32;
        *ActiveStatus=((NVIC->IABR[Reg_Num]>>Bit_Num)&0x01);
    }
    return RET_ErrorStatus;
}

/**
 * @brief  		 Function to Generate Software Interrupt for Any Interrupt in the System
 *
 * @param   	 ID(Interrupt Number)
 *               - MACRO Begins with NVIC_
 *               - Options listed in STM32F401xx.h
 *
 * @return		  Error Status
 *         		 - Returns Error if Interrupt Number is out of range 
 */
NVIC_ErrorStatus_t NVIC_GenerateSoftwareInterrupt(uint8_t ID)
{
    NVIC_ErrorStatus_t RET_ErrorStatus= NVIC_Ok;
    if(ID>=IRQn_MAX)
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else
    {
        NVIC->STIR=ID;
    }
    return RET_ErrorStatus;
}


/**
 * @brief  		 Function to Set Priority Bit for Any Interrupt in the System
 *
 * @param   	 1) IRQn(Interrupt Number)
 *                  - MACRO Begins with NVIC_
 *                  - Options listed in STM32F401xx.h
 *              
 *               2) PreemptLevel
 *                  - Takes preemtion level, number must less than 2^(preemption bits)
 *                  - Preemtion bits is (Priority bits - Subgroup Bits)
 * 
 *               3) SubGroupLevel
 *                  - Takes Subgroup level, number must less than 2^(subgroup bits)
 * 
 *               4) SubGroupBitsMask
 *                  - Takes Number of Subgroup bits configuration, choose options from:
 *                          * SUBPRIORITY_BITS_NONE
 *                          * SUBPRIORITY_BITS_ONE
 *                          * SUBPRIORITY_BITS_TWO
 *                          * SUBPRIORITY_BITS_THREE
 *                          * SUBPRIORITY_BITS_FOUR
 *
 * @return		  Error Status
 *         		 - Returns Error if:
 *                          * Interrupt Number is out of range 
 *                          * Preemtion bits >= 2^(preemption bits)
 *                          * SubGroupLevel >= 2^(subgroup bits)
 *                          * Subgroup Bits + Preemption Bits > PRIORITY_BITS
 */
NVIC_ErrorStatus_t NVIC_SetPriority(uint8_t IRQn, uint8_t PreemptLevel, uint8_t SubGroupLevel, uint32_t SubGroupBitsMask)
{
    NVIC_ErrorStatus_t RET_ErrorStatus= NVIC_Ok;

    /*Extract Subgroup Bits from Subgroup Mask*/
    uint8_t SubGroupBitsNum=(uint8_t)(((SubGroupBitsMask>>8)&0x07)-0x03);

    /*Extract Preemtion Bits from Data*/
    uint8_t PreemptionBitsNum=PRIORITY_BITS-SubGroupBitsNum;

    uint8_t MaxPreemptLevel=(two_power(PreemptionBitsNum)-1);
    uint8_t MaxSubGroupLevel=(two_power(SubGroupBitsNum)-1);

    if(IRQn>=IRQn_MAX)
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else if(SubGroupBitsNum+PreemptionBitsNum>PRIORITY_BITS)
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else if(SubGroupLevel>MaxSubGroupLevel)
    {
       RET_ErrorStatus= NVIC_NOk;
    }
    else if(PreemptLevel> MaxPreemptLevel )
    {
        RET_ErrorStatus= NVIC_NOk;
    }
    else
    {
        /**************************************************/
        /*	    SubGroup Configuration - SCB Peripheral   */
        /**************************************************/
        uint32_t Local_AIRCR=SCB->AIRCR;
        /*Apply Write Mask to Register*/
        Local_AIRCR&=~SCB_VECTKEY_MASK;
        /*Apply Priority Mask to Register*/
        Local_AIRCR&=~SCB_PRIGROUP_MASK;
        /*Apply Write Value + Priority Value to Register*/
        Local_AIRCR|=(SCR_AIRCR_WRITE | SubGroupBitsMask);
        SCB->AIRCR=Local_AIRCR;

        /***************************************************/
        /*	    Priority Configuration - NVIC Peripheral   */
        /***************************************************/
        uint8_t Priority_Reg_Num= IRQn/4;
        uint8_t Priority_Byte_Offset= IRQn%4;

        uint32_t Local_IPR=NVIC->IPR[Priority_Reg_Num];
        Local_IPR&=~(EIGHT_BIT_MASK<<Priority_Byte_Offset);
        Local_IPR|=((((PreemptLevel<<SubGroupBitsNum)|(SubGroupLevel))<<FOUR_BIT_SHIFT)<<(Priority_Byte_Offset*BYTE));
        NVIC->IPR[Priority_Reg_Num]|=Local_IPR;

    }
    return RET_ErrorStatus;
    
}

