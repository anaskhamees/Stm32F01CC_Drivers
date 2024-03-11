/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "SysTick_CFG.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define SYSTICK_CTR_CLEAR_MASK     (0x00000007UL)
#define SYSTICK_MAX_RELOAD         (0x00FFFFFFUL)
#define EXPIRED                    (1UL)
#define READ_1BIT                  (1UL)
#define MASK_SHIFT_COUNT_FLAG      (15UL)

 
/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile SysTick_Registers_t*const SYSTICK=(volatile SysTick_Registers_t*const)(SYSTICK_BASE_ADDRESS);
static uint32_t SysTickMode;

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
ErrorStatus_t SYSTICK_Start(uint32_t CFG,uint32_t Mode)
{
    ErrorStatus_t ReturnError;
    if(!IS_VALID_SYSTICK_CFG(CFG))
    {
        ReturnError=SYSTICK_WRONG_PARAMETER;
    }
    else if(Mode >SYSTICK_MODE_ONE_TIME)
    {
        ReturnError=SYSTICK_WRONG_PARAMETER;
    }
    else
    {
        SysTickMode=Mode;
        uint32_t Loc_STK_CTRL=SYSTICK->STK_CTRL;
        Loc_STK_CTRL&=~SYSTICK_CTR_CLEAR_MASK;
        Loc_STK_CTRL|=CFG;
        SYSTICK->STK_CTRL=Loc_STK_CTRL;
        ReturnError=SYSTICK_OK;
    }
    return ReturnError;
}
void SYSTICK_Stop(void)
{   
    SYSTICK->STK_CTRL=SYSTICK_DISABLE;
}
ErrorStatus_t SYSTICK_SetTimeMs(uint32_t TimeMS)
{
    ErrorStatus_t ReturnError;
    
    uint32_t CPU_Freq=(SYSTICK_CLOCK_SOURCE==SYSTICK_ENABLE_INT_AHB)?(SYSTICK_AHB_CLK_SOURCE):(SYSTICK_AHB_CLK_SOURCE/(uint32_t)8);
    uint32_t ReloadValue=((TimeMS/(uint32_t)1000)*CPU_Freq)-(uint32_t)1;
    if(ReloadValue>SYSTICK_MAX_RELOAD)   
    {
        ReturnError=SYSTICK_WRONG_PARAMETER;
    }
    else
    {
        SYSTICK->STK_LOAD=ReloadValue;
    }
    return ReturnError;
}
ErrorStatus_t SYSTICK_IsExpired(uint8_t* ExpiredFlag)
{
    uint32_t CountFlag=((SYSTICK->STK_CTRL)>>MASK_SHIFT_COUNT_FLAG)&READ_1BIT);
    if(CountFlag==EXPIRED)
    {
        *ExpiredFlag=EXPIRED;
    }
}


   