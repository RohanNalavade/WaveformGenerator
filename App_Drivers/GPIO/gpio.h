#ifndef __APP_GPIO_H__
#define __APP_GPIO_H__


#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "config.h"

/* Variables Prototype*/

/* Function Prototype */

int set_gpio(GPIO_TypeDef* gpio_port, int pin);
int reset_gpio(GPIO_TypeDef* gpio_port, int pin);
bool get_gpio_pin_status(GPIO_TypeDef* gpio_port, int pin);



#endif //__APP_GPIO_H__
