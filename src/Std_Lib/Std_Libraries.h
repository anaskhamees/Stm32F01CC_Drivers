
/*
 * File Name: Std_Library.h
 * Version  : V 0.0
 * Created	:  Feb 11, 2024
 * Author	: Anas Khamees
 */

#ifndef STD_LIB_STD_LIBRARY_H_
#define STD_LIB_STD_LIBRARY_H_

/*---------------------------------- Defines ------------------------------------------*/
#define  NULL	((void *)0)
/*--------------------------------- Data Types ----------------------------------------*/

typedef unsigned char      uint8_t ;
typedef unsigned char      char_t ;
typedef signed char        sint8_t ;
typedef unsigned short     uint16_t;
typedef signed short int   sint16_t;
typedef unsigned long int  uint32_t;
typedef signed long int    sint32_t;
typedef unsigned long long uint64_t;
typedef signed long long   sint64_t;
typedef float 			   float32_t;
typedef double             float64_t;

typedef enum{
	OK,
	NOK,
	NULL_POINTER,
	WRONG_PARAMETER,
	NOT_READY,
	WrongPLLM_PARAMETER,
	WrongPLLN_PARAMETER,
	WrongPLLP_PARAMETER,
	WrongPLLQ_PARAMETER,
	WrongPLLSRC_PARAMETER,
	GPIO_WrongModeConfig,
	GPIO_WrongSpeed,
	GPIO_WrongPort,
	GPIO_WrongPin,
	GPIO_WrongPinValue,
	GPIO_WrongAF,
	GPIO_WrongModeAFCFG,
	NVIC_OK,
	NVIC_NOK,
	NVIC_WRONG_PARAMETER,
	SYSTICK_OK,
	SYSTICK_WRONG_PARAMETER,
	SYSTICK_NULL_POINTER,
	SYSTICK_NOK,
	SCHED_OK,
	SCHED_NULL_POINTER,
	USART_OK,
    USART_NOK,
    USART_INVALID_USART_CHANNEL,
    USART_INVALID_BAUDRATE,
    USART_INVALID_WORD_LENGTH,
    USART_INVALID_STOP_BITS,
    USART_INVALID_PARITY,
    USART_INVALID_SAMPLING,
    USART_NULL_PTR,
    USART_TX_TIMEOUT,
    USART_RX_TIMEOUT
}ErrorStatus_t;




#endif /* STD_LIB_STD_LIBRARY_H_ */
