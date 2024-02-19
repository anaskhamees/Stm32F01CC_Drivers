/*
 * File Name: GPIO.h
 * Layer	: MCAL
 * Module	: GPIO
 * Version  : V 0.0
 * Created	:  Feb 17, 2024
 * Author	: Anas Khamees
 */

#ifndef GPIO_H_
#define GPIO_H_

/******************************************************************************************/
/*************************************** Includes *****************************************/
/******************************************************************************************/
#include"Std_Lib/Std_Libraries.h"

/******************************************************************************************/
/**************************************** Defines *****************************************/
/******************************************************************************************/
/*=========================== Base Addresses of GPIOs Peripherals ========================*/
#define GPIO_PORTA		((void*)0x40020000) /*GPIOA_BASE_ADDRESS*/
#define GPIO_PORTB		((void*)0x40020400) /*GPIOB_BASE_ADDRESS*/
#define GPIO_PORTC		((void*)0x40020800) /*GPIOC_BASE_ADDRESS*/
#define GPIO_PORTD		((void*)0x40020C00) /*GPIOD_BASE_ADDRESS*/
#define GPIO_PORTE		((void*)0x40021000) /*GPIOE_BASE_ADDRESS*/
#define GPIO_PORTH		((void*)0x40021C00) /*GPIOH_BASE_ADDRESS*/
/*=============================== Pins Position==========================================*/
#define GPIO_PIN0               0
#define GPIO_PIN1               1
#define GPIO_PIN2               2
#define GPIO_PIN3               3
#define GPIO_PIN4               4
#define GPIO_PIN5               5
#define GPIO_PIN6               6
#define GPIO_PIN7               7
#define GPIO_PIN8               8
#define GPIO_PIN9               9
#define GPIO_PIN10              10
#define GPIO_PIN11              11
#define GPIO_PIN12              12
#define GPIO_PIN13              13
#define GPIO_PIN14              14
#define GPIO_PIN15              15
/*-------------------------------------------------------------------------------------*/
/*------------------------------- Pin Configurations ----------------------------------*/
/*-------------------------------------------------------------------------------------*/
/*================================= GPIO SPEED=========================================*/
#define GPIO_LOW_SPEED          0
#define GPIO_MEDIUM_SPEED       1
#define GPIO_HIGH_SPEED         2
#define GPIO_VERY_HIGH_SPEED    3
/*================================= GPIO Value (State)==================================*/
#define GPIO_SET_PIN_HIGH       (0x00000001UL)  /* I write this value in BSRR Bit0:Bit15*/
#define GPIO_SET_PIN_LOW        (0x00010000UL)  /* I use Bit Set/Reset Register Bit16:Bit31*/

/*==================== Masks of GPIO Mode, Type and PUPD Configuration=====================*/
/**
 * @brief: I use the first 5 bits in the Mask to configure GPIO Mode, GPIO Type and GPIO PUPD.
 * @note :
 * (Bit0:Bit1) > Mode Configuration (Input / Output /Alternative Function).
 * (Bit2)      > Type Configuration (PushPull-OpenDrain).
 * (Bit3:Bit4) > PUPD Configuration (Pull Up or Pull down).
 */

/*------------------------------ ALL GPIO OutPut Configurations-------------------------*/
#define GPIO_OUT_PP_NO_PUPD     (0x00000001UL) 
#define GPIO_OUT_PP_PU          (0x00000009UL)
#define GPIO_OUT_PP_PD          (0x00000011UL)
#define GPIO_OUT_OD_NO_PUPD     (0x00000005UL)
#define GPIO_OUT_OD_PU          (0x0000000DUL)
#define GPIO_OUT_OD_PD          (0x00000015UL)
/*------------------------------ ALL GPIO Input Configurations-------------------------*/
#define GPIO_IN_FLOATING        (0x00000000UL)
#define GPIO_IN_PU              (0x00000008UL)
#define GPIO_IN_PD              (0x00000010UL)
#define GPIO_IN_ANALOG          (0x00000003UL)
/*----------------------------- ALL GPIO AF Configurations-------------------------*/
#define GPIO_AF_PP_NO_PUPD      (0x00000002UL)
#define GPIO_AF_PP_PU           (0x0000000AUL)
#define GPIO_AF_PP_PD           (0x00000012UL)
#define GPIO_AF_OD_NO_PUPD      (0x00000006UL)
#define GPIO_AF_OD_PU           (0x0000000EUL)
#define GPIO_AF_OD_PD           (0x00000016UL)

/******************************************************************************************/
/*************************************** Data Types ***************************************/
/******************************************************************************************/
/**
  * @brief  Configuration structure for GPIO initialization.
  */
typedef struct 
{
    void*    GPIO_Port;       /*!< Pointer to the GPIO port (GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, GPIO_PORTD, GPIO_PORTE, GPIO_PORTH) */
    uint32_t GPIO_Pin;        /*!< GPIO pin number (GPIO_PIN0 to GPIO_PIN15) */
    uint32_t GPIO_Mode;       /*!< GPIO pin mode:
                                   - GPIO_OUT_PP_NO_PUPD: Output push-pull without pull-up/pull-down
                                   - GPIO_OUT_PP_PU: Output push-pull with pull-up
                                   - GPIO_OUT_PP_PD: Output push-pull with pull-down
                                   - GPIO_OUT_OD_NO_PUPD: Output open-drain without pull-up/pull-down
                                   - GPIO_OUT_OD_PU: Output open-drain with pull-up
                                   - GPIO_OUT_OD_PD: Output open-drain with pull-down
                                   - GPIO_IN_FLOATING: Input floating
                                   - GPIO_IN_PU: Input with pull-up
                                   - GPIO_IN_PD: Input with pull-down
                                   - GPIO_IN_ANALOG: Analog mode
                                   - GPIO_AF_PP_NO_PUPD: Alternate function push-pull without pull-up/pull-down
                                   - GPIO_AF_PP_PU: Alternate function push-pull with pull-up
                                   - GPIO_AF_PP_PD: Alternate function push-pull with pull-down
                                   - GPIO_AF_OD_NO_PUPD: Alternate function open-drain without pull-up/pull-down
                                   - GPIO_AF_OD_PU: Alternate function open-drain with pull-up
                                   - GPIO_AF_OD_PD: Alternate function open-drain with pull-down */
    uint32_t GPIO_Speed;      /*!< GPIO pin speed:
                                   - GPIO_LOW_SPEED
                                   - GPIO_MEDIUM_SPEED
                                   - GPIO_HIGH_SPEED
                                   - GPIO_VERY_HIGH_SPEED */
} GPIO_CFG_t;

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------  API'S -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/

/**
  * @brief  Initializes a GPIO pin based on the provided configuration.
  * @param  GPIO_Config: Pointer to a structure containing GPIO pin configuration parameters.
  * @warning You Must Enable GPIO Peripheral CLock in RCC Peripheral. 
  * @retval ErrorStatus_t: Indicates the status of the initialization operation.
  *         - OK: Operation completed successfully.
  *         - NULL_POINTER: The pointer to GPIO_Config is NULL.
  *         - GPIO_WrongModeConfig: Incorrect GPIO mode configuration provided.
  *         - GPIO_WrongPin: Invalid GPIO pin specified.
  *         - GPIO_WrongPort: Invalid GPIO port specified.
  *         - GPIO_WrongSpeed: Invalid GPIO speed specified.
  */
ErrorStatus_t GPIO_InitPin(GPIO_CFG_t* GPIO_Config);
/**
 * @brief Sets the value of a specific GPIO pin.
 * @param GPIO_Port Pointer to the GPIO port base address.
 *        This parameter should be one of the following values:
 *        - GPIO_PORTA: GPIO Port A base address.
 *        - GPIO_PORTB: GPIO Port B base address.
 *        - GPIO_PORTC: GPIO Port C base address.
 *        - GPIO_PORTD: GPIO Port D base address.
 *        - GPIO_PORTE: GPIO Port E base address.
 *        - GPIO_PORTH: GPIO Port H base address.
 * @param GPIO_Pin Pin number should be a value between GPIO_PIN0 to GPIO_PIN15.
 * @param GPIO_PinState : This parameter should be one of the following values:
 *        - GPIO_SET_PIN_HIGH: Set the pin to a HIGH .
 *        - GPIO_SET_PIN_LOW: Set the pin to a LOW .
 * @return ErrorStatus_t Error status.
 *         - OK: Operation successful.
 *         - NULL_POINTER: GPIO_Port pointer is NULL.
 *         - GPIO_WrongPort: Invalid GPIO port specified.
 *         - GPIO_WrongPinValue: Invalid pin state specified.
 */
ErrorStatus_t GPIO_SetPinValue(void* GPIO_Port,uint32_t GPIO_Pin,uint32_t GPIO_PinState);
/**
 * @brief Gets the value of a specific GPIO pin.
 * @param GPIO_Port Pointer to the GPIO port base address.
 *        This parameter should be one of the following values:
 *        - GPIO_PORTA: GPIO Port A base address.
 *        - GPIO_PORTB: GPIO Port B base address.
 *        - GPIO_PORTC: GPIO Port C base address.
 *        - GPIO_PORTD: GPIO Port D base address.
 *        - GPIO_PORTE: GPIO Port E base address.
 *        - GPIO_PORTH: GPIO Port H base address.
 * @param GPIO_Pin Pin number to get the value for.
 *        This parameter should be a value between GPIO_PIN0 to GPIO_PIN15.
 * @param GPIO_PinState Pointer to store the retrieved pin state.
 *        This parameter should be a pointer to a uint32_t variable.
 * @return ErrorStatus_t Error status.
 *         - OK: Operation successful.
 *         - NULL_POINTER: GPIO_Port pointer or GPIO_PinState pointer is NULL.
 *         - GPIO_WrongPort: Invalid GPIO port specified.
 */
ErrorStatus_t GPIO_GetPinValue(void* GPIO_Port,uint32_t GPIO_Pin,uint32_t* GPIO_PinState);

#endif /* GPIO_H_ */
