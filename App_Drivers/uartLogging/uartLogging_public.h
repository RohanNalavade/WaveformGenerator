#ifndef _UARTLOGGING_PUBLIC_H__
#define _UARTLOGGING_PUBLIC_H__
#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"

#define LOGGING_RX_BUFFER_SIZE      (16)

bool getUartLoggingTxTransferCompleteFlag(void);
bool getUartLoggingRxTransferCompleteFlag(void);
void setUartLoggingTxTransferCompleteFlag(bool value);
void setUartLoggingRxTransferCompleteFlag(bool value);
void uart_printf(const char* format, ...);
void clearRxLoggingOverRunErr(void);
uint8_t* getLoggingDevRxBufferAddress(void);
void uartLoggingInit(void);
#endif //_UARTLOGGING_PUBLIC_H__