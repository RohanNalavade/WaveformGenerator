#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include "config.h"

#include "gpio_public.h"
#include "timers_public.h"
#include "buttonLedCTRL_public.h"
#include "uartLogging_public.h"


#define DMA_PHERIPHERAL_TO_MEM      (0)
#define DMA_MEM_TO_PHERIPHERAL      (1)

typedef struct {
    uint32_t stream;                    /*!< DMA Stream (e.g., DMA1_Stream2) */
    uint32_t channel;                   /*!< DMA Channel */
    uint32_t direction;                 /*!< Direction: Memory-to-Periph / Periph-to-Memory */
    volatile bool transfer_complete;    /*!< IRQ Flag for transfer completion */
    bool isEnabled;                       /* Status if DMA is enabled or disabled*/
} dma_config_t;



void DMA1_Stream6_IRQHandler(void);

#endif //__DMA_H__