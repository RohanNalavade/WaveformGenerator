#ifndef __UARTLOGGING_H__
#define __UARTLOGGING_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"

#include "uart.h"
#include "uart_public.h"


/**
 * @brief  Device Descriptor 
 * 
 */
typedef struct {
    uart_dev_t uartDev;
    uint8_t rx_buffer[LOGGING_RX_BUFFER_SIZE];
} log_dev_t;

/**
 * @brief Instance of uart_log_dev_t
 * 
 */
log_dev_t loggingDev;

#endif //__UARTLOGGING_H__