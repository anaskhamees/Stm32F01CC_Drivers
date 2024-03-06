#ifndef MCAL_NVIC_NVIC_H_
#define MCAL_NVIC_NVIC_H_


/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Std_Lib/Std_Libraries.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



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

}SCB_Reg_t

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

ErrorStatus_t NVIC_EnableIRQ(uint8_t IRQ);
ErrorStatus_t NVIC_DisableIRQ(uint8_t IRQ);
ErrorStatus_t NVIC_SetPendingIRQ(uint8_t IRQ);
ErrorStatus_t NVIC_ClearPendingIRQ(uint8_t IRQ);
ErrorStatus_t NVIC_GetPendingIRQ(uint8_t IRQ);
ErrorStatus_t NVIC_SetPriority(uint8_t IRQ,uint32_t Priority);
ErrorStatus_t NVIC_GetPriority(uint8_t IRQ,uint32_t Priority);




























#endif // MCAL_NVIC_NVIC_H_