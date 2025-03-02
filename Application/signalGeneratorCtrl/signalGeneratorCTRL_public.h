#ifndef __SIGNAL_GENERATOR_CTRL_PUBLIC_H__
#define __SIGNAL_GENERATOR_CTRL_PUBLIC_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>



void signalGeneratorCTRLProcessInit();
void signalGeneratorCTRLProcess();
void incrementContinusWaveformFrequency();
#endif //__SIGNAL_GENERATOR_CTRL_PUBLIC_H__