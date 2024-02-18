/*
 * File Name: GPIO.c
 * Layer	: MCAL
 * Module	: GPIO
 * Version  : V 0.0
 * Created	:  Feb 17, 2024
 * Author	: Anas Khamees
 */


/******************************************************************************************/
/*************************************** Includes *****************************************/
/******************************************************************************************/
#include "GPIO.h"
/******************************************************************************************/
/**************************************** Defines *****************************************/
/******************************************************************************************/
#define MASK_READ_GPIO_MODE     (0x00000003UL)
#define MASK_READ_GPIO_OTYPE    (0x00000001UL)
#define MASK_READ_GPIO_PUPD     (0x00000003UL)
#define MASK_READ_GPIO_OSPEED   (0x00000003UL)
#define MASK_READ_1BIT          (0x00000001UL)   
#define MASK_SHIFT_OTYPE_BIT    (0x00000002UL)
#define MASK_SHIFT_PUPD_BITS    (0x00000003UL)
#define GPIO_2BITS_CFG          (0x00000002UL)
/******************************************************************************************/
/*************************************** Data Types ***************************************/
/******************************************************************************************/
typedef struct{
    uint32_t MODER     ;
	uint32_t OTYPER    ;
    uint32_t OSPEEDR   ;
    uint32_t PUPDR     ;
    uint32_t IDR       ;
    uint32_t ODR       ;
    uint32_t BSRR      ;
    uint32_t LCKR      ;
    uint32_t AFRL      ;
    uint32_t AFRH      ;
}GPIO_Registers_t;

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------  API'S -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/

ErrorStatus_t GPIO_InitPin(GPIO_CFG_t* GPIO_Config)
{
    ErrorStatus_t ReturnState;
    if(GPIO_Config==NULL)
    {
        ReturnState = NULL_POINTER;
    }
    else if(GPIO_Config->GPIO_Mode != GPIO_OUT_PP_NO_PUPD && GPIO_Config->GPIO_Mode != GPIO_OUT_PP_PU && GPIO_Config->GPIO_Mode != GPIO_OUT_PP_PD&&
            GPIO_Config->GPIO_Mode != GPIO_OUT_OD_NO_PUPD && GPIO_Config->GPIO_Mode != GPIO_OUT_OD_PU && GPIO_Config->GPIO_Mode != GPIO_OUT_OD_PD&&
            GPIO_Config->GPIO_Mode != GPIO_IN_FLOATING    && GPIO_Config->GPIO_Mode != GPIO_IN_PU     && GPIO_Config->GPIO_Mode != GPIO_IN_PD    &&
            GPIO_Config->GPIO_Mode != GPIO_IN_ANALOG      && GPIO_Config->GPIO_Mode != GPIO_OUT_OD_PU && GPIO_Config->GPIO_Mode != GPIO_OUT_OD_PD&&
            GPIO_Config->GPIO_Mode != GPIO_AF_PP_NO_PUPD  && GPIO_Config->GPIO_Mode != GPIO_AF_PP_PU  && GPIO_Config->GPIO_Mode != GPIO_AF_PP_PD &&
            GPIO_Config->GPIO_Mode != GPIO_AF_OD_NO_PUPD  && GPIO_Config->GPIO_Mode != GPIO_AF_OD_PU  && GPIO_Config->GPIO_Mode != GPIO_AF_OD_PD )
    {
       ReturnState=GPIO_WrongModeConfig;
    }
    else if((GPIO_Config->GPIO_Pin)<0 || (GPIO_Config->GPIO_Pin)>15)
    {
        ReturnState=GPIO_WrongPin;
    }
    else if(GPIO_Config->GPIO_Port !=GPIO_PORTA && GPIO_Config->GPIO_Port !=GPIO_PORTB && GPIO_Config->GPIO_Port !=GPIO_PORTC&&
            GPIO_Config->GPIO_Port !=GPIO_PORTD && GPIO_Config->GPIO_Port !=GPIO_PORTE && GPIO_Config->GPIO_Port !=GPIO_PORTH)
    {
        ReturnState=GPIO_WrongPort;
    } 
    else if(GPIO_Config->GPIO_Speed !=GPIO_LOW_SPEED  && GPIO_Config->GPIO_Speed !=GPIO_MEDIUM_SPEED && 
            GPIO_Config->GPIO_Speed !=GPIO_HIGH_SPEED && GPIO_Config->GPIO_Speed !=GPIO_VERY_HIGH_SPEED)
    {
        ReturnState=GPIO_WrongSpeed;
    }

    else
    {
        /* decyption of the GPIO Mode Configuration Masks*/
        uint32_t LocGPIO_Mode=(GPIO_Config->GPIO_Mode)&MASK_READ_GPIO_MODE;
        uint32_t LocGPIO_OutType=((GPIO_Config->GPIO_Mode)>>MASK_SHIFT_OTYPE_BIT)&MASK_READ_GPIO_OTYPE ;
        uint32_t LocGPIO_PUPD=((GPIO_Config->GPIO_Mode)>>MASK_SHIFT_PUPD_BITS)&MASK_READ_GPIO_PUPD;
        /**
         * Read The Registers
         * Clear The Desired Bits in the register
         * Modify The Register Value 
         * Write back the new value in the Register
        */
        uint32_t Loc_RegisterValue=((volatile GPIO_Registers_t*)GPIO_Config->GPIO_Port)->MODER; /*Read the GPIO Mode Register                       */
        Loc_RegisterValue&=~MASK_READ_GPIO_MODE<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG);      /*Clear the Corresponding Bits in the register Value*/ 
        Loc_RegisterValue|=LocGPIO_Mode<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG);              /*Modify the Mode register Value                    */
        ((volatile GPIO_Registers_t*)GPIO_Config->GPIO_Port)->MODER=Loc_RegisterValue;          /* Write Back the value in GPIO Mode Register       */
     
        Loc_RegisterValue=((volatile GPIO_Registers_t*)GPIO_Config->GPIO_Port)->OTYPER;  /*Read the GPIO OTYPE Register                     */
        Loc_RegisterValue&=~MASK_READ_GPIO_OTYPE<<GPIO_Config->GPIO_Pin;                 /*Clear the Corresponding Bit in the register Value*/
        Loc_RegisterValue|=LocGPIO_OutType<<GPIO_Config->GPIO_Pin;                       /*Modify the OType register Value                  */
        ((volatile GPIO_Registers_t*)GPIO_Config->GPIO_Port)->OTYPER=Loc_RegisterValue;  /*Write Back the value in GPIO OType Register      */

        Loc_RegisterValue=((volatile GPIO_Registers_t*)GPIO_Config->GPIO_Port)->PUPDR;    /*Read the GPIO PUPD Register                      */
        Loc_RegisterValue&=~MASK_READ_GPIO_PUPD<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG);/*Clear the Corresponding Bit in the register Value*/
        Loc_RegisterValue|=LocGPIO_PUPD<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG);        /*Modify The PUPD Register Value                   */
        ((volatile GPIO_Registers_t*)GPIO_Config->GPIO_Port)->PUPDR=Loc_RegisterValue;    /*Write Back the value in GPIO PUPD Register       */

        Loc_RegisterValue=((volatile GPIO_Registers_t*)GPIO_Config->GPIO_Port)->OSPEEDR;       /* Read the GPIO OSpeed Register                     */
        Loc_RegisterValue&=~MASK_READ_GPIO_OSPEED<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG);   /* Clear the Corresponding Bit in the register Value */
        Loc_RegisterValue|=(GPIO_Config->GPIO_Speed)<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG);/* Modify The OSpeed Register Value                  */
        ((volatile GPIO_Registers_t*)GPIO_Config->GPIO_Port)->OSPEEDR=Loc_RegisterValue;       /* Write Back the value in GPIO OSpeed Register      */
        ReturnState=OK;
    }
    return ReturnState;
}


ErrorStatus_t GPIO_SetPinValue(void* GPIO_Port,uint32_t GPIO_Pin,uint32_t GPIO_PinState)
{
    ErrorStatus_t ReturnState;
    if(GPIO_Port==NULL)
    {
        ReturnState=NULL_POINTER;
    }
    else if(GPIO_Port !=GPIO_PORTA && GPIO_Port !=GPIO_PORTB && GPIO_Port !=GPIO_PORTC&&
            GPIO_Port !=GPIO_PORTD && GPIO_Port !=GPIO_PORTE && GPIO_Port !=GPIO_PORTH )
    {
        ReturnState=GPIO_WrongPort;
    } 
    else if(GPIO_PinState != GPIO_SET_PIN_HIGH && GPIO_PinState != GPIO_SET_PIN_LOW)
    {
        ReturnState=GPIO_WrongPinValue;
    }
    else
    {
        ((volatile GPIO_Registers_t*)GPIO_Port)->BSRR=GPIO_PinState<<GPIO_Pin;
         ReturnState=OK;
    }
    return ReturnState;
}
ErrorStatus_t GPIO_GetPinValue(void* GPIO_Port,uint32_t GPIO_Pin,uint32_t* GPIO_PinState)
{
    ErrorStatus_t ReturnState;
    if((GPIO_Port==NULL)||(GPIO_PinState))
    {
        ReturnState=NULL_POINTER;
    }
    else if(GPIO_Port !=GPIO_PORTA && GPIO_Port !=GPIO_PORTB && GPIO_Port !=GPIO_PORTC&&
            GPIO_Port !=GPIO_PORTD && GPIO_Port !=GPIO_PORTE && GPIO_Port !=GPIO_PORTH )
    {
        ReturnState=GPIO_WrongPort;
    } 
    else
    {
        *GPIO_PinState=((((volatile GPIO_Registers_t*)GPIO_Port)->IDR)>>GPIO_Pin) & MASK_READ_1BIT;  
        ReturnState=OK;
    }
    return ReturnState;
}
