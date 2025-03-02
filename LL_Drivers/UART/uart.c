#include "uart.h"
// #include "uart_public.h"

int uart_write(char* data, int size) {
    int count = size;
    while (count--) {
        while (!(USART2->SR & USART_SR_TXE));  // Wait for TXE instead of TC
        USART2->DR = *data++;  // Send the data byte
    }
    
    // Ensure the last byte completes fully before returning
    while (!(USART2->SR & USART_SR_TC));  // Wait for TC after the last byte
    return size;
}

int uart_write_dma(char* data, int size)
{
    // Ensure DMA is disabled before configuration
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;

    // Set the number of data items to transfer
    DMA1_Stream6->NDTR = size;

    // Set the memory address (data source)
    DMA1_Stream6->M0AR = (uint32_t)data;

    // Clear any pending interrupt flags before enabling
    DMA1->HIFCR |= DMA_HIFCR_CTCIF6;  

    // Enable USART2 DMA transmission
    USART2->CR3 |= USART_CR3_DMAT;

    // Enable the DMA Stream
    DMA1_Stream6->CR |= DMA_SxCR_EN;

    return size;
}


int init_uart2()
{
    int err_code = 0;
    
    //Set PD5 and PD6 in ALternate Function Mode.
    GPIOD->MODER &= ~(GPIO_MODER_MODER5_1 | GPIO_MODER_MODER5_0);
    GPIOD->MODER |= (GPIO_MODER_MODER5_1);

    GPIOD->MODER &= ~(GPIO_MODER_MODER6_1 | GPIO_MODER_MODER6_0);
    GPIOD->MODER |= (GPIO_MODER_MODER6_1);

    //Alternate Function Mux
    GPIOD->AFR[0] |= (7<<(4*UART2_TX_PIN )); 
    GPIOD->AFR[0] |= (7<<(4*UART2_RX_PIN )); 
    
    //SPEED VERY HIGH SPEED
    //GPIOD->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5);
    //GPIOD->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5);

    //GPIOD->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6);
    //GPIOD->OSPEEDR |= (GPIO_OSPEEDR_OSPEED6);

    //USART BRR for 9600 Baud rate = 0x683. fclk = 16 MHz
    //USART2->BRR = 0x683; 
    
    // Set USART2 baud rate to 115200
    USART2->BRR = 0x8B;  // Calculated for 16 MHz clock and 115200 baud rate

    //USART2 TX,RX and UART Enable
    USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);
   
    // Enable DMA mode for USART2 TX
    USART2->CR3 |= USART_CR3_DMAT;
    
    // Enable DMA mode for USART2 RX
    USART2->CR3 |= USART_CR3_DMAR;

    return err_code;
}

/**
 * @brief Uart Initialization
 * 
 * @param uart_cfg 
 * @return int 
 */
int UART_Init(uart_config_t *uart_cfg)
{
    int err_code = 0;

    // Enable GPIO and USART peripheral clocks for USART2 and USART3
    if (uart_cfg->usart == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // Enable USART2 clock
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;   // Enable GPIOD clock for PD5 and PD6 (USART2 pins)
    } else if (uart_cfg->usart == USART3) {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;  // Enable USART3 clock
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;   // Enable GPIOB clock for PB10 and PB11 (USART3 pins)
    } else {
        return -1; // Unsupported USART peripheral
    }

    // Configure TX and RX pins for the selected USART
    if (uart_cfg->usart == USART2) 
    {
        uart_cfg->port->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6);
        uart_cfg->port->MODER |= (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1);       // Alternate function mode
        uart_cfg->port->AFR[0] |= (UART_AF_USART2 << (4 * uart_cfg->tx_pin_num));   // PD5 -> TX
        uart_cfg->port->AFR[0] |= (UART_AF_USART2 << (4 * uart_cfg->rx_pin_num));   // PD6 -> RX
    } 
    else if (uart_cfg->usart == USART3) 
    {
        uart_cfg->port->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11);
        uart_cfg->port->MODER |= (GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1);             // Alternate function mode
        uart_cfg->port->AFR[1] |= (UART_AF_USART3 << (4 * (uart_cfg->tx_pin_num - 8)));     // PB10 -> TX
        uart_cfg->port->AFR[1] |= (UART_AF_USART3 << (4 * (uart_cfg->rx_pin_num - 8)));     // PB11 -> RX
    }

    // Configure USART Baud Rate (assuming APB1 clock is 16 MHz)
    uint32_t fclk = CLK_FREQ_APB1_MHZ * 1000000;  // Assuming 16 MHz clock for APB1
    uint32_t baud_rate = uart_cfg->baudRate;
    uart_cfg->usart->BRR = (fclk + (baud_rate / 2)) / baud_rate;  // Calculate BRR

    // Enable USART (Receiver, Transmitter, UART enable)
    uart_cfg->usart->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);

    // Enable DMA (if enabled in the configuration)
    if (uart_cfg->usart == USART2)
    {
        /* DMA Enabling for Data Logging */
        if (uart_cfg->dma_uart_rx.isEnabled) {
            uart_cfg->usart->CR3 |= USART_CR3_DMAR;     
            DMA1_Stream5->CR |= DMA_SxCR_EN;            // Enable DMA for RX Data Logging
        }
        if (uart_cfg->dma_uart_tx.isEnabled) {
            uart_cfg->usart->CR3 |= USART_CR3_DMAT;     
            DMA1_Stream6->CR |= DMA_SxCR_EN;            // Enable DMA for TX Data Logging
        }
    }
    if (uart_cfg->usart == USART3)
    {
        /* DMA Enabling for Data Logging */
        if (uart_cfg->dma_uart_rx.isEnabled) {
            uart_cfg->usart->CR3 |= USART_CR3_DMAR;     
            DMA1_Stream1->CR |= DMA_SxCR_EN;            // Enable DMA for RX LiDAR Sensor
        }
        if (uart_cfg->dma_uart_tx.isEnabled) {
            uart_cfg->usart->CR3 |= USART_CR3_DMAT;    
            DMA1_Stream3->CR |= DMA_SxCR_EN;            // Enable DMA for TX LiDAR Sensor
        }
    }

    return err_code;
}

/**
 * @brief: UART TX using DMA having data and data size.
 * 
 * @param uart_cfg 
 * @param data 
 * @param size 
 * @return int 
 */
int uart_tx_dma(uart_config_t *uart_cfg, char *data, int size) 
{
    // Ensure DMA is disabled before configuration
    DMA_Stream_TypeDef *dma_stream = (DMA_Stream_TypeDef *)uart_cfg->dma_uart_tx.stream;

    if (dma_stream->CR & DMA_SxCR_EN) 
    {
        // DMA is currently enabled; disable it first
        dma_stream->CR &= ~DMA_SxCR_EN;
        while (dma_stream->CR & DMA_SxCR_EN); // Wait for it to disable
    }

    // Set the number of data items to transfer
    dma_stream->NDTR = size;

    // Set the memory address (data source)
    dma_stream->M0AR = (uint32_t)data;

    // Configure the DMA channel
    dma_stream->CR = (dma_stream->CR & ~DMA_SxCR_CHSEL) | (uart_cfg->dma_uart_tx.channel << DMA_SxCR_CHSEL_Pos);

    // Clear any pending interrupt flags
    if (uart_cfg->dma_uart_tx.stream < DMA2_BASE)
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6; // For DMA1 streams
    else
        DMA2->HIFCR |= DMA_HIFCR_CTCIF6; // For DMA2 streams

    // Enable DMA transmission for the specified USART
    uart_cfg->usart->CR3 |= USART_CR3_DMAT;

    // Enable the DMA Stream
    dma_stream->CR |= DMA_SxCR_EN;

    return size;
}

/**
 * @brief Clear the USART OverRun Error
 * 
 * @param uart_cfg 
 * @return true 
 * @return false 
 */
bool clearUsartOverRunErr(uart_config_t *uart_cfg)
{
    bool returnStatus = 0;  //Error Not detected

    if(uart_cfg->usart->SR & USART_SR_ORE)
    {
        // If USART Overrun Err happens
        
        (void)uart_cfg->usart->SR;
        (void)uart_cfg->usart->DR;

        returnStatus = 1; //Error Detected and Cleared
    }

    return returnStatus;
}

/**
 * @brief 
 * 
 * @param uart_cfg 
 */
void enableUsartDmaTransmission(uart_config_t *uart_cfg)
{
    // Enable USART DMA transmission mode
    uart_cfg->usart->CR3 |= USART_CR3_DMAT;
}

/**
 * @brief 
 * 
 * @param uart_cfg 
 */
void enableUsartDmaReception(uart_config_t *uart_cfg)
{
    // Enable USART DMA transmission mode
    uart_cfg->usart->CR3 |= USART_CR3_DMAR;
}