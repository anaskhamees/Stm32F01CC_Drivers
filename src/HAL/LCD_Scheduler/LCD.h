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

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Std_Lib/Std_Libraries.h"
#include "LCD_CFG.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/*---------------------------------------- Configuration MACROs-----------------------------------------*/

#define FOUR_BIT_MODE	0x04
#define EIGHT_BIT_MODE  0x08
#define LCD_ROW1		0x01
#define LCD_ROW2		0x02
/*----------------------- LCD Request State --------------------------*/
#define LCD_BUSY                1
#define LCD_READY               2

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef void (*ReqCallBack_t)(void);
// typedef struct
// {
//     uint32_t pin; /* Which Pin Number will connect with LCD */
//     void* port;   /* The Pin in which Port ?                */
// }LCD_Pin_t;

// typedef struct 
// {
//     LCD_Pin_t LCD_Pins[_LCD_PINS]; /* Array of Pins CFG for 1 LCD      */
//     uint8_t LCD_DataLength       ; /* LCD Data Length 4-Bits or 8-Bits */  
// }LCD_t;


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
/************************************************APIs****************************************************/
/********************************************************************************************************/
 void LCD_Init(uint8_t LCD_Name);
uint8_t LCD_GetState(uint8_t LCD_Name);
ErrorStatus_t LCD_SetCursorPosAsynch(uint8_t LCD_Name, uint8_t PosX,uint8_t PosY,ReqCallBack_t CB);
ErrorStatus_t LCD_WriteStringAsynch(uint8_t LCD_Name, char_t* string,uint8_t length,ReqCallBack_t CB);
void LCD_ClearScreenAsynch(uint8_t LCD_Name, ReqCallBack_t CB);
























#endif // HAL_LCD_SCHEDULER_LCD_H_