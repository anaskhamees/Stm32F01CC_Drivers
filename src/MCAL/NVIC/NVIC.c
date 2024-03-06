/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "NVIC.h"
#include "STM32F401xx.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define NVIC_PREPHERAL_BASE_ADDRESS     (0xE000E100UL)
#define SCB_PREPHERAL_BASE_ADDRESS      (0xE000ED00UL)

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile NVIC_Reg_t*const NVIC=(volatile NVIC_Reg_t*)(NVIC_PREPHERAL_BASE_ADDRESS);
volatile SCB_Reg_t*const SCB=(volatile SCB_Reg_t*)(SCB_PREPHERAL_BASE_ADDRESS);
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
ErrorStatus_t NVIC_EnableIRQ(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if(IRQ>MAX_IRQ_VALUE)
    {
        Return_State=NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum=IRQ/32;
        uint8_t BitNum=IRQ%32;
        NVIC->ISER[RegisterNum]|=((1UL)<<BitNum);
        Return_State=NVIC_OK;
    }
    return Return_State;
}

ErrorStatus_t NVIC_DisableIRQ(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if(IRQ>MAX_IRQ_VALUE)
    {
        Return_State=NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum=IRQ/32;
        uint8_t BitNum=IRQ%32;
        NVIC->ICER[RegisterNum]=((1UL)<<BitNum);
        Return_State=NVIC_OK;
    }
    return Return_State;
}
ErrorStatus_t NVIC_SetPendingIRQ(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if(IRQ>MAX_IRQ_VALUE)
    {
        Return_State=NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum=IRQ/32;
        uint8_t BitNum=IRQ%32;
        NVIC->ISPR[RegisterNum]|=((1UL)<<BitNum);
        Return_State=NVIC_OK;
    }
    return Return_State;
}
ErrorStatus_t NVIC_ClearPendingIRQ(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if(IRQ>MAX_IRQ_VALUE)
    {
        Return_State=NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum=IRQ/32;
        uint8_t BitNum=IRQ%32;
        NVIC->ICPR[RegisterNum]|=((1UL)<<BitNum);
        Return_State=NVIC_OK;

    }
    return Return_State;
}
ErrorStatus_t NVIC_GetPendingIRQ(uint8_t IRQ,uint8_t* PendingState)
{
     ErrorStatus_t Return_State;
    if(IRQ>MAX_IRQ_VALUE)
    {
        Return_State=NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum=IRQ/32;
        uint8_t BitNum=IRQ%32;
        *PendingState=(((NVIC->ISPR[RegisterNum])>>BitNum)&(1UL));
        Return_State=NVIC_OK;
    }
    return Return_State;
}
ErrorStatus_t NVIC_GetActiveState(uint8_t IRQ,uint8_t* ActiveState)
{
    ErrorStatus_t Return_State;
    if(IRQ>MAX_IRQ_VALUE)
    {
        Return_State=NVIC_WRONG_PARAMETER;
    }
    else
    {
        uint8_t RegisterNum=IRQ/32;
        uint8_t BitNum=IRQ%32;
        *ActiveState=(((NVIC->IABR[RegisterNum])>>BitNum)&(1UL));
        Return_State=NVIC_OK;
    }
    return Return_State;
}
ErrorStatus_t NVIC_GenerateSoftwareInterrupt(uint8_t IRQ)
{
    ErrorStatus_t Return_State;
    if(IRQ>MAX_IRQ_VALUE)
    {
        Return_State=NVIC_WRONG_PARAMETER;
    }
    else
    {
        NVIC->STIR=IRQ;
        Return_State=NVIC_OK;
    }
    return Return_State;
}
ErrorStatus_t NVIC_SetPriority(uint8_t IRQ,uint32_t Priority)
{
    /* Test*/
}