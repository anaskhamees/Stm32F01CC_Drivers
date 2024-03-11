#ifndef MCAL_SYSTICK_SYSTICK_CFG_H_
#define MCAL_SYSTICK_SYSTICK_CFG_H_

#include "SysTick.h"
/**
 * If you want work with AHB CLK select :SYSTICK_ENABLE_INT_AHB
 * else, you want to work with prescalar 8 select : SYSTICK_ENABLE_INT_AHB_8
*/
#define SYSTICK_CLOCK_SOURCE        SYSTICK_ENABLE_INT_AHB


#endif // MCAL_SYSTICK_SYSTICK_CFG_H_