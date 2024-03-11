#ifndef MCAL_SYSTICK_SYSTICK_VALIDATION_H_
#define MCAL_SYSTICK_SYSTICK_VALIDATION_H_


#define SYSTICK_ENABLE_INT_AHB_8   (0x00000003UL) /* Enable: ON, INT: ON, CLK: AHB/8 */
#define SYSTICK_ENABLE_INT_AHB     (0x00000007UL) /* Enable: ON, INT: ON, CLK: AHB   */
#define SYSTICK_DISABLE            (0x00000000UL)

#define IS_VALID_SYSTICK_CFG(CFG)      ((mode==SYSTICK_ENABLE_INT_AHB_8)||\
                                         (mode==SYSTICK_ENABLE_INT_AHB)  ||\
                                         (mode==SYSTICK_DISABLE)         ||\
                                        )



#endif // MCAL_SYSTICK_SYSTICK_VALIDATION_H_