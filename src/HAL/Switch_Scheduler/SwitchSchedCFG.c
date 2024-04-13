/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/GPIO/GPIO.h"
#include "SwitchSched.h"


/********************************************************************************************************/
/************************************************Variables***************************************************/
/********************************************************************************************************/

HSwitchCFG_t HSwitches[_HSWITCHES_NUM]=
{
    /* Switches of Demo1 (StopWatch & Time-Date)*/

    [MODE_OK_SWITCH]=
    {
        .SW_Port=GPIO_PORTC,
        .SW_Pin=GPIO_PIN13,
        .SW_Connection=GPIO_IN_PU
    }
    ,
    [UP_SWITCH]=
    {
        .SW_Port=GPIO_PORTB,
        .SW_Pin=GPIO_PIN8,
        .SW_Connection=GPIO_IN_PU
    },
    [EDIT_SWITCH]=
    {
        .SW_Port=GPIO_PORTB,
        .SW_Pin=GPIO_PIN9,
        .SW_Connection=GPIO_IN_PU
    },
    [RIGHT_START_STOP_SWITCH]=
    {
        .SW_Port=GPIO_PORTB,
        .SW_Pin=GPIO_PIN5,
        .SW_Connection=GPIO_IN_PU
    },
    [LEFT_RESET_SWITCH]=
    {
        .SW_Port=GPIO_PORTB,
        .SW_Pin=GPIO_PIN6,
        .SW_Connection=GPIO_IN_PU
    },
    [DOWN_PAUSE_CONTINUE_SWITCH]=
    {
        .SW_Port=GPIO_PORTB,
        .SW_Pin=GPIO_PIN7,
        .SW_Connection=GPIO_IN_PU
    }

    /*[HSWITCH_1]=
    {
        .SW_Port=GPIO_PORTC,
        .SW_Pin=GPIO_PIN13,
        .SW_Connection=GPIO_IN_PD,
    }*/
};