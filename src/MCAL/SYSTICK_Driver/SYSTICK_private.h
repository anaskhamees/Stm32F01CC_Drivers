/*
* File:    SYSTICK_private.h
* Author:  Yasmin Amr
* Created: 9/3/2024
* Brief:   SYSTICK Driver
* Target:  STM32F401cc
*/

#ifndef SYSTICK_PRIVATE_H_
#define SYSTICK_PRIVATE_H_

#define STK_AHB_CLOCK           0x00000004
#define STK_AHB_CLOCK_DIV_8     0x00000000 

#define STK_DONT_ACTIVATE_HANDLER   (0U)
#define STK_ACTIVATE_HANDLER        (1U)

#endif