#ifndef HAL_SWITCH_SCHEDULER_SWITCHSCHEDCFG_H_
#define HAL_SWITCH_SCHEDULER_SWITCHSCHEDCFG_H_


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
 * You should puts your Switches Here
        typedef enum
        {
            SWITCH_1,
            SWITCH_2,
            ALARM_SW,
            _SWITCHS_NUM
        }SW_NUM_t;
*/
typedef enum
{
    MODE_OK_SWITCH,
    UP_SWITCH,
    EDIT_SWITCH,
    RIGHT_START_STOP_SWITCH,
    LEFT_RESET_SWITCH,
    DOWN_PAUSE_CONTINUE_SWITCH,
    //HSWITCH_1,
    _HSWITCHES_NUM /*Don't change the position of this Enumerator*/
}HSwitch_NUM_t;






#endif // HAL_SWITCH_SCHEDULER_SWITCHSCHEDCFG_H_