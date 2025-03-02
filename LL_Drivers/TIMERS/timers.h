#ifndef __TIMERS_H__
#define  __TIMERS_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"
#include <stdbool.h>
#include "stateMachineCtrl_public.h"

// Define the timer structure
typedef struct {
    volatile uint32_t timerIsrTick;
    sProcess process;
} sTimer_t;


/* Function Prototype */
void TIM2_IRQHandler(void);

#endif // __TIMERS_H__


