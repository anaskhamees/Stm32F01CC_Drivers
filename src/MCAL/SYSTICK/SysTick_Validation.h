/*
 * File Name: SysTick_Validation.h
 * Layer	: MCAL
 * Module	: System Tick
 * Version  : V 0.0
 * Created	:  Mar 12, 2024
 * Author	: Anas Khamees
 */
#ifndef MCAL_SYSTICK_SYSTICK_VALIDATION_H_
#define MCAL_SYSTICK_SYSTICK_VALIDATION_H_

#define SYSTICK_ENABLE_INT_AHB_8   (0x00000003UL) /* Enable: ON, INT: ON, CLK: AHB/8 */
#define SYSTICK_ENABLE_INT_AHB     (0x00000007UL) /* Enable: ON, INT: ON, CLK: AHB   */
#define SYSTICK_DISABLE            (0x00000000UL)

#define IS_VALID_SYSTICK_CFG(CFG)       ((CFG==SYSTICK_ENABLE_INT_AHB_8)||\
                                         (CFG==SYSTICK_ENABLE_INT_AHB)  ||\
                                         (CFG==SYSTICK_DISABLE))



#endif // MCAL_SYSTICK_SYSTICK_VALIDATION_H_