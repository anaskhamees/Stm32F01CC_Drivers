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
    HSWITCH_1,
    _HSWITCHES_NUM /*Don't change the position of this Enumerator*/
}HSwitch_NUM_t;






#endif // HAL_SWITCH_SCHEDULER_SWITCHSCHEDCFG_H_