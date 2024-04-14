/*
* File:    SYSTICK.h
* Author:  Yasmin Amr
* Created: 9/3/2024
* Brief:   SYSTICK Driver
* Target:  STM32F401cc
*/

#ifndef SYSTICK_H_
#define SYSTICK_H_

/**************************************************************************/
/*						Includes                			 			  */
/**************************************************************************/
#include "Std_Lib/Std_Libraries.h"
#include "SYSTICK_private.h"
#include "SYSTICK_cfg.h"

/**************************************************************************/
/*						#Defines                   			 			  */
/**************************************************************************/

/* Peiodicity Options */
#define STK_PERIODICITY_ONE_TIME    (0U)
#define STK_PERIODICITY_INFINITE    (1U)

/**************************************************************************/
/*						SYSTICK Error Status Enum   	 			 	  */
/**************************************************************************/
typedef enum
{
    STK_Ok,
    STK_Nok
}STK_ErrorStatus_t;

/*SysTick Call-Back Function Type*/
typedef void (*stkcbf_t) (void); 

/**
 * @brief   Function to Start SysTick Timer
 * 
 * @param   Periodicity:   
 *                 - STK_PERIODICITY_ONE_TIME
 *                 - STK_PERIODICITY_INFINITE
 * 
 * @return  Error Status (If Periodicity Option is Invalid)
 */
STK_ErrorStatus_t STK_Start(uint8_t Copy_Periodicity);

/**
 * @brief   Function to Stop SysTick Timer
 */
void STK_Stop(void);

/**
 * @brief   Function to Set Time in MilliSeconds in SysTick Timer
 *
 * @param   - Time in MS
 *
 * @return  Error Status (Checks if counts exceeds max counts in 'STK_MAX_COUNT_MS' variable)
 */
STK_ErrorStatus_t STK_SetTimeMS(uint32_t Time);

/**
 * @brief   Function to Set CallBack Function from Application Layer
 *
 * @param   - cbf (Function Name)
 * 
 * @constr  - cbf must be a funcation that takes void and returns void
 *
 * @return  Error Status (Checks if cbf is Null)
 */
STK_ErrorStatus_t STK_SetCallBack(stkcbf_t cbf);

/**
 * @brief   SysTick Handler Function Implementation
 */
void SysTick_Handler(void);

#endif
