#ifndef __SIGNAL_GENERATOR_CTRL_H__
#define __SIGNAL_GENERATOR_CTRL_H__


#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "config.h"
#include "stateMachineCtrl_public.h"

#include "timers_public.h"
#include "dac.h"
#include "adc.h"

#define NS                          (128)
#define TIM3_FREQUENCY_HZ           (TIM3_FREQUENCY_DEFAULT_HZ)
#define TIM3_MAX_FREQUENCY_HZ       (1000)          //CW Signal freq ~ 10Hz (1000/128)
#define TIM3_MIN_FREQUENCY_HZ       (100)             
#define TIM3_INCREMENT_STEP_SIZE    (100)        
#define CW_SIGNAL_FREQUENCY         (TIM3_FREQUENCY_HZ/NS)


uint16_t Wave_LUT[NS] = {
    2048, 2149, 2250, 2350, 2450, 2549, 2646, 2742, 2837, 2929, 3020, 3108, 3193, 3275, 3355,
    3431, 3504, 3574, 3639, 3701, 3759, 3812, 3861, 3906, 3946, 3982, 4013, 4039, 4060, 4076,
    4087, 4094, 4095, 4091, 4082, 4069, 4050, 4026, 3998, 3965, 3927, 3884, 3837, 3786, 3730,
    3671, 3607, 3539, 3468, 3394, 3316, 3235, 3151, 3064, 2975, 2883, 2790, 2695, 2598, 2500,
    2400, 2300, 2199, 2098, 1997, 1896, 1795, 1695, 1595, 1497, 1400, 1305, 1212, 1120, 1031,
    944, 860, 779, 701, 627, 556, 488, 424, 365, 309, 258, 211, 168, 130, 97,
    69, 45, 26, 13, 4, 0, 1, 8, 19, 35, 56, 82, 113, 149, 189,
    234, 283, 336, 394, 456, 521, 591, 664, 740, 820, 902, 987, 1075, 1166, 1258,
    1353, 1449, 1546, 1645, 1745, 1845, 1946, 2047
};

typedef struct{
    uint32_t externalTimerTick;              //TIMER3 TICK
    bool externalTimerCurrentStatusFlag;
    bool externalTimerPreviousStatusFlag;
}extTimingInfo_t;

typedef struct{
    sProcess process;
    extTimingInfo_t extTimingInfo;
    uint16_t currentTimer3OveflowFrequecy;
} signalGenCtrl_t;


signalGenCtrl_t appSignalGenCtrl;

void checkTimer3Update(void);


#endif //__SIGNAL_GENERATOR_CTRL_H__
