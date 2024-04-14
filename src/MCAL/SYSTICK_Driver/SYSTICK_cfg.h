/*
* File:    SYSTICK_cfg.h
* Author:  Yasmin Amr
* Created: 9/3/2024
* Brief:   SYSTICK Driver
* Target:  STM32F401cc
*/

#ifndef SYSTICK_CFG_H_
#define SYSTICK_CFG_H_

/*Enter your AHB Frequency*/
#define STK_AHB_FREQUENCY     16000000

/*Choose SysTick Frequency from
 *      - STK_AHB_CLOCK
 *      - STK_AHB_CLOCK_DIV_8
 */
#define STK_CLOCK_CHOICE      STK_AHB_CLOCK

#endif