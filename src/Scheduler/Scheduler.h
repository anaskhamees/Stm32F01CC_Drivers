/*
 * File Name: Scheduler.h
 * Layer	: Scheduler
 * Module	: Scheduler
 * Version  : V 0.0
 * Created	:  Mar 13, 2024
 * Author	: Anas Khamees
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Std_Lib/Std_Libraries.h"
#include "SchedulerCFG.h"

/**
 * @brief Type definition for a callback function pointer.
 *        This type is used to declare functions that can be registered as callbacks.
 * @details The callback function must have no parameters and return type `void`.
 */
typedef void (*RunnableCB_t)(void);

/**
 * @brief   Structure representing a user-defined runnable task.
 * @details This structure holds information about a task, including its periodicity,
 *          initial delay, and callback function to execute.
 */
typedef struct 
{
    uint32_t     PeriodicityMS; /**< The periodicity of the task in milliseconds. */
    uint32_t     FirstDelayMS;  /**< The initial delay before the task starts executing, in milliseconds. */
    RunnableCB_t CallBack;      /**< Pointer to the callback function to execute for this task. */
} UserRunnable_t;


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
 * @brief   Initializes the scheduler.
 * @details This function sets up the scheduler by configuring the system tick timer,
 *          setting the callback function for scheduler ticks, and initializing the runnable tasks.
 * @note The scheduler must be initialized before starting it.
 */
void Scheduler_Init(void);
/**
 * @brief   Starts the scheduler.
 * @details This function starts the scheduler by enabling the system tick interrupt
 *          and entering a loop to handle scheduler ticks.
 * @note The scheduler must be initialized before calling this function.
 */
void Scheduler_Start(void);



#endif // SCHEDULER_SCHEDULER_H_