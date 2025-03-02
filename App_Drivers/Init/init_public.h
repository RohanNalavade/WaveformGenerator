#ifndef __INIT_PUBLIC_H__
#define __INIT_PUBLIC_H__
#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"
#include "init.h"

void clk_init();
void initializationSystem();
#endif //__PUBLIC_H__