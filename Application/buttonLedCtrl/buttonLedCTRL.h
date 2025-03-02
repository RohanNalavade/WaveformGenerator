#ifndef __BUTTONLEDCTRL_H__
#define __BUTTONLEDCTRL_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "config.h"
#include "stateMachineCtrl_public.h"

#include "adc.h"
#include "dac.h"

#define DEBOUCE_TIME_BUTTON_PRESS_IN_UNITS_100MSEC   (2)    //200 msec

typedef struct{
    sProcess process;
    sStability buttonPressed;
} sButtonLedCtrl;

//buttonLedCTRL structure instance
sButtonLedCtrl appButtonLedCtrl;



#endif //__BUTTONLEDCTRL_H__