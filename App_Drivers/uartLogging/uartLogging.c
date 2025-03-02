#include "uartLogging.h"
#include "uartLogging_public.h"

// Global loggingDev instance
log_dev_t loggingDev = {
    .uartDev = {
        .cfg = {
            .usart = USART2,
            .tx_pin_num = UART2_TX_PIN,
            .rx_pin_num = UART2_RX_PIN,
            .baudRate = UART2_BAUDRATE,
            .port = UART2_PORT,
            .dma_uart_rx = {
                .stream = (uint32_t)UART2_DMA_RX_STREAM,
                .channel = UART2_DMA_CHANNEL,
                .direction = DMA_PHERIPHERAL_TO_MEM,
                .transfer_complete = false,
                .isEnabled = true
            },
            .dma_uart_tx = {
                .stream = (uint32_t)UART2_DMA_TX_STREAM,
                .channel = UART2_DMA_CHANNEL,
                .direction = DMA_MEM_TO_PHERIPHERAL,
                .transfer_complete = false,
                .isEnabled = true
            }
        }
    },
    .rx_buffer ={0}
};

/**
 * @brief Set the Uart Logging Tx Transfer Complete Flag
 * 
 * @param value 
 */
void setUartLoggingTxTransferCompleteFlag(bool value)
{
    loggingDev.uartDev.cfg.dma_uart_tx.transfer_complete = value;
}

/**
 * @brief Set the Uart Logging Rx Transfer Complete Flag
 * 
 * @param value 
 */
void setUartLoggingRxTransferCompleteFlag(bool value)
{
    loggingDev.uartDev.cfg.dma_uart_rx.transfer_complete = value;
}

/**
 * @brief Get the Uart Logging Tx Transfer Complete Flag
 * 
 * @return true 
 * @return false 
 */
bool getUartLoggingTxTransferCompleteFlag(void)
{
    return loggingDev.uartDev.cfg.dma_uart_tx.transfer_complete;
}

/**
 * @brief Get the Uart Logging Rx Transfer Complete Flag
 * 
 * @return true 
 * @return false 
 */
bool getUartLoggingRxTransferCompleteFlag(void)
{
    return loggingDev.uartDev.cfg.dma_uart_rx.transfer_complete;
}



/**
 * @brief : Custom printf wrapper for UART
 * 
 * @param format 
 * @param ... 
 */
void uart_printf(const char* format, ...)
{
    char buffer[PRINTF_BUFFER_SIZE];  // Buffer to store the formatted string
    va_list args;
    
    // Start processing the format string and the variable arguments
    va_start(args, format);
    
    // Format the string using vsnprintf
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    // End the argument processing
    va_end(args);
    
    // Enable DMA Stream and mark DMA as busy i.e. Tx is not complited
    setUartLoggingTxTransferCompleteFlag(false);

    // Send the formatted string over UART using _write
    //uart_write_dma(buffer, strlen(buffer));
    uart_tx_dma(&loggingDev.uartDev.cfg, buffer, strlen(buffer));
}

/**
 * @brief: Clears the RX USART LOGGING OVERRUN ERROR, if the RX Speed is too high.
 * 
 */
void clearRxLoggingOverRunErr()
{
    (void)clearUsartOverRunErr(&loggingDev.uartDev.cfg);
}

/**
 * @brief Get the Logging Dev Rx Buffer Address object
 * 
 * @return uint8_t* 
 */
uint8_t* getLoggingDevRxBufferAddress(void)
{
    return loggingDev.rx_buffer;
}

/**
 * @brief: Uart Initialization for uartLogging
 * 
 */
void uartLoggingInit()
{
    UART_Init(&loggingDev.uartDev.cfg);
}