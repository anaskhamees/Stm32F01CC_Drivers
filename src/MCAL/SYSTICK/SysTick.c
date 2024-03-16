/*
 * File Name: SysTick.c
 * Layer	: MCAL
 * Module	: System Tick
 * Version  : V 0.0
 * Created	:  Mar 12, 2024
 * Author	: Anas Khamees
 */

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "SysTick_CFG.h"
#include "SysTick_Validation.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define SYSTICK_CTR_CLEAR_MASK     (0x00000007UL)
#define SYSTICK_MAX_RELOAD         (0x00FFFFFFUL)
#define EXPIRED                    (1UL)
#define READ_1BIT                  (1UL)
#define MASK_SHIFT_COUNT_FLAG      (15UL)
#define MASK_READ_VAL              (0xFFFFFFFFUL)

/********************************************************************************************************/
/************************************************  Types  ***********************************************/
/********************************************************************************************************/ 
 typedef struct 
{
    uint32_t STK_CTRL ;
    uint32_t STK_LOAD ;
    uint32_t STK_VAL  ;
    uint32_t STK_CALIB;
}SysTick_Registers_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile SysTick_Registers_t*const SYSTICK=(volatile SysTick_Registers_t*const)(SYSTICK_BASE_ADDRESS);
static uint32_t SysTickMode;
static SysTick_CallBack_t AppCallBack=NULL;
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
        SYSTICK->STK_VAL&=~MASK_READ_VAL;
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
    uint32_t ReloadValue=(((uint64_t)TimeMS*(uint64_t)CPU_Freq)/(uint64_t)1000)-(uint32_t)1;
    if(ReloadValue>SYSTICK_MAX_RELOAD)   
    {
        ReturnError=SYSTICK_WRONG_PARAMETER;
    }
    else
    {
        SYSTICK->STK_LOAD=ReloadValue;
        ReturnError=SYSTICK_OK;
    }
    return ReturnError;
}
ErrorStatus_t SYSTICK_IsExpired(uint8_t* ExpiredFlag)
{
    ErrorStatus_t ReturnError=SYSTICK_NOK;
    uint32_t CountFlag=(((SYSTICK->STK_CTRL)>>MASK_SHIFT_COUNT_FLAG)&READ_1BIT);
    if(CountFlag==EXPIRED)
    {
        *ExpiredFlag=EXPIRED;
        ReturnError=SYSTICK_OK;
    }
    return ReturnError;
}

ErrorStatus_t SYSTICK_SetCallBack(SysTick_CallBack_t SysTickCB)
{
    ErrorStatus_t ReturnError;
    if(SysTickCB)
    {
        AppCallBack=SysTickCB;
        ReturnError=SYSTICK_OK;
    }
    else
    {
        ReturnError=SYSTICK_NULL_POINTER;
    }
    return ReturnError;
}

void SysTick_Handler(void)
{
    static uint64_t x =0;
    if(AppCallBack)
    {
        AppCallBack();
        x++;
    }
    if(SysTickMode==SYSTICK_MODE_ONE_TIME)
    {
        SYSTICK_Stop();    
    }
}


   