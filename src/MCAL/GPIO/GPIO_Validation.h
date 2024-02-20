
#ifndef MCAL_GPIO_GPIO_VALIDATION_H_
#define MCAL_GPIO_GPIO_VALIDATION_H_


/******************************************************************************************/
/******************************** [Functions Like Macros] *********************************/
/******************************************************************************************/

/**
 * @brief :
 * This macro is used to check if a given GPIO mode (mode) is valid.
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

#endif // MCAL_GPIO_GPIO_VALIDATION_H_
