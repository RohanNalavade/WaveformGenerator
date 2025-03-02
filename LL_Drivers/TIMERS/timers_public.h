#ifndef __TIMERS_PUBLIC_H__
#define __TIMERS_PUBLIC_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"
#include <stdbool.h>


void configure_timer(TIM_TypeDef* timer, uint32_t frequency);
uint64_t getTimerProcessSystemTick();
uint64_t getTimer3Tick();

#endif //__TIMERS_PUBLIC_H__