/*
 * File Name: LCD_CFG.h
 * Layer	: HAL
 * Module	: LCD
 * Version  : V 0.0
 * Created	: 03/22/2023 9:49:35 PM
 * Author	: Anas Khamees
 */ 
#ifndef HAL_LCD_SCHEDULER_LCD_CFG_H_
#define HAL_LCD_SCHEDULER_LCD_CFG_H_

#define BUFFER_SIZE     10

typedef enum
{
    RegisterSelect,
    Enable,
    /*Read/Write Pin, I grounded it because I always Write on LCD */
    /* Lower Data Bus */
    DB0,
    DB1,
    DB2,
    DB3,
    /* Higher Data Bus */
    DB4,
    DB5,
    DB6,
    DB7,
    _LCD_PINS
}LCD_Pins_t;

typedef enum
{
    LCD1,
    _LCD_NUMBER
}LCD_Num_t;

typedef struct
{
    uint32_t pin; /* Which Pin Number will connect with LCD */
    void* port;   /* The Pin in which Port ?                */
}LCD_Pin_t;

typedef struct 
{
    LCD_Pin_t LCD_Pins[_LCD_PINS]; /* Array of Pins CFG for 1 LCD      */
    uint8_t LCD_DataLength       ; /* LCD Data Length 4-Bits or 8-Bits */  
}LCD_t;


#endif 