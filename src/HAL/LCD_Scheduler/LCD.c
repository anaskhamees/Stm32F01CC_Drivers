/*
 * File Name: LCD.c
 * Layer	: HAL
 * Module	: LCD
 * Version  : V 0.0
 * Created	: 03/22/2023 9:49:35 PM
 * Author	: Anas Khamees
 */ 

/********************************************************************************************************/
/*                                               Includes                                               */
/********************************************************************************************************/
#include "MCAL/GPIO/GPIO.h"
#include "LCD.h"
#include "LCD_CFG.h"
#include "Scheduler/Scheduler.h"
/********************************************************************************************************/
/*                                  LCD Commands MACROs                                                 */
/********************************************************************************************************/

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
#define LCD_SEND_COMMAND_DATA_READY    0
#define LCD_RESET_ENABLE               1
#define LCD_SEND_COMMAND_DATA_4BIT     2
#define LCD_RESET_ENABLE_4BIT          3
#define LCD_SEND_COMMAND_DATA_IDLE     4
/*-------------- Required Time to Write Command on LCD 4bits mode ---------------*/
/**
 *@brief : 
 * 1 TickTime to send first Higher Nibble of 8bits (command or data) and set enable.
 * 1 TickTime to Reset Enable Pin.
 * 1 TickTime to to send Second Lower Nibble of 8bits (command or data) and set enable.
 * 1 TickTime to Reset Enable Pin.
 */

#define LCD_4BIT_FUN_SET_REQ_TIME            8    /* 8 mSec required to Done Send Fun Set command in 4 bit mode */

#define LCD_4BIT_SEND_DATA_COMND_REQ_TIME    4    /* 4 mSec required to Done Send Data in 4 bit mode            */

/**
 * @brief Macro Function to read a specific bit from a Number value.
 * 
 * This macro takes two arguments: COMMAND_DATA and BIT_POS. It shifts the COMMAND_DATA
 * right by BIT_POS positions and then performs a bitwise AND operation with 0x01 to
 * extract the desired bit value.
 * 
 * @param COMMAND_DATA The data value from which to read the bit.
 * @param BIT_POS      The position of the bit to be read (0 to 8).
 * @return             The value of the specified bit (0 or 1).
 */
#define READ_BIT(COMMAND_DATA,BIT_POS)      (((COMMAND_DATA)>>(BIT_POS))&(0x01))

/********************************************************************************************************/
/*                                        Global Variables                                              */
/********************************************************************************************************/

/**
 * @brief : Variable represent Time in milliSeconds
*/
uint8_t TimeMS=0;

/**
 * @brief : Variable represent the State of LCD : 
 *          - LCD_STATE_INIT
 *          - LCD_STATE_OPERATION
 *          - LCD_STATE_OFF
*/
uint8_t LCD_State[_LCD_NUMBER];

extern LCD_t LCDs[_LCD_NUMBER];
/**
 * @brief : variable for State machine of send command function
 */
uint8_t LCD_Command_DataState[_LCD_NUMBER]={LCD_SEND_COMMAND_DATA_READY};

/**
 * @brief : struct contains Buffer (Array of the user request data) for send data or command to LCD.
 */
typedef struct 
{
    /* Array of structs represent a Buffer of UserRequests for One LCD */
    UserRequest_t userRequestBuffer[BUFFER_SIZE];

}LCD_Buffer_t;

/**
 * @brief : Array of structs, each struct represent UserRequest Buffer for LCD
 */
LCD_Buffer_t Buffer[_LCD_NUMBER];

/**
 *  Variable represent the index of User Requests Buffer
 */
uint8_t UsrReqBufferIdx=0;

/**
 *  Variable represent the index of Done User Requests
 */
uint8_t DoneUsrReqIdx=0;

/**
 * @brief : variable for Current position of string charaters
 */
uint8_t currentPos=0;

/********************************************************************************************************/
/*                                  Static Functions Prototypes                                         */
/********************************************************************************************************/
static void LCD_PinsInit(void);
static void LCD_Send4Bit(LCD_Num_t LCD_Name, uint8_t DataORcommand);
static void LCD_Send8Bit(LCD_Num_t LCD_Name, uint8_t DataORcommand);
static void LCD_SendCommand(LCD_Num_t LCD_Name, uint8_t command);
static void LCD_SendData(LCD_Num_t LCD_Name, uint8_t Data);
static void LCD_InitStateMachine(void);
static void LCD_WriteProcess(uint8_t LCD_Name);
static void LCD_ClearProcess(uint8_t LCD_Name);
static void LCD_SetPosProcess(uint8_t LCD_Name);

/**
 * @brief Task function for handling LCD operations, comes each 2 mSec.
 * 
 * This function is called periodically at a 2ms interval to handle LCD-related tasks.
 * It increments the TimeMS variable to keep track of time and iterates through each LCD
 * device to perform LCD initialization or handle user requests based on the current LCD state.
 * If a user request is pending and the LCD is not busy, the function processes the request
 * accordingly (write, clear, or set position).
 */
void LCD_Task(void)
{
    TimeMS += TICK_TIME;  /**< Increment the time variable by the tick time */

    /* Iterate through each LCD device */
    for (uint8_t LCD_Name = 0; LCD_Name < _LCD_NUMBER; LCD_Name++)
    {   
        /* Check if LCD is in initialization state */
        if (LCD_State[LCD_Name] == LCD_STATE_INIT)
        {
            LCD_InitStateMachine();  /**< Initialize LCD state machine */
        }
        /* Check if LCD is in operation state */
        else if (LCD_State[LCD_Name] == LCD_STATE_OPERATION)
        {
            /* Check if LCD is not busy and user request is pending */
            if (Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].LCD_State == LCD_BUSY)
            {
                /* Process user request based on request type */
                switch (Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].RequestType)
                {
                    case LCD_REQ_WRITE:
                        LCD_WriteProcess(LCD_Name);  /**< Process write request */
                        break;
                    case LCD_REQ_CLEAR:
                        LCD_ClearProcess(LCD_Name);  /**< Process clear request */
                        break;
                    case LCD_REQ_SET_POS:
                        LCD_SetPosProcess(LCD_Name);  /**< Process set position request */
                        break;
                    default:
                        break;
                }
            }
        }
    }
}



/**
 * @brief Initializes the LCD state machine to configure the LCD display.
 * 
 * This function manages the LCD initialization process by transitioning through different
 * states to set up the LCD display. It calculates the time required for sending commands
 * based on the maximum time required in 4-bit mode. The initialization process includes
 * powering on the LCD, setting the functional mode (4-bit or 8-bit), configuring display
 * settings, clearing the display, setting the entry mode, and completing the initialization.
 * Once initialization is complete, it transitions the LCD state to operation mode.
 */
static void LCD_InitStateMachine(void)
{
    static uint8_t InitState = LCD_POWER_ON; /**< Current initialization state */

    switch (InitState)
    {
        case LCD_POWER_ON:
            /* Power on the LCD and wait for more than 30 ms after VDD rises to 4.5V */
            if (TimeMS > 30)
            {
                LCD_PinsInit(); /**< Initialize LCD pins */
                InitState = LCD_FUNCTIONAL_SET;
                TimeMS = 0;
            }
            break;
        
        case LCD_FUNCTIONAL_SET:
            /* Send Function Set Command for 4-bit or 8-bit mode */
            for (uint8_t LCD_Name = 0; LCD_Name < _LCD_NUMBER; LCD_Name++)
            {
                if (LCDs[LCD_Name].LCD_DataLength == EIGHT_BIT_MODE)
                {
                    LCD_SendCommand(LCD_Name, LCD_8BIT_MODE); /**< Send 8-bit mode command */
                }
                else
                {
                    if (TimeMS <= LCD_4BIT_SEND_DATA_COMND_REQ_TIME)
                    {
                        /* Send function set command (Need Enhancement) */
                        LCD_SendCommand(LCD_Name, 0x02);
                    }
                    else
                    {
                        LCD_SendCommand(LCD_Name, LCD_4BIT_MODE); /**< Send 4-bit mode command */
                    }
                }
            }
            
            if ((TimeMS > LCD_4BIT_FUN_SET_REQ_TIME) && (LCD_Command_DataState[0] == LCD_SEND_COMMAND_DATA_READY))
            {
                InitState = LCD_DISPLAY_SETTINGS;
                TimeMS = 0;
            }
            break;

        case LCD_DISPLAY_SETTINGS:
            /* Send Display ON/OFF Control Command */
            for (uint8_t LCD_Name = 0; LCD_Name < _LCD_NUMBER; LCD_Name++)
            {
                LCD_SendCommand(LCD_Name, LCD_DISPLAY_CURSOR_BLINK_ALL_ON);
            }

            if ((TimeMS > LCD_4BIT_SEND_DATA_COMND_REQ_TIME) && (LCD_Command_DataState[0] == LCD_SEND_COMMAND_DATA_READY))
            {       
                InitState = LCD_DISPLAY_CLEAR;
                TimeMS = 0;
            }
            break;

        case LCD_DISPLAY_CLEAR:
            /* Send Clear Display Command */
            for (uint8_t LCD_Name = 0; LCD_Name < _LCD_NUMBER; LCD_Name++)
            {
                LCD_SendCommand(LCD_Name, LCD_CLEAR_DISPLAY);
            }
            
            if ((TimeMS > LCD_4BIT_SEND_DATA_COMND_REQ_TIME) && (LCD_Command_DataState[0] == LCD_SEND_COMMAND_DATA_READY))
            {
                InitState = LCD_ENTRY_MODE;
                TimeMS = 0;
            }    
            break;

        case LCD_ENTRY_MODE:
            /* Send Entry Mode Command */
            for (uint8_t LCD_Name = 0; LCD_Name < _LCD_NUMBER; LCD_Name++)
            {
                LCD_SendCommand(LCD_Name, LCD_ENTRY_MODE_INC_SHIFT_OFF);
            }

            if ((TimeMS > LCD_4BIT_SEND_DATA_COMND_REQ_TIME) && (LCD_Command_DataState[0] == LCD_SEND_COMMAND_DATA_READY))
            {
                InitState = LCD_END;
                TimeMS = 0;
            }    
            break;

        case LCD_END:
            /* End of LCD Initialization */
            for (uint8_t LCD_ID = 0; LCD_ID < _LCD_NUMBER; LCD_ID++) 
            {
                LCD_State[LCD_ID] = LCD_STATE_OPERATION; /**< Transition LCD state to operation */
            }
            break;
        
        default: /* Nothing to do but for MISRA */
            break;
    }
}






/**
 * @brief Initializes the GPIO pins used for controlling the LCD.
 * 
 * This function configures the GPIO pins associated with the LCD control signals
 * such as Register Select (RS), Enable (EN), and data bus pins (DB0-DB7).
 * The configuration settings for each pin are set according to the specified
 * mode (8-bit or 4-bit) of operation for each LCD .
 */
static void LCD_PinsInit(void)
{
    ErrorStatus_t ReturnError;                  
    GPIO_CFG_t LCD_Pin;                        /**< Structure to hold GPIO configuration              */
    LCD_Pin.GPIO_AF = GPIO_AF_DISABLED;        /**< Configure GPIO Alternate Function as disabled      */
    LCD_Pin.GPIO_Mode = GPIO_OUT_PP_NO_PUPD;   /**< Configure GPIO as output with no pull-up/pull-down */
    LCD_Pin.GPIO_Speed = GPIO_LOW_SPEED;       /**< Configure GPIO speed as low speed                   */
    uint8_t LCD_pins = 0;                      /**< Variable to hold the number of LCD data pins (8-bit or 4-bit mode)  */

    /* Loop through each LCD device */
    for (uint16_t LCD_Num = 0; LCD_Num < _LCD_NUMBER; LCD_Num++)
    {
        /* Determine the number of LCD data pins based on the mode of operation */
        /* Remove comment after test please*/
        LCD_pins = (LCDs[LCD_Num].LCD_DataLength == EIGHT_BIT_MODE) ? EIGHT_BIT_MODE : FOUR_BIT_MODE;
        
        /* Loop through each pin associated with the LCD device */
        for (uint16_t Pin = 0; Pin < LCD_pins ; Pin++)
        {
            /* Configure the GPIO port and pin for the current LCD pin */
            LCD_Pin.GPIO_Port = LCDs[LCD_Num].LCD_Pins[Pin].port;
            LCD_Pin.GPIO_Pin = LCDs[LCD_Num].LCD_Pins[Pin].pin;

            /* Initialize the GPIO pin with the specified configuration */
            ReturnError = GPIO_InitPin(&LCD_Pin);
        }
    }
}

/**
 * @brief Sends a 4-bit data/command to the specified LCD.
 * 
 * This function sends a 4-bit data/command to the specified LCD device.
 * It iterates through the data bus pins (DB0-DB3) associated with the LCD
 * and sets their values according to the provided data/command byte.
 * 
 * @param LCD_Name  the LCD ID.
 * @param DataORcommand The 4-bit data/command to be sent to the LCD.
 */
static void LCD_Send4Bit(LCD_Num_t LCD_Name, uint8_t DataORcommand)
{
    ErrorStatus_t ReturnError;     /**< Variable to hold the status of GPIO pin value setting */
    uint8_t BitPos = 0;            /**< Variable to track the bit position in the data/command byte */

    /* Iterate through the 4-bit data bus */
    for (uint8_t DataBus = DB4; DataBus <= DB7; DataBus++)
    {
        /* Set the value of the current data bus pin based on the corresponding bit in the data/command byte */
        if (READ_BIT(DataORcommand, BitPos++))
        {
            ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[DataBus].port, LCDs[LCD_Name].LCD_Pins[DataBus].pin, GPIO_SET_PIN_HIGH);
        }
        else
        {
            ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[DataBus].port, LCDs[LCD_Name].LCD_Pins[DataBus].pin, GPIO_SET_PIN_LOW);
        }
    }
}

/**
 * @brief Sends an 8-bit data/command to the specified LCD.
 * 
 * This function sends an 8-bit data/command to the specified LCD device.
 * It iterates through the data bus pins (DB0-DB7) associated with the LCD
 * and sets their values according to the provided data/command byte.
 * 
 * @param LCD_Name The identifier of the target LCD device.
 * @param DataORcommand The 8-bit data/command to be sent to the LCD.
 */
static void LCD_Send8Bit(LCD_Num_t LCD_Name, uint8_t DataORcommand)
{
    ErrorStatus_t ReturnError;     /**< Variable to hold the status of GPIO pin value setting */
    uint8_t BitPos = 0;            /**< Variable to track the bit position in the data/command byte */

    /* Iterate through the 8-bit data bus */
    for (uint8_t DataBus = DB0; DataBus <= DB7; DataBus++)
    {
        /* Set the value of the current data bus pin based on the corresponding bit in the data/command byte */
        if (READ_BIT(DataORcommand, BitPos++))
        {
            ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[DataBus].port, LCDs[LCD_Name].LCD_Pins[DataBus].pin, GPIO_SET_PIN_HIGH);
        }
        else
        {
            ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[DataBus].port, LCDs[LCD_Name].LCD_Pins[DataBus].pin, GPIO_SET_PIN_LOW);
        }
    }
}


/**
 * @brief Sends a command to the specified LCD device.
 * 
 * This function sends a command to the specified LCD device using the configured GPIO pins.
 * It sets the Register Select pin to LOW (indicating command mode), sends the command  to
 * the LCD data bus pins (either 4-bit or 8-bit mode), and toggles the Enable signal to initiate
 * the command transmission. The function handles the necessary state transitions based on the
 * current LCD command/data state.
 * 
 * @param LCD_Name The LCD ID.
 * @param command  The command to be sent to the LCD.
 */
static void LCD_SendCommand(LCD_Num_t LCD_Name, uint8_t command)
{
    ErrorStatus_t ReturnError;     

    /* Verify if the LCD identifier is within the valid range */
    if (LCD_Name >= _LCD_NUMBER)
    {
        ReturnError = NOK;  /**< Return error if the LCD identifier is invalid */
    }
    else
    {
        /* Reset Register Select Pin to LOW (Command mode) */
        ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[RegisterSelect].port,
                                        LCDs[LCD_Name].LCD_Pins[RegisterSelect].pin, GPIO_SET_PIN_LOW);

        /* Send command to LCD data bus pins depending on 4-bits or 8-bits mode */
        switch (LCD_Command_DataState[LCD_Name])
        {
            case LCD_SEND_COMMAND_DATA_READY:
                if (LCDs[LCD_Name].LCD_DataLength == FOUR_BIT_MODE)
                {
                    LCD_Send4Bit(LCD_Name, command >> 4);  /**< Send upper nibble in 4-bit mode */
                }
                else
                {
                    LCD_Send8Bit(LCD_Name, command);  /**< Send full byte in 8-bit mode */
                }
                /* Set Enable Signal to HIGH */
                ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,
                                                LCDs[LCD_Name].LCD_Pins[Enable].pin, GPIO_SET_PIN_HIGH);
                LCD_Command_DataState[LCD_Name] = LCD_RESET_ENABLE;
                break;

            case LCD_RESET_ENABLE:
                /* Reset Enable Signal to LOW */
                ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,
                                                LCDs[LCD_Name].LCD_Pins[Enable].pin, GPIO_SET_PIN_LOW);
                /* Update LCD command state based on data length */
                LCD_Command_DataState[LCD_Name] = (LCDs->LCD_DataLength == FOUR_BIT_MODE) ? 
                                                    LCD_SEND_COMMAND_DATA_4BIT : LCD_SEND_COMMAND_DATA_READY;
                break;

            case LCD_SEND_COMMAND_DATA_4BIT:
                LCD_Send4Bit(LCD_Name, command);  /**< Send lower nibble in 4-bit mode */
                /* Set Enable Signal to HIGH */
                ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,
                                                LCDs[LCD_Name].LCD_Pins[Enable].pin, GPIO_SET_PIN_HIGH);
                LCD_Command_DataState[LCD_Name] = LCD_RESET_ENABLE_4BIT;
                break;

            case LCD_RESET_ENABLE_4BIT:
                /* Reset Enable Signal to LOW */
                ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,
                                                LCDs[LCD_Name].LCD_Pins[Enable].pin, GPIO_SET_PIN_LOW);
                LCD_Command_DataState[LCD_Name] = LCD_SEND_COMMAND_DATA_IDLE;
                break;

            default:
                /* No action needed but for MISRA */
                break;
        }
    }
}


/**
 * @brief Sends data to the specified LCD device.
 * 
 * This function sends data to the specified LCD device using the configured GPIO pins.
 * It sets the Register Select pin to HIGH (indicating data mode), sends the data to
 * the LCD data bus pins (either 4-bit or 8-bit mode), and toggles the Enable signal to
 * initiate the data transmission. The function handles the necessary state transitions
 * based on the current LCD data state.
 * 
 * @param LCD_Name The LCD ID.
 * @param Data The data byte to be sent to the LCD.
 */
static void LCD_SendData(LCD_Num_t LCD_Name, uint8_t Data)
{
    ErrorStatus_t ReturnError;     

    /* Verify if the LCD ID is within the valid range */
    if (LCD_Name >= _LCD_NUMBER)
    {
        ReturnError = NOK;  /**< Return error if the LCD identifier is invalid */
    }
    else
    {
        /* Set Register Select Pin to HIGH (Data mode) */
        ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[RegisterSelect].port,
                                        LCDs[LCD_Name].LCD_Pins[RegisterSelect].pin, GPIO_SET_PIN_HIGH);

        /* Send data to LCD data bus pins depending on 4-bits or 8-bits mode */
        switch (LCD_Command_DataState[LCD_Name])
        {
            case LCD_SEND_COMMAND_DATA_READY:
                if (LCDs[LCD_Name].LCD_DataLength == FOUR_BIT_MODE)
                {
                    LCD_Send4Bit(LCD_Name, Data >> 4);  /**< Send upper nibble in 4-bit mode */
                }
                else
                {
                    LCD_Send8Bit(LCD_Name, Data);  /**< Send full byte in 8-bit mode */
                }
                /* Set Enable Signal to HIGH */
                ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,
                                                LCDs[LCD_Name].LCD_Pins[Enable].pin, GPIO_SET_PIN_HIGH);
                LCD_Command_DataState[LCD_Name] = LCD_RESET_ENABLE;
                break;

            case LCD_RESET_ENABLE:
                /* Reset Enable Signal to LOW */
                ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,
                                                LCDs[LCD_Name].LCD_Pins[Enable].pin, GPIO_SET_PIN_LOW);
                /* Update LCD command/data state based on data length */
                LCD_Command_DataState[LCD_Name] = (LCDs->LCD_DataLength == FOUR_BIT_MODE) ? 
                                                    LCD_SEND_COMMAND_DATA_4BIT : LCD_SEND_COMMAND_DATA_READY;
                break;

            case LCD_SEND_COMMAND_DATA_4BIT:
                LCD_Send4Bit(LCD_Name, Data);  /**< Send lower nibble in 4-bit mode */
                /* Set Enable Signal to HIGH */
                ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,
                                                LCDs[LCD_Name].LCD_Pins[Enable].pin, GPIO_SET_PIN_HIGH);
                LCD_Command_DataState[LCD_Name] = LCD_RESET_ENABLE_4BIT;
                break;

            case LCD_RESET_ENABLE_4BIT:
                /* Reset Enable Signal to LOW */
                ReturnError = GPIO_SetPinValue(LCDs[LCD_Name].LCD_Pins[Enable].port,
                                                LCDs[LCD_Name].LCD_Pins[Enable].pin, GPIO_SET_PIN_LOW);
                LCD_Command_DataState[LCD_Name] = LCD_SEND_COMMAND_DATA_READY;
                break;

            default:
                /* No action needed but for MISRA */
                break;
        }
    }
}


/* ----------------------------------------------------------------------------------*/
/*                                  LCD Processes                                    */
/*-----------------------------------------------------------------------------------*/

/**
 * @brief Processes the write request for the specified LCD.
 * 
 * This function processes the write request for the specified LCD by sending data characters
 * one by one to the LCD display. It retrieves the string and its length from the user request
 * structure. It sends each character to the LCD using the LCD_SendData function and increments
 * the current position index. Once all characters are sent, it sets the LCD state to ready.
 * 
 * @param LCD_Name The ID of the LCD for which the write request is being processed.
 */
static void LCD_WriteProcess(uint8_t LCD_Name)
{
    uint8_t length=Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].StringLength;
    uint8_t* string=Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].String;

    LCD_SendData(LCD_Name,string[currentPos]);
    if(LCD_Command_DataState[LCD_Name]==LCD_SEND_COMMAND_DATA_READY)
    {
        currentPos++;
    }
    if((LCD_Command_DataState[LCD_Name]==LCD_SEND_COMMAND_DATA_READY)&&(currentPos==length))
    {
       Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].LCD_State=LCD_READY;
        currentPos=0;
        /* check if the Buffer is Full, Circulate it again to position 0 */
        if(DoneUsrReqIdx==(BUFFER_SIZE-1))
        {
            DoneUsrReqIdx=0;
        }
        else
        {
            /* Else, Update the Request Index of the next Request should be to be handled */
            DoneUsrReqIdx++;
        }
    }
}

/**
 * @brief Processes the clear request for the specified LCD.
 * 
 * This function processes the clear request for the specified LCD by sending the clear display
 * command to the LCD. It checks if the command data state is ready before setting the LCD state
 * to ready.
 * 
 * @param LCD_Name The index of the LCD for which the clear request is being processed.
 */

static void LCD_ClearProcess(uint8_t LCD_Name)
{
    /* Send clear display command to the LCD */
    LCD_SendCommand(LCD_Name, LCD_CLEAR_DISPLAY);

    /* If command data state is ready, set LCD state to ready */
    if (LCD_Command_DataState[LCD_Name] == LCD_SEND_COMMAND_DATA_READY)
    {
        Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].LCD_State=LCD_READY; /**< Set LCD state to ready */
        
        /* check if the Buffer is Full, Circulate it again to position 0 */
        if(DoneUsrReqIdx==(BUFFER_SIZE-1))
        {
            DoneUsrReqIdx=0;
        }
        else
        {
            /* Else, Update the Request Index (Which points to the Done Request) to handle the next User Request */
            DoneUsrReqIdx++;
        }
    }
}

/**
 * @brief Processes the set position request for the specified LCD.
 * 
 * This function processes the set position request for the specified LCD by calculating
 * the cursor position based on the given row and column indices. It determines the start
 * address of the DDRAM based on the row index. Then it calculates the cursor position
 * by adding the column index to the start address. Once the cursor position is determined,
 * it sends the command to set the cursor position to the LCD and sets the LCD state to ready.
 * 
 * @param LCD_Name The index of the LCD for which the set position request is being processed.
 */

static void LCD_SetPosProcess(uint8_t LCD_Name)
{
    uint8_t row = Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].PosX ; /**< Row index for the cursor position    */
    uint8_t colm =Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].PosY ; /**< Column index for the cursor position */
    uint8_t CursorPos = 0; /**< Cursor position */

    /* Calculate cursor position based on row and column indices */
    if (row == 0)
    {
        /* First row */
        CursorPos = LCD_DDRAM_START_ADDRESS + colm;
    }
    else
    {
        /* Second row */
        CursorPos = LCD_DDRAM_START_ADDRESS + 0x40 + colm;
    }

    /* Send command to set cursor position to the LCD */
    LCD_SendCommand(LCD_Name, CursorPos);

    /* If command data state is ready, set LCD state to ready */
    if (LCD_Command_DataState[LCD_Name] == LCD_SEND_COMMAND_DATA_READY)
    {
        Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].LCD_State = LCD_READY; /**< Set LCD state to ready */
         
        /* check if the Buffer is Full, Circulate it again to position 0 */
        if(DoneUsrReqIdx==(BUFFER_SIZE-1))
        {
            DoneUsrReqIdx=0;
        }
        else
        {
            /* Else, Update the Request Index of the next Request should be to be handled */
            DoneUsrReqIdx++;
        }
    }
}


/********************************************************************************************************/
/*                                      APIs Implementation                                             */
/********************************************************************************************************/

/**
 * @brief Initializes the LCD with the specified name.
 * 
 * This function initializes the LCD with the specified name by setting its state to initialization.
 * 
 * @param LCD_Name The index of the LCD to be initialized.
 */
void LCD_Init(uint8_t LCD_Name)
{
    LCD_State[LCD_Name] = LCD_STATE_INIT; /**< Set LCD state to initialization */
    for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        Buffer[LCD_Name].userRequestBuffer[i].LCD_State=LCD_READY;
    }
    
}

/**
 * @brief Gets the state of the specified LCD.
 * 
 * This function retrieves the state of the specified LCD. If the LCD is ready and in operation state,
 * it returns LCD_READY, otherwise, it returns LCD_BUSY.
 * 
 * @param LCD_Name The LCD ID .
 * @return The state of the LCD (LCD_READY if ready and in operation state, LCD_BUSY otherwise).
 */
uint8_t LCD_GetState(uint8_t LCD_Name)
{
    uint8_t State = 0; 

    /* Determine the state of the LCD */
    State = (( Buffer[LCD_Name].userRequestBuffer[DoneUsrReqIdx].LCD_State == LCD_READY) && (LCD_State[LCD_Name] == LCD_STATE_OPERATION)) ? LCD_READY : LCD_BUSY;

    return State; /**< Return the state of the LCD */
}

/**
 * @brief Clears the LCD asynchronously for the specified LCD.
 * 
 * This function initiates an asynchronous request to clear the screen for the specified LCD.
 * It sets the LCD state to busy, sets the request type to clear, and assigns the callback function.
 * 
 * @param LCD_Name The ID of the LCD for which the screen is to be cleared.
 * @param CB The callback function to be executed after the request is completed.
 */
void LCD_ClearScreenAsynch(uint8_t LCD_Name, ReqCallBack_t CB)
{
    if (Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].LCD_State == LCD_READY)
    {
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].LCD_State = LCD_BUSY;        /**< Set LCD state to busy        */
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].RequestType = LCD_REQ_CLEAR; /**< Set request type to clear    */
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].CallBack = CB;               /**< Assign the callback function */

        /* check if the Buffer is Full, Circulate it again to position 0 */
        if(UsrReqBufferIdx==(BUFFER_SIZE-1))
        {
            UsrReqBufferIdx=0;
        }
        else
        {
        /* Else, Update the UserRequest Index of the next Request should be Saved */
            UsrReqBufferIdx++;
        }
    }
    
}

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
ErrorStatus_t LCD_WriteStringAsynch(uint8_t LCD_Name, char_t* string, uint8_t length, ReqCallBack_t CB)
{
    ErrorStatus_t ReturnError = NOK; 
    
    /* Check for null pointer */
    if (string == NULL)
    {
        ReturnError = NULL_POINTER;
    }
    /* Check string length */
    else if (length > 16)
    {
        ReturnError = WRONG_PARAMETER;
    }
    /* Check LCD state and user request state */
    else if (Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].LCD_State == LCD_READY)
    {
        /* Set string, string length, request type, LCD state to busy, and assign the callback function */
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].String = string;
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].StringLength = length;
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].RequestType = LCD_REQ_WRITE;
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].LCD_State = LCD_BUSY;
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].CallBack = CB;
       
        /* check if the Buffer is Full, Circulate it again to position 0 */
        if(UsrReqBufferIdx==(BUFFER_SIZE-1))
        {
            UsrReqBufferIdx=0;
        }
        else
        {
        /* Else, Update the UserRequest Index of the next Request should be Saved */
            UsrReqBufferIdx++;
        }
        
        ReturnError = OK; /**< Set return error to OK */
    }
    else
    {
        /* Do nothing (MISRA compliance) */
    }
    
    return ReturnError; /**< Return the error status */
}

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
ErrorStatus_t LCD_SetCursorPosAsynch(uint8_t LCD_Name, uint8_t PosX, uint8_t PosY, ReqCallBack_t CB)
{
    ErrorStatus_t ReturnError = NOK; /**< Error status */

    /* Check for maximum X position */
    if (PosX > 1)
    {
        ReturnError = WRONG_PARAMETER;
    }
    /* Check for maximum Y position */
    else if (PosY > 15)
    {
        ReturnError = WRONG_PARAMETER;
    }
    /* Check if LCD is ready */
    else if (Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].LCD_State == LCD_READY)
    {
        /* Set request type, LCD state, position coordinates, callback function */
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].RequestType = LCD_REQ_SET_POS;
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].LCD_State = LCD_BUSY;
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].PosX = PosX;
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].PosY = PosY;
        Buffer[LCD_Name].userRequestBuffer[UsrReqBufferIdx].CallBack = CB;
         /* check if the Buffer is Full, Circulate it again to position 0 */
        if(UsrReqBufferIdx==(BUFFER_SIZE-1))
        {
            UsrReqBufferIdx=0;
        }
        else
        {
        /* Else, Update the UserRequest Index of the next Request should be Saved */
            UsrReqBufferIdx++;
        }
        ReturnError = OK; 
    }
    else
    {
        /* Do nothing (MISRA Rule) */
    }

    return ReturnError; 
}
