#ifndef MCAL_NVIC_NVIC_VALIDATION_H_
#define MCAL_NVIC_NVIC_VALIDATION_H_


/*------------------------------------ Validation Function like Macros ---------------------------------------------*/
#define IS_PRIORITY_GROUPING_VALID(PRIGROUP)     ((PRIGROUP)==(NVIC_PREEMPTION_16_SUBGROUP_NONE)      ||\
                                                  (PRIGROUP)==(NVIC_PREEMPTION_8_SUBGROUP_TWO)        ||\
                                                  (PRIGROUP)==(NVIC_PREEMPTION_4_SUBGROUP_FOUR)       ||\
                                                  (PRIGROUP)==(NVIC_PREEMPTION_2_SUBGROUP_EIGHT)      ||\
                                                  (PRIGROUP)==(NVIC_PREEMPTION_NONE_SUBGROUP_SIXTEEN))





#endif // MCAL_NVIC_NVIC_VALIDATION_H_