/*******************************************************************************************/
/* Author       : Karim Salah Fathey                                                       */
/* Authority    : Information Technology Institute                                         */
/* Layer        : Service Abstraction Layer (SAL)                                          */
/* Module       : OS Scheduler                                                             */
/* Version      : v1.1                                                                     */
/* Date         : 11/03/2024                                                               */
/* File Details : Configuration file for Scheduler module                                  */
/* Target       : STM32f401cc may be comptaeble with any m3/m4 target                      */
/*******************************************************************************************/
#pragma once
/**********************************  Definition **********************************************/
#define TICK_TIME_MS 1

/**********************************   Runnable names ************************************************/

/**
 * @brief Enumeration for defining the order and priority of RunnableTasks.
 * 
 * This enumeration is used to specify the order and priority of RunnableTasks.
 * The order of enumeration members indicates the priority of tasks, with lower
 * values representing higher priority.
 */
typedef enum
{
    Clock_Date,
    StopWatch,
    Manager,
    Control_Switch,
    Switch_Debouncing,
    LCD,
    _Runnable_no
} RunnablePriority_t;

/*********************************** Runnable prototypers ******************************************/
extern void Clock_Date_Runnable        (void);
extern void Stopwatch_Runnable         (void);
extern void Application_Runnable       (void);
extern void LCD_Runnable_Manager       (void);
extern void Switch_Debouncing_Runnable  (void);
extern void ControlSwitches_Runnable    (void);