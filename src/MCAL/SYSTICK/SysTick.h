/*
 * File Name: SysTick.h
 * Layer	: MCAL
 * Module	: System Tick
 * Version  : V 0.0
 * Created	:  Mar 12, 2024
 * Author	: Anas Khamees
 */
#ifndef MCAL_SYSTICK_SYSTICK_H_
#define MCAL_SYSTICK_SYSTICK_H_
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Std_Lib/Std_Libraries.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define SYSTICK_BASE_ADDRESS            (0xE000E010UL)

#define SYSTICK_ENABLE_INT_AHB_8        (0x00000003UL) /* Enable: ON, INT: ON, CLK: AHB/8 */
#define SYSTICK_ENABLE_INT_AHB          (0x00000007UL) /* Enable: ON, INT: ON, CLK: AHB   */
#define SYSTICK_DISABLE                 (0x00000000UL)

#define SYSTICK_MODE_PERIODIC           (0UL)
#define SYSTICK_MODE_ONE_TIME           (1UL)
#define SYSTICK_AHB_CLK_SOURCE          (16000000UL)

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void (*SysTick_CallBack_t)(void);
/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
 * @brief  Starts the SysTick timer with the specified configuration and mode.
 * @param  CFG: 
 *            - SYSTICK_ENABLE_INT_AHB_8: Set the SysTick clock source to be the AHB clock divided by 8 (Interrupt Enabled).
 *            - SYSTICK_ENABLE_INT_AHB  : Set the SysTick clock source to be the AHB clock  (Interrupt Enabled).
 * @param  Mode: Mode of operation for the SysTick timer:
 *            - SYSTICK_MODE_ONE_TIME   : The SysTick timer operates in single-shot mode, triggering an interrupt after counting down once.
 *            - SYSTICK_MODE_PERIODIC   : The SysTick timer operates in periodic mode, counting down and triggering interrupts and repeat.
 * @note   It also clears the current count value of the SysTick timer at first.
 * @retval ErrorStatus_t:  Returns SYSTICK_OK if the operation is successful, or an error code if there is a parameter validation failure.
 */
ErrorStatus_t SYSTICK_Start(uint32_t CFG, uint32_t Mode);
/**
 * @brief  Stops the SysTick timer.
 * @note   It clears any active SysTick interrupts and stops the timer from counting down.
 * @retval void
 */
void SYSTICK_Stop(void);
/**
 * @brief  Sets the reload value of the SysTick timer to achieve the desired time delay in milliseconds.
 * @param  TimeMS: The desired time delay in milliseconds.
 * @note   This function calculates the appropriate reload value based on the desired time delay and the CPU frequency.
 * @retval ErrorStatus_t: SYSTICK_OK if the operation is successful, 
 *                        SYSTICK_WRONG_PARAMETER if the provided time delay exceeds the maximum allowable value.
 */
ErrorStatus_t SYSTICK_SetTimeMs(uint32_t TimeMS);
/**
 * @brief  Checks if the SysTick timer has expired (Counter Reaches to Zero or Not).
 * @param  ExpiredFlag: Pointer to a variable where the expiration status will be stored.
 * @note   This function reads the COUNTFLAG bit of the SysTick control register to determine if the timer has expired.
 * @note   The expiration status is stored in the ExpiredFlag parameter, which will be set to 1 if the timer has expired, and 0 otherwise.
 * @retval ErrorStatus_t: SYSTICK_OK if the operation is successful.
 */
ErrorStatus_t SYSTICK_IsExpired(uint8_t* ExpiredFlag);
/**
 * @brief  Sets a callback function to be called when the SysTick timer expires.
 * @param  SysTickCB: Pointer to the callback function to be set.
 * @note   The callback function should have the following signature: void (*SysTick_CallBack_t)(void).
 * @retval ErrorStatus_t: SYSTICK_OK if the operation is successful, SYSTICK_NULL_POINTER if SysTickCB is NULL.
 */
ErrorStatus_t SYSTICK_SetCallBack(SysTick_CallBack_t SysTickCB);


#endif // MCAL_SYSTICK_SYSTICK_H_