#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"

#include "dma.h"


// Define the GPIO alternate function mappings for each UART port
#define UART_AF_USART1    7
#define UART_AF_USART2    7
#define UART_AF_USART3    7
#define UART_AF_USART6    8

/**
 * @brief UART initialization parameters of master
 */
typedef struct{
    USART_TypeDef *usart;                   /*!< Pointer to the USART peripheral */
    int tx_pin_num;                         /*!< GPIO number for UART TX signal */
    int rx_pin_num;                         /*!< GPIO number for UART RX signal */
    uint32_t baudRate;                      /* BaudRate of UART*/
    GPIO_TypeDef *port;                     /* UART Pins Port Number*/
    dma_config_t dma_uart_rx;               /*!< DMA Configuration for RX */
    dma_config_t dma_uart_tx;               /*!< DMA Configuration for TX */
} uart_config_t;

/**
 * UART device descriptor
 */
typedef struct{
    uart_config_t cfg;        //< UART driver configuration
} uart_dev_t;


/* Function Prototype */
// int uart_write(char* data, int size );
int uart_tx_dma(uart_config_t *uart_cfg, char *data, int size) ;
bool clearUsartOverRunErr(uart_config_t *uart_cfg);
int UART_Init(uart_config_t *uart_cfg);
void enableUsartDmaTransmission(uart_config_t *uart_cfg);
void enableUsartDmaReception(uart_config_t *uart_cfg);

#endif // __UART_H__

