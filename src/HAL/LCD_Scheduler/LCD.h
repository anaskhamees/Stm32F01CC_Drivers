/*
 * File Name: LCD.h
 * Layer	: HAL
 * Module	: LCD
 * Version  : V 0.0
 * Created	: 03/22/2023 9:49:35 PM
 * Author	: Anas Khamees
 * Brief	: This header file defines structures, Enumerators and functions API's for LCD driver.
 */ 
#ifndef HAL_LCD_SCHEDULER_LCD_H_
#define HAL_LCD_SCHEDULER_LCD_H_

/****************************************************************************************/
/*                                      Includes                                        */
/****************************************************************************************/
#include "Std_Lib/Std_Libraries.h"
#include "LCD_CFG.h"

/****************************************************************************************/
/*                                      Defines                                         */
/****************************************************************************************/
#define FOUR_BIT_MODE	0x06
#define EIGHT_BIT_MODE  0x0A
#define LCD_ROW1		0x01
#define LCD_ROW2		0x02
/****************************************************************************************/
/*                                 LCD Requests State                                   */
/****************************************************************************************/
#define LCD_BUSY                1
#define LCD_READY               0
/******************************************************************************************/
/*                                   Types                                                */
/******************************************************************************************/

/**
 * @brief Type definition for a pointer to a function with no arguments and returning void.
 * 
 * This type is used to define callback functions that notify the user , The Asynchrounous function is Done.
 */
typedef void (*ReqCallBack_t)(void);

typedef struct 
{
    const char_t* String;       /* String To be Written on LCD */
    uint8_t StringLength;       /* The Length of String        */

    uint8_t LCD_State   ;       /* LCD handles current Request or Ready to recieve Requests:
                                 * LCD_BUSY
                                 * LCD_READY
                                 */

    uint8_t RequestType ;       /* Write Request, Clear Request and Set Position:
                                 * LCD_REQ_WRITE
                                 * LCD_REQ_CLEAR
                                 * LCD_REQ_SET_POS
                                 */
    uint8_t PosX;                /* PositionX on LCD which the Data to be Written */
    uint8_t PosY;                /* PositionY on LCD which the Data to be Written */
    ReqCallBack_t CallBack;      /* CallBack Function executed after Request Done To notify the user,that the Request is Done*/

}UserRequest_t;

/********************************************************************************************************/
/*                                      API's Prototypes                                                */
/********************************************************************************************************/

/**
 * @brief Initializes the LCD with the specified name.
 * 
 * This function initializes the LCD with the specified name by setting its state to initialization.
 * 
 * @param LCD_Name The index of the LCD to be initialized.
 */
void LCD_Init(uint8_t LCD_Name);
/**
 * @brief Gets the state of the specified LCD.
 * 
 * This function retrieves the state of the specified LCD. If the LCD is ready and in operation state,
 * it returns LCD_READY, otherwise, it returns LCD_BUSY.
 * 
 * @param LCD_Name The LCD ID .
 * @return The state of the LCD (LCD_READY if ready and in operation state, LCD_BUSY otherwise).
 */
uint8_t LCD_GetState(uint8_t LCD_Name);

/**
 * @brief Sets the cursor position asynchronously for the specified LCD.
 * 
 * This function initiates an asynchronous request to set the cursor position for the specified LCD.
 * It checks the validity of the position coordinates (PosX and PosY) and the LCD state.
 * If the position coordinates exceed the maximum allowed values or if the LCD is not ready,
 * it returns a WRONG_PARAMETER error status. Otherwise, it sets the request type to set position,
 * sets the LCD state to busy, assigns the position coordinates, assigns the callback function,
 * and returns an OK status.
 * 
 * @param LCD_Name The ID of the LCD for which the cursor position is to be set.
 * @param PosX The X position of the cursor (row) (0 or 1 for a 16x2 LCD).
 * @param PosY The Y position of the cursor (column) (0 to 15 for a 16x2 LCD).
 * @param CB The callback function to be executed after the request is completed.
 * @return Error status indicating success or failure of the operation.
 */
ErrorStatus_t LCD_SetCursorPosAsynch(uint8_t LCD_Name, uint8_t PosX,uint8_t PosY,ReqCallBack_t CB);
/**
 * @brief Writes a string to the LCD asynchronously.
 * 
 * This function initiates an asynchronous request to write a string to the LCD.
 * It checks for null pointer and string length constraints, and if the LCD is ready and in operation state,
 * it sets the string, string length, request type, LCD state to busy, and assigns the callback function.
 * 
 * @param LCD_Name The ID of the LCD to which the string is to be written.
 * @param string The string to be written to the LCD.
 * @param length The length of the string.
 * @param CB The callback function to be executed after the request is completed.
 * @return Error status indicating success or failure of the operation.
 */
ErrorStatus_t LCD_WriteStringAsynch(uint8_t LCD_Name, char_t* string,uint8_t length,ReqCallBack_t CB);
/**
 * @brief Clears the LCD asynchronously for the specified LCD.
 * 
 * This function initiates an asynchronous request to clear the screen for the specified LCD.
 * It sets the LCD state to busy, sets the request type to clear, and assigns the callback function.
 * 
 * @param LCD_Name The ID of the LCD for which the screen is to be cleared.
 * @param CB The callback function to be executed after the request is completed.
 */
void LCD_ClearScreenAsynch(uint8_t LCD_Name, ReqCallBack_t CB);


#endif // HAL_LCD_SCHEDULER_LCD_H_