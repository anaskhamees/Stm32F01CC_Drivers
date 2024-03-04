
#ifndef MCAL_GPIO_GPIO_VALIDATION_H_
#define MCAL_GPIO_GPIO_VALIDATION_H_


/******************************************************************************************/
/******************************** [Functions Like Macros] *********************************/
/******************************************************************************************/

/**
 * @brief :
 * This macro Function is used to check if a given GPIO mode (mode) is valid.
 * It returns 1 if the mode matches one of the specified GPIO mode constants, 
 * and 0 otherwise. 
*/

#define IS_VALID_GPIO_MODE(mode)       (((mode) == GPIO_OUT_PP_NO_PUPD) || \
                                        ((mode) == GPIO_OUT_PP_PU)      || \
                                        ((mode) == GPIO_OUT_PP_PD)      || \
                                        ((mode) == GPIO_OUT_OD_NO_PUPD) || \
                                        ((mode) == GPIO_OUT_OD_PU)      || \
                                        ((mode) == GPIO_OUT_OD_PD)      || \
                                        ((mode) == GPIO_IN_FLOATING)    || \
                                        ((mode) == GPIO_IN_PU)          || \
                                        ((mode) == GPIO_IN_PD)          || \
                                        ((mode) == GPIO_IN_ANALOG)      || \
                                        ((mode) == GPIO_AF_PP_NO_PUPD)  || \
                                        ((mode) == GPIO_AF_PP_PU)       || \
                                        ((mode) == GPIO_AF_PP_PD)       || \
                                        ((mode) == GPIO_AF_OD_NO_PUPD)  || \
                                        ((mode) == GPIO_AF_OD_PU)       || \
                                        ((mode) == GPIO_AF_OD_PD))

/**
 *@brief:
 * This macro is used to check if a given GPIO port (port) is valid. 
 * It returns 1 if the port matches one of the specified GPIO port constants, and 0 otherwise
 */
#define IS_VALID_GPIO_PORT(port)   (((port) == GPIO_PORTA) || \
                                    ((port) == GPIO_PORTB) || \
                                    ((port) == GPIO_PORTC) || \
                                    ((port) == GPIO_PORTD) || \
                                    ((port) == GPIO_PORTE) || \
                                    ((port) == GPIO_PORTH))
 /**
  *@brief :
  *This macro is used to check if a given GPIO speed (speed) is valid.
  *It returns 1 if the speed matches one of the specified GPIO speed constants, and 0 otherwise.  
  */
 #define IS_VALID_GPIO_SPEED(speed)    (((speed) == GPIO_LOW_SPEED)    || \
                                        ((speed) == GPIO_MEDIUM_SPEED) || \
                                        ((speed) == GPIO_HIGH_SPEED)   || \
                                        ((speed) == GPIO_VERY_HIGH_SPEED))
 /**
  * @brief  Checks if the specified alternate function is valid.
  * @param  AF: Alternate function to check.
  * @retval 1 if the AF is valid, 0 otherwise.
  */
 #define IS_VALID_GPIO_AF(AF)          (((AF) == GPIO_AF00) || \
                                        ((AF) == GPIO_AF01) || \
                                        ((AF) == GPIO_AF02) || \
                                        ((AF) == GPIO_AF03) || \
                                        ((AF) == GPIO_AF04) || \
                                        ((AF) == GPIO_AF05) || \
                                        ((AF) == GPIO_AF06) || \
                                        ((AF) == GPIO_AF07) || \
                                        ((AF) == GPIO_AF08) || \
                                        ((AF) == GPIO_AF09) || \
                                        ((AF) == GPIO_AF10) || \
                                        ((AF) == GPIO_AF11) || \
                                        ((AF) == GPIO_AF12) || \
                                        ((AF) == GPIO_AF13) || \
                                        ((AF) == GPIO_AF14) || \
                                        ((AF) == GPIO_AF15) || \
                                        ((AF) == GPIO_AF15) || \
                                        ((AF) == GPIO_AF_DISABLED))

#define IS_VALID_GPIO_MODE_AF(mode,AF)     ((((mode) == GPIO_AF_PP_NO_PUPD) || \
                                            ((mode) == GPIO_AF_PP_PU)       || \
                                            ((mode) == GPIO_AF_PP_PD)       || \
                                            ((mode) == GPIO_AF_OD_NO_PUPD)  || \
                                            ((mode) == GPIO_AF_OD_PU)       || \
                                            ((mode) == GPIO_AF_OD_PD))      && \
                                            ((AF)!=GPIO_AF_DISABLED))  




#endif // MCAL_GPIO_GPIO_VALIDATION_H_
