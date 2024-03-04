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
#include"GPIO_Validation.h"
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
#define MASK_SHIFT_AF           (0x00000004UL)
#define MASK_READ_AF            (0x0000000FUL)
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
    else if(!IS_VALID_GPIO_MODE(GPIO_Config->GPIO_Mode))
    {
       ReturnState=GPIO_WrongModeConfig;
    }
    else if(((GPIO_Config->GPIO_Pin)<(0UL)) || ((GPIO_Config->GPIO_Pin)>(15UL)))
    {
        ReturnState=GPIO_WrongPin;
    }
    else if(!IS_VALID_GPIO_PORT(GPIO_Config->GPIO_Port))
    {
        ReturnState=GPIO_WrongPort;
    } 
    else if(!IS_VALID_GPIO_SPEED(GPIO_Config->GPIO_Speed))
    {
        ReturnState=GPIO_WrongSpeed;
    }
     else if(!IS_VALID_GPIO_AF(GPIO_Config->GPIO_AF))
    {
        ReturnState=GPIO_WrongAF;
    }
    else if(!IS_VALID_GPIO_MODE_AF((GPIO_Config->GPIO_Mode),(GPIO_Config->GPIO_AF)))
    {
        ReturnState=GPIO_WrongModeAFCFG;
    }

    else
    {
        volatile GPIO_Registers_t*Port = (volatile GPIO_Registers_t*)(GPIO_Config->GPIO_Port);
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
        uint32_t Loc_RegisterValue= Port->MODER;                                                /*Read the GPIO Mode Register                       */
        Loc_RegisterValue&=~((MASK_READ_GPIO_MODE)<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG));  /*Clear the Corresponding Bits in the register Value*/ 
        Loc_RegisterValue|=(LocGPIO_Mode<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG));            /*Modify the Mode register Value                    */  
        Port->MODER=Loc_RegisterValue;                                                          /* Write Back the value in GPIO Mode Register       */
       
        Loc_RegisterValue=Port->OTYPER;                                                         /*Read the GPIO OTYPE Register                     */
        Loc_RegisterValue&=~(MASK_READ_GPIO_OTYPE<<GPIO_Config->GPIO_Pin);                      /*Clear the Corresponding Bit in the register Value*/
        Loc_RegisterValue|=(LocGPIO_OutType<<GPIO_Config->GPIO_Pin);                            /*Modify the OType register Value                  */
        Port->OTYPER=Loc_RegisterValue;                                                         /*Write Back the value in GPIO OType Register      */

        Loc_RegisterValue=Port->PUPDR;                                                          /*Read the GPIO PUPD Register                      */
        Loc_RegisterValue&=~(MASK_READ_GPIO_PUPD<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG));    /*Clear the Corresponding Bit in the register Value*/
        Loc_RegisterValue|=(LocGPIO_PUPD<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG));            /*Modify The PUPD Register Value                   */
        Port->PUPDR=Loc_RegisterValue;                                                          /*Write Back the value in GPIO PUPD Register       */

        Loc_RegisterValue=Port->OSPEEDR;                                                          /* Read the GPIO OSpeed Register                     */
        Loc_RegisterValue&=~(MASK_READ_GPIO_OSPEED<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG));    /* Clear the Corresponding Bit in the register Value */
        Loc_RegisterValue|=((GPIO_Config->GPIO_Speed)<<((GPIO_Config->GPIO_Pin)*GPIO_2BITS_CFG)); /* Modify The OSpeed Register Value                  */
        Port->OSPEEDR=Loc_RegisterValue;                                                          /* Write Back the value in GPIO OSpeed Register      */
       
       if((GPIO_Config->GPIO_AF) !=GPIO_AF_DISABLED)
       {
        uint32_t AFpin=(GPIO_Config->GPIO_Pin)%8;
        if(AFpin>GPIO_PIN7)
        {
            Loc_RegisterValue= (Port->AFRH);
        }
        else
        {
             Loc_RegisterValue= (Port->AFRL);
        }
        //(AFpin>GPIO_PIN7)?(Loc_RegisterValue= (Port->AFRH)):(Loc_RegisterValue=Port->AFRL);
        Loc_RegisterValue&=~(MASK_READ_AF<<(AFpin*MASK_SHIFT_AF));
        Loc_RegisterValue|=((GPIO_Config->GPIO_AF)<<(AFpin*MASK_SHIFT_AF));
        //(AFpin>GPIO_PIN7)?((Port->AFRH)=Loc_RegisterValue):((Port->AFRL)=Loc_RegisterValue);
         if(AFpin>GPIO_PIN7)
        {
            (Port->AFRH)= Loc_RegisterValue;
        }
        else
        {
            (Port->AFRL)= Loc_RegisterValue;
        }
       }
       
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
    else if(!IS_VALID_GPIO_PORT((uint32_t*)GPIO_Port))
    {
        ReturnState=GPIO_WrongPort;
    } 
    else if((GPIO_PinState != GPIO_SET_PIN_HIGH) && (GPIO_PinState != GPIO_SET_PIN_LOW))
    {
        ReturnState=GPIO_WrongPinValue;
    }
    else
    {
        /**
         * @note :We don't need to Read/Modify/Write operation because we use Bit Set/Reset Register.
         */
        ((volatile GPIO_Registers_t*)GPIO_Port)->BSRR=GPIO_PinState<<GPIO_Pin; /* Note We don't need to Read/Modify/Write operation*/
         ReturnState=OK;
    }
    return ReturnState;
}

ErrorStatus_t GPIO_GetPinValue(void* GPIO_Port,uint32_t GPIO_Pin,uint32_t* GPIO_PinState)
{
    ErrorStatus_t ReturnState;
    if((GPIO_Port==NULL)||(GPIO_PinState==NULL))
    {
        ReturnState=NULL_POINTER;
    }
    else if(!IS_VALID_GPIO_PORT((uint32_t*)GPIO_Port))
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
