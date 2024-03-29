/*
 * File Name: LCD.c
 * Layer	: HAL
 * Module	: LCD
 * Version  : V 0.0
 * Created	: 03/22/2023 9:49:35 PM
 * Author	: Anas Khamees
 */ 

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/GPIO/GPIO.h"
#include "LCD.h"
#include "LCD_CFG.h"
#include "Scheduler/Scheduler.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/*--------------------------------- Commands MACROs ----------------------------------------------------*/

/** 
 *@brief: Clear command, write spaces on all DDRAM addresses.
 */
#define LCD_CLEAR_DISPLAY			0x01
/** 
 *@brief: Return Home command, Sets DDRAM address to (0x00) in AC and returns shifted display to its original position.
 *Cursor at home position at top left.
 */
#define LCD_RETURN_HOME				0x02
/** 
 *@brief: Entry Mode Set command, Sets cursor move direction(incremental or decremental) and enable the shift of entire display but cursor is fixed.
 */
#define LCD_ENTRY_MODE_DEC_SHIFT_OFF		 0x04
#define LCD_ENTRY_MODE_INC_SHIFT_OFF		 0x06  /*Normal Mode*/
#define LCD_ENTRY_MODE_DEC_SHIFT_ON			 0x05
#define LCD_ENTRY_MODE_INC_SHIFT_ON			 0x07
/** 
 *@brief: Display ON/OFF command,Set ON/OFF of entire display (D), cursor underline ON/OFF(C), and blinking of cursor position character(B).
 */
#define LCD_DISPLAY_CURSOR_BLINK_ALL_OFF		0x08
#define LCD_DISPLAY_ON_CURSOR_OFF_BLINK_OFF		0x0C
#define LCD_DISPLAY_ON_CURSOR_OFF_BLINK_ON		0x0D
#define LCD_DISPLAY_ON_CURSOR_ON_BLINK_OFF		0x0E
#define LCD_DISPLAY_CURSOR_BLINK_ALL_ON			0x0F
/** 
 *@brief: Cursor or Display shift command, Moves cursor and shifts display without changing DDRAM contents.
 */
#define LCD_CURSOR_SHIFT_LEFT		0x10	/*Shifts cursor position to the left  (AC is decreased by 1).			   */
#define	LCD_CURSOR_SHIFT_RIGHT		0x14	/*Shifts cursor position to the right (AC is increased by 1).			   */
#define	LCD_DISPLAY_SHIFT_LEFT		0x18	/*Shifts the entire display to the left, cursor follows the display shift. */ 
#define LCD_DISPLAY_SHIFT_RIGHT		0x1C	/*Shifts the entire display to the right, cursor follows the display shift.*/
/** 
 *@brief: Function Set command, Sets interface data length (DL:8-bit/4-bit), numbers of display line (N: 2-line/1-line), and display font type (F: 5x11 dots/ 5x8 dots).
 */
#define LCD_4BIT_MODE		0x28
#define LCD_8BIT_MODE		0x38
/** 
 *@brief: Set CGRAM command, Set CGRAM address in address counter.
 */
#define LCD_CGRAM_START_ADDRESS			0x40
/** 
 *@brief: Set DDRAM command, Set DDRAM address in address counter.
 */
#define LCD_DDRAM_START_ADDRESS			0x80

/*------------------------ LCD State ----------------------------------*/
#define LCD_STATE_INIT          1
#define LCD_STATE_OPERATION     2
#define LCD_STATE_OFF           3
/*----------------------- LCD Initialization Modes -------------------*/
#define LCD_POWER_ON            1
#define LCD_FUNCTIONAL_SET      2
#define LCD_DISPLAY_SETTINGS    3
#define LCD_DISPLAY_CLEAR       4
#define LCD_ENTRY_MODE          5
#define LCD_END                 6
/*---------------------- LCD Request Types ---------------------------*/
#define LCD_REQ_WRITE           1
#define LCD_REQ_CLEAR           2
#define LCD_REQ_SET_POS         3
/*-------------- state machine of Send Commands on LCD ---------------*/
#define LCD_SEND_COMMAND_DATA_READY    1
#define LCD_RESET_ENABLE               2
#define LCD_SEND_COMMAND_DATA_4BIT     3
#define LCD_RESET_ENABLE_4BIT          4
/*--------------------- Required Time to Write Command on LCD -----------------------*/
/**
 *@brief : 
 * 1 TickTime to send first Higher Nibble of 8bits (command or data).
 * 1 TickTime to Reset Enable Pin.
 * 1 TickTime to to send Second Lower Nibble of 8bits (command or data).
 * 1 TickTime to Reset Enable Pin
 */
#define LCD_4BIT_FUN_SET_REQ_TIME            8    /* 8 mSec required to Done Send Fun Set command in 4 bit mode */

#define LCD_4BIT_SEND_DATA_COMND_REQ_TIME    4    /* 4 mSec required to Done Send Data in 4 bit mode            */


#define READ_BIT(COMMAND_DATA,BIT_POS)      (((COMMAND_DATA)>>(BIT_POS))&(0x01))
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*****************************************Global Variables***********************************************/
/********************************************************************************************************/
uint8_t TimeMS=0;
uint8_t LCD_State[_LCD_NUMBER];
extern LCD_t LCDs[_LCD_NUMBER];
uint8_t LCD_Command_DataState[_LCD_NUMBER]={LCD_SEND_COMMAND_DATA_READY};
UserRequest_t userRequest[_LCD_NUMBER];
 

/********************************************************************************************************/
/*****************************************Static Functions***********************************************/
/********************************************************************************************************/
static void LCD_PinsInit(void)
{
    ErrorStatus_t ReturnError;
    GPIO_CFG_t LCD_Pin;
    LCD_Pin.GPIO_AF=GPIO_AF_DISABLED;
    LCD_Pin.GPIO_Mode=GPIO_OUT_PP_NO_PUPD;
    LCD_Pin.GPIO_Speed=GPIO_LOW_SPEED;
    uint8_t LCD_pins=0;
    for (uint16_t LCD_Num=0;LCD_Num<_LCD_NUMBER;LCD_Num++)
    {
        LCD_pins=(LCDs[LCD_Num].LCD_DataLength==EIGHT_BIT_MODE)?EIGHT_BIT_MODE:FOUR_BIT_MODE;
        for(uint16_t Pin=0;Pin<LCD_pins;Pin++)
        {
          LCD_Pin.GPIO_Port=LCDs[LCD_Num].LCD_Pins[Pin].port;
          LCD_Pin.GPIO_Pin =LCDs[LCD_Num].LCD_Pins[Pin].pin;
          ReturnError=GPIO_InitPin(&LCD_Pin);
        }
    }
}

static void LCD_Send4Bit(LCD_Num_t LCD_Name, uint8_t DataORcommand)
{
    ErrorStatus_t ReturnError;
    uint8_t BitPos=0;

    for (uint8_t DataBus = DB0; DataBus <= DB3; DataBus++)
    {
        if(READ_BIT(DataORcommand,BitPos++))
        {
            ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[DataBus].port,LCDs[LCD_Name].LCD_Pins[DataBus].pin,GPIO_SET_PIN_HIGH);
        }
        else
        {
            ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[DataBus].port,LCDs[LCD_Name].LCD_Pins[DataBus].pin,GPIO_SET_PIN_LOW);
        }
    }
}

static void LCD_Send8Bit(LCD_Num_t LCD_Name, uint8_t DataORcommand)
{
    ErrorStatus_t ReturnError;
    uint8_t BitPos=0;

    for (uint8_t DataBus = DB0; DataBus <= DB7; DataBus++)
    {
        if(READ_BIT(DataORcommand,BitPos++))
        {
            ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[DataBus].port,LCDs[LCD_Name].LCD_Pins[DataBus].pin,GPIO_SET_PIN_HIGH);
        }
        else
        {
            ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[DataBus].port,LCDs[LCD_Name].LCD_Pins[DataBus].pin,GPIO_SET_PIN_LOW);
        }
    }
}

static void LCD_SendCommand(LCD_Num_t LCD_Name,uint8_t command)
{
    ErrorStatus_t ReturnError;
    if(LCD_Name>=_LCD_NUMBER)
    {
        ReturnError=NOK;
    }
    else
    {
            /* 1. Reset Register Select Pin To LOW (Command mode) */
            ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[RegisterSelect].port,LCDs[LCD_Name].LCD_Pins[RegisterSelect].pin,GPIO_SET_PIN_LOW);
            /* 2. I grounded the Read/Write Pin in Hardware, so Read/Write Pin is Always LOW */
            /* 3. Send Command to LCD Data Bus pins but depend on 4-bits or 8-bits Mode.     */
           
           switch (LCD_Command_DataState[LCD_Name])
           {
            case LCD_SEND_COMMAND_DATA_READY:
                if(LCDs[LCD_Name].LCD_DataLength==FOUR_BIT_MODE)
                {
                    LCD_Send4Bit(LCD_Name,command>>4);
                }
            else
                {
                    LCD_Send8Bit(LCD_Name,command);
                }
                /* 4. Set Enable Signal To HIGH */
                ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,LCDs[LCD_Name].LCD_Pins[Enable].pin,GPIO_SET_PIN_HIGH);
                LCD_Command_DataState[LCD_Name]=LCD_RESET_ENABLE;
                break;

            case LCD_RESET_ENABLE:
                /* 5. Reset Enable Signal To LOW */
                ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,LCDs[LCD_Name].LCD_Pins[Enable].pin,GPIO_SET_PIN_LOW);
                if(LCDs->LCD_DataLength==FOUR_BIT_MODE)
                {
                LCD_Command_DataState[LCD_Name]=LCD_SEND_COMMAND_DATA_4BIT; 
                }
                else
                {
                    LCD_Command_DataState[LCD_Name]=LCD_SEND_COMMAND_DATA_READY; 
                }
                break;
                case LCD_SEND_COMMAND_DATA_4BIT:
                    LCD_Send4Bit(LCD_Name,command);
                /* 6. Set Enable Signal To HIGH */
                    ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,LCDs[LCD_Name].LCD_Pins[Enable].pin,GPIO_SET_PIN_HIGH);
                    LCD_Command_DataState[LCD_Name]=LCD_RESET_ENABLE_4BIT; 
                    break;
                    
                case LCD_RESET_ENABLE_4BIT:
                    ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,LCDs[LCD_Name].LCD_Pins[Enable].pin,GPIO_SET_PIN_LOW);
                    LCD_Command_DataState[LCD_Name]=LCD_SEND_COMMAND_DATA_READY;
                    break;
            default: /* No thing to do but for MISRA*/
                break;
           }
    }
}

static void LCD_SendData(LCD_Num_t LCD_Name ,uint8_t Data)
{
    ErrorStatus_t ReturnError;
    if(LCD_Name>=_LCD_NUMBER)
    {
        ReturnError=NOK;
    }
    else
    {
            /* 1. Set Register Select Pin To HIGH (Data mode) */
            ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[RegisterSelect].port,LCDs[LCD_Name].LCD_Pins[RegisterSelect].pin,GPIO_SET_PIN_HIGH);
            /* 2. I grounded the Read/Write Pin in Hardware */
            /* 3. Send Charachter to LCD Data Bus pins */
            switch (LCD_Command_DataState[LCD_Name])
            {
                case LCD_SEND_COMMAND_DATA_READY:
                    if(LCDs[LCD_Name].LCD_DataLength==FOUR_BIT_MODE)
                    {
                        LCD_Send4Bit(LCD_Name,Data>>4);
                    }
                else
                    {
                        LCD_Send8Bit(LCD_Name,Data);
                    }
                    /* 4. Set Enable Signal To HIGH */
                    ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,LCDs[LCD_Name].LCD_Pins[Enable].pin,GPIO_SET_PIN_HIGH);
                    LCD_Command_DataState[LCD_Name]=LCD_RESET_ENABLE;
                    break;

                case LCD_RESET_ENABLE:
                    /* 5. Reset Enable Signal To LOW  (Send is DONE)*/
                    ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,LCDs[LCD_Name].LCD_Pins[Enable].pin,GPIO_SET_PIN_LOW);
                    if(LCDs->LCD_DataLength==FOUR_BIT_MODE)
                    {
                        LCD_Command_DataState[LCD_Name]=LCD_SEND_COMMAND_DATA_4BIT; 
                    }
                    else
                    {
                        LCD_Command_DataState[LCD_Name]=LCD_SEND_COMMAND_DATA_READY; 
                    }
                    break;
                    case LCD_SEND_COMMAND_DATA_4BIT:
                        LCD_Send4Bit(LCD_Name,Data);
                    /* 6. Set Enable Signal To HIGH */
                        ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,LCDs[LCD_Name].LCD_Pins[Enable].pin,GPIO_SET_PIN_HIGH);
                        LCD_Command_DataState[LCD_Name]=LCD_RESET_ENABLE_4BIT; 
                        break;
                        
                    case LCD_RESET_ENABLE_4BIT:
                        ReturnError=GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,LCDs[LCD_Name].LCD_Pins[Enable].pin,GPIO_SET_PIN_LOW);
                        LCD_Command_DataState[LCD_Name]=LCD_SEND_COMMAND_DATA_READY;
                        break;
                default: /* No thing to do but for MISRA*/
                    break;
            }
    }
}

static void LCD_InitStateMachine(void)
{
    static uint8_t InitState=LCD_POWER_ON;
    TimeMS+=TICK_TIME;

    /**
     * @note : I calculate the Time required for Done Sending a command depend on the max Time requird (4-bit mode)
    */
    switch (InitState)
    {
    case LCD_POWER_ON:
        /* 1- Power on the LCD and wait for more than 30 ms after VDD rises to 4.5V */
        if(TimeMS>30)
        {
            LCD_PinsInit();
            InitState=LCD_FUNCTIONAL_SET;
            TimeMS=0;
        }
        break;
    
    case LCD_FUNCTIONAL_SET:
        
        /* 2- Send Function Set Command for 4-bit or 8-bit mode */					
        for(uint8_t LCD_Name=0;LCD_Name<_LCD_NUMBER;LCD_Name++)
        {
            if(LCDs[LCD_Name].LCD_DataLength==EIGHT_BIT_MODE)
            {
                LCD_SendCommand(LCD_Name,LCD_8BIT_MODE);
            }
            else
            {
                if(TimeMS<=LCD_4BIT_SEND_DATA_COMND_REQ_TIME)
                {
                /* LCD Data sheet constrain */
                LCD_SendCommand(LCD_Name,0x02); /* Need Enhancement */
                }
               else
               {
                    LCD_SendCommand(LCD_Name,LCD_4BIT_MODE);
               }
            }
        }
        
        if((TimeMS>LCD_4BIT_FUN_SET_REQ_TIME)&&(LCD_Command_DataState[0]==LCD_SEND_COMMAND_DATA_READY))
        {
            InitState=LCD_DISPLAY_SETTINGS;
            TimeMS=0;
        }
        break;

    case LCD_DISPLAY_SETTINGS:
        
        /*3- Send Display ON/OFF Control Command */
        for(uint8_t LCD_Name=0;LCD_Name<_LCD_NUMBER;LCD_Name++)
        {
            LCD_SendCommand(LCD_Name,LCD_DISPLAY_CURSOR_BLINK_ALL_ON);
        }

        if((TimeMS>LCD_4BIT_SEND_DATA_COMND_REQ_TIME)&&(LCD_Command_DataState[0]==LCD_SEND_COMMAND_DATA_READY))
        {       
            InitState=LCD_DISPLAY_CLEAR;
            TimeMS=0;
        }
        break;

    case LCD_DISPLAY_CLEAR:
        /* 4- Send Clear Display Command */
        for(uint8_t LCD_Name=0;LCD_Name<_LCD_NUMBER;LCD_Name++)
        {
            LCD_SendCommand(LCD_Name,LCD_CLEAR_DISPLAY);
        }
        if((TimeMS>LCD_4BIT_SEND_DATA_COMND_REQ_TIME)&&(LCD_Command_DataState[0]==LCD_SEND_COMMAND_DATA_READY))
        {
            InitState=LCD_ENTRY_MODE;
            TimeMS=0;
        }    
        
        break;

     case LCD_ENTRY_MODE:
         
        /* 5- Send Clear Display Command */
        for(uint8_t LCD_Name=0;LCD_Name<_LCD_NUMBER;LCD_Name++)
        {
            LCD_SendCommand(LCD_Name,LCD_ENTRY_MODE_INC_SHIFT_OFF);
        }

       if((TimeMS>LCD_4BIT_SEND_DATA_COMND_REQ_TIME)&&(LCD_Command_DataState[0]==LCD_SEND_COMMAND_DATA_READY))
        {
            InitState=LCD_END;
            TimeMS=0;
        }    
        break;

    case LCD_END:
       
        for (uint8_t LCD_ID = 0; LCD_ID < _LCD_NUMBER; LCD_ID++) 
        {
             /* 6- End of LCD Initialization */
            LCD_State[LCD_ID]=LCD_STATE_OPERATION;
           /* Initialize LCD_State to LCD_READY for each UserRequest_t structure*/
            userRequest[LCD_ID].LCD_State = LCD_READY;
        }
        break;
    
    default: /* Nothing to do but for MISRA */
        break;
    }
}

/* ------------------------------ LCD Processes -------------------------------------*/
static void LCD_WriteProcess(uint8_t LCD_Name)
{
    uint8_t currentPos=0;
    uint8_t length=userRequest[LCD_Name].StringLength;
    uint8_t* string=userRequest[LCD_Name].String;

    LCD_SendData(LCD_Name,string[currentPos]);
    if(LCD_Command_DataState[LCD_Name]==LCD_SEND_COMMAND_DATA_READY)
    {
        currentPos++;
    }
    if((LCD_Command_DataState[LCD_Name]==LCD_SEND_COMMAND_DATA_READY)&&(currentPos==length))
    {
        userRequest[LCD_Name].LCD_State=LCD_READY;
    }
}

static void LCD_ClearProcess(uint8_t LCD_Name)
{
    LCD_SendCommand(LCD_Name,LCD_CLEAR_DISPLAY);
     if(LCD_Command_DataState[LCD_Name]==LCD_SEND_COMMAND_DATA_READY)
    {
        userRequest[LCD_Name].LCD_State=LCD_READY;
    }
}

static void LCD_SetPosProcess(uint8_t LCD_Name)
{
    uint8_t row=userRequest[LCD_Name].PosX;
    uint8_t colm=userRequest[LCD_Name].PosY;
    uint8_t CursorPos=0;
    /* 
     * @note: start Address of DDRAM is  0x80.
     * The first location in DDRAM is (0x80 +0x00) > First Location in the First Row at LCD.
     * The second location is (0x80 +0x01) > Second Location in the First Row at LCD, and so on.
     * Row 0 , the first row 
     */
    if(row==0)
    {
        CursorPos=LCD_DDRAM_START_ADDRESS+colm;
    }
    /* 
     * @note: The Second Line at LCD start from (0x80 + 0x40) because each line can stores 40 characters.
     * The First Location in the Second Row at LCD is 0xC0 (0x80+0x40).
     * The Second Location in the Second Row at LCD is (0xC0 +0x01), and so on.
     * Row 1 , the second row */
    else
    {
        CursorPos=LCD_DDRAM_START_ADDRESS+40+colm;
    }

    LCD_SendCommand(LCD_Name,CursorPos);
     if(LCD_Command_DataState[LCD_Name]==LCD_SEND_COMMAND_DATA_READY)
    {
        userRequest[LCD_Name].LCD_State=LCD_READY;
    }

}
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void LCD_Init(uint8_t LCD_Name)
{
    LCD_State[LCD_Name]=LCD_STATE_INIT;
}

uint8_t LCD_GetState(uint8_t LCD_Name)
{
    uint8_t State=0;
    State=((userRequest[LCD_Name].LCD_State==LCD_READY)&&(LCD_State[LCD_Name]==LCD_STATE_OPERATION))?LCD_READY:LCD_BUSY;
    
    return State;
}

void LCD_ClearScreenAsynch(uint8_t LCD_Name, ReqCallBack_t CB)
{   //check state
    userRequest[LCD_Name].LCD_State=LCD_BUSY;
    userRequest[LCD_Name].RequestType=LCD_REQ_CLEAR; 
    userRequest[LCD_Name].CallBack=CB;
}

ErrorStatus_t LCD_WriteStringAsynch(uint8_t LCD_Name, char_t* string,uint8_t length,ReqCallBack_t CB)
{
    ErrorStatus_t ReturnError=NOK;
    if(string==NULL)
    {
        ReturnError=NULL_POINTER;
    }
    /* I use 16x2 LCD, so LCD The max length of String is 16 bytes */
    else if(length>16)
    {
        ReturnError=WRONG_PARAMETER;
    }
    else if((LCD_State[LCD_Name]==LCD_STATE_OPERATION)&&(userRequest[LCD_Name].LCD_State==LCD_READY))
    {
        userRequest[LCD_Name].String=string;
        userRequest[LCD_Name].StringLength=length;
        userRequest[LCD_Name].RequestType=LCD_REQ_WRITE;
        userRequest[LCD_Name].LCD_State=LCD_BUSY;
        userRequest[LCD_Name].CallBack=CB;
    
        ReturnError=OK;
    }
    else
    {
        /* Do Nothing but for MISRA */
    }
    return ReturnError;
}

ErrorStatus_t LCD_SetCursorPosAsynch(uint8_t LCD_Name, uint8_t PosX,uint8_t PosY,ReqCallBack_t CB)
{
    
    ErrorStatus_t ReturnError=NOK;
    /* I use 16x2 LCD, so LCD The max Xposition is 15 */
    if(PosX>15)
    {
        ReturnError=WRONG_PARAMETER;
    }
    /* I use 16x2 LCD, so LCD The max Yposition is 1 */
    else if(PosY>1)
    {
        ReturnError=WRONG_PARAMETER;
    }
    else if((LCD_State[LCD_Name]==LCD_STATE_OPERATION)&&(userRequest[LCD_Name].LCD_State==LCD_READY))
    {
        userRequest[LCD_Name].RequestType=LCD_REQ_SET_POS;
        userRequest[LCD_Name].LCD_State=LCD_BUSY;
        userRequest[LCD_Name].PosX=PosX;
        userRequest[LCD_Name].PosY=PosY;
        userRequest[LCD_Name].CallBack=CB;
        ReturnError=OK;

    }
    else
    {
        /* Do Nothing but for MISRA */
    }

    return ReturnError;
}

/* Task comes each 2 mSec*/
void LCD_Task(void)
{
    for (uint8_t LCD_Name=0;LCD_Name<_LCD_NUMBER;LCD_Name++)
    {   
        if(LCD_State[LCD_Name]==LCD_STATE_INIT)
        {
            LCD_InitStateMachine();
        }
        else if(LCD_State[LCD_Name]==LCD_STATE_OPERATION)
        {
            if(userRequest[LCD_Name].LCD_State==LCD_BUSY)
            {
                switch (userRequest[LCD_Name].RequestType)
                {
                case LCD_REQ_WRITE:
                    LCD_WriteProcess(LCD_Name);
                    break;
                case LCD_REQ_CLEAR:
                    LCD_ClearProcess(LCD_Name);
                    break;
                case LCD_REQ_SET_POS:
                    LCD_SetPosProcess(LCD_Name);
                    break;
                default:
                    break;
                }
            }
        }
    }
}
