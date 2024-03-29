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


// const LCD_PortPin_t LCD_Pins_Arr[LCD_Num] = 
// {
//     [LCD1] =    // Configuration for LCD1
//     {
//         .LCD_arr[LCD_RS].Port  = GPIO_PORTA,    // Port B for Register Selection pin
//         .LCD_arr[LCD_RS].Pin   = GPIO_PIN0,     // Pin 0 for Register Selection pin

//         .LCD_arr[LCD_RW].Port  = GPIO_PORTA,    // Port B for Read/Write Selection pin
//         .LCD_arr[LCD_RW].Pin   = GPIO_PIN1,     // Pin 1 for Read/Write Selection pin

//         .LCD_arr[LCD_E].Port   = GPIO_PORTA,    // Port B for Enable Signal pin
//         .LCD_arr[LCD_E].Pin    = GPIO_PIN2,     // Pin 2 for Enable Signal pin

//         .LCD_arr[LCD_DB0].Port = GPIO_PORTA,    // Port B for Data pin 0
//         .LCD_arr[LCD_DB0].Pin  = GPIO_PIN3,     // Pin 3 for Data pin 0

//         .LCD_arr[LCD_DB1].Port = GPIO_PORTA,    // Port B for Data pin 1
//         .LCD_arr[LCD_DB1].Pin  = GPIO_PIN4,     // Pin 4 for Data pin 1

//         .LCD_arr[LCD_DB2].Port = GPIO_PORTA,    // Port B for Data pin 2
//         .LCD_arr[LCD_DB2].Pin  = GPIO_PIN5,     // Pin 5 for Data pin 2

//         .LCD_arr[LCD_DB3].Port = GPIO_PORTA,    // Port B for Data pin 3
//         .LCD_arr[LCD_DB3].Pin  = GPIO_PIN6,     // Pin 6 for Data pin 3

//         .LCD_arr[LCD_DB4].Port = GPIO_PORTA,    // Port B for Data pin 4
//         .LCD_arr[LCD_DB4].Pin  = GPIO_PIN7,     // Pin 7 for Data pin 4

//         .LCD_arr[LCD_DB5].Port = GPIO_PORTA,    // Port B for Data pin 5
//         .LCD_arr[LCD_DB5].Pin  = GPIO_PIN8,     // Pin 8 for Data pin 5

//         .LCD_arr[LCD_DB6].Port = GPIO_PORTA,    // Port B for Data pin 6
//         .LCD_arr[LCD_DB6].Pin  = GPIO_PIN9,     // Pin 9 for Data pin 6

//         .LCD_arr[LCD_DB7].Port = GPIO_PORTA,    // Port B for Data pin 7
//         .LCD_arr[LCD_DB7].Pin  = GPIO_PIN10,    // Pin 10 for Data pin 7
//     },
    
// };

LCD_t LCDs[_LCD_NUMBER]=
{
    [LCD1]= 
    {
    
        .LCD_Pins[RegisterSelect].pin=GPIO_PIN0,
        .LCD_Pins[RegisterSelect].port=GPIO_PORTA,

        .LCD_Pins[Enable].pin=GPIO_PIN1,
        .LCD_Pins[Enable].port=GPIO_PORTA,  

        .LCD_Pins[DB0].pin=GPIO_PIN2,
        .LCD_Pins[DB0].port=GPIO_PORTA,

        .LCD_Pins[DB1].pin=GPIO_PIN3,
        .LCD_Pins[DB1].port=GPIO_PORTA,

        .LCD_Pins[DB2].pin=GPIO_PIN4,
        .LCD_Pins[DB2].port=GPIO_PORTA,

        .LCD_Pins[DB3].pin=GPIO_PIN5,
        .LCD_Pins[DB3].port=GPIO_PORTA,

        .LCD_Pins[DB4].pin=GPIO_PIN6,
        .LCD_Pins[DB4].port=GPIO_PORTA,

        .LCD_Pins[DB5].pin=GPIO_PIN7,
        .LCD_Pins[DB5].port=GPIO_PORTA,

        .LCD_Pins[DB6].pin=GPIO_PIN8,
        .LCD_Pins[DB6].port=GPIO_PORTA,

        .LCD_Pins[DB7].pin=GPIO_PIN9,
        .LCD_Pins[DB7].port=GPIO_PORTA,

        .LCD_DataLength=EIGHT_BIT_MODE,

    }
};

/*LCD_t LCDs[_LCD_NUMBER] = {
    [LCD1] = {
        .LCD_Pins = {
            { .pin = GPIO_PIN0, .port = GPIO_PORTA },  // RegisterSelect
            { .pin = GPIO_PIN1, .port = GPIO_PORTA },  // Enable
            { .pin = GPIO_PIN2, .port = GPIO_PORTA },  // DB0
            { .pin = GPIO_PIN3, .port = GPIO_PORTA },  // DB1
            { .pin = GPIO_PIN4, .port = GPIO_PORTA },  // DB2
            { .pin = GPIO_PIN5, .port = GPIO_PORTA },  // DB3
            { .pin = GPIO_PIN6, .port = GPIO_PORTA },  // DB4
            { .pin = GPIO_PIN7, .port = GPIO_PORTA },  // DB5
            { .pin = GPIO_PIN8, .port = GPIO_PORTA },  // DB6
            { .pin = GPIO_PIN9, .port = GPIO_PORTA }   // DB7
        },
        .LCD_DataLength = 8  // Assuming it's an 8-bit LCD
    }
};*/