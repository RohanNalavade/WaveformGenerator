#include "dma.h"
#include "dma_public.h"

void dma1Stream6Init(void)
{
    // Old Transmission completed.
    setUartLoggingTxTransferCompleteFlag(true);

    // Enable DMA1 Stream6 interrupt
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);  

    // Set priority for DMA interrupt (you can adjust the priority as needed)
    NVIC_SetPriority(DMA1_Stream6_IRQn, 0);  // Highest priority, for example

    // Enable DMA1 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    // Disable DMA1 Stream6 before configuring
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;  

    // Clear all flags for stream 6
    DMA1->HIFCR |= (DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | 
                    DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6 | 
                    DMA_HIFCR_CFEIF6);

    // Set the peripheral address (USART2->DR)
    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;

    // Configure DMA1 Stream6 for UART2 TX
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;  // Disable stream for configuration
    DMA1_Stream6->CR = (3 << DMA_SxCR_PL_Pos) |       // High priority
                       (4 << DMA_SxCR_CHSEL_Pos) |    // Channel 4 for USART2-TX
                       DMA_SxCR_MINC |                  // Memory increment
                       DMA_SxCR_TCIE |                // Transfer complete interrupt
                       DMA_SxCR_DIR_0;                // Memory to Peripheral

    // Enable USART2 DMA transmission mode
    USART2->CR3 |= USART_CR3_DMAT;

}


/**
 * @brief DMA ISR for USART2-TX i.e. Logging UART data to the console/PC.
 * 
 */
void DMA1_Stream6_IRQHandler(void)
{

    // Check for transfer complete flag
    if (DMA1->HISR & DMA_HISR_TCIF6)
    {
        // Clear the transfer complete flag
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6;

        // Transmission completed.
        setUartLoggingTxTransferCompleteFlag(true);
        
        // Optionally disable the DMA stream after completion
        DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    }
}

