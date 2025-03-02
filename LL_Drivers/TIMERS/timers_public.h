#ifndef __TIMERS_PUBLIC_H__
#define __TIMERS_PUBLIC_H__
#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"
#include <stdbool.h>



void configure_timer(TIM_TypeDef* timer);
void usr_delay_mS(uint32_t period_mS);
void timerProcessInit();
uint64_t getTimerProcessSystemTick();
void TIM4_PWM_Init(void);
void SetServoAngle(uint16_t angle);
#endif //__TIMERS_PUBLIC_H__