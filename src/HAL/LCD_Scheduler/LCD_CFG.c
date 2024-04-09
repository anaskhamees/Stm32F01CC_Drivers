/*
 * File Name: LCD_CFG.c
 * Layer	: HAL
 * Module	: LCD
 * Version  : V 0.0
 * Created	: 03/22/2023 9:49:35 PM
 * Author	: Anas Khamees
 */ 
#include "MCAL/GPIO/GPIO.h"
#include "LCD_CFG.h"
#include "LCD.h"

/**
 * @brief Configuration of LCDs.
 * 
 * Array containing configurations for multiple LCDs.
 * Each LCD configuration includes pin and port and data length.
 */
LCD_t LCDs[_LCD_NUMBER]=
{
    [LCD1]= 
    {
    
        .LCD_Pins[RegisterSelect].pin=GPIO_PIN0,
        .LCD_Pins[RegisterSelect].port=GPIO_PORTA,

        .LCD_Pins[Enable].pin=GPIO_PIN1,
        .LCD_Pins[Enable].port=GPIO_PORTA,  

        .LCD_Pins[DB0].pin=GPIO_PIN4,
        .LCD_Pins[DB0].port=GPIO_PORTA,

        .LCD_Pins[DB1].pin=GPIO_PIN5,
        .LCD_Pins[DB1].port=GPIO_PORTA,

        .LCD_Pins[DB2].pin=GPIO_PIN6,
        .LCD_Pins[DB2].port=GPIO_PORTA,

        .LCD_Pins[DB3].pin=GPIO_PIN7,
        .LCD_Pins[DB3].port=GPIO_PORTA,

        .LCD_Pins[DB4].pin=GPIO_PIN8,
        .LCD_Pins[DB4].port=GPIO_PORTA,

        .LCD_Pins[DB5].pin=GPIO_PIN9,
        .LCD_Pins[DB5].port=GPIO_PORTA,

        .LCD_Pins[DB6].pin=GPIO_PIN10,
        .LCD_Pins[DB6].port=GPIO_PORTA,

        .LCD_Pins[DB7].pin=GPIO_PIN11,
        .LCD_Pins[DB7].port=GPIO_PORTA,

        .LCD_DataLength=EIGHT_BIT_MODE,

    }
};
