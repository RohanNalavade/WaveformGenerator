#ifndef __UART_PUBLIC_H__
#define __UART_PUBLIC_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"

int init_uart2();
int uart_write(char* data, int size );
int uart_write_dma(char* data, int size);

#endif //__UART_PUBLIC_H__