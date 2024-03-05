/*
 * File Name: LED_CFG.h
 * Layer	: HAL
 * Module	: LED
 * Version  : V 0.0
 * Created	:  Feb 19, 2024
 * Target   : Stm32F401CC (BlackBill)
 * Author	: Anas Khamees
 */

/**
 *@note   : User Should Fill This enum with his Leds
 *@warning: Don't Change The Position of _Led_Number, it must be the last item in the enum.
 * For Example :
            typedef enum{
                Led_Green,
                Led_Red,
                Led_Alarm,
                    .
                    .
                    .
                    .
            _Led_Number
            }Leds_Number_t;
            
*/
typedef enum{


LED1,
_Led_Number
}Leds_Number_t;