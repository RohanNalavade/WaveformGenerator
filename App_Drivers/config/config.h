#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "stm32f207xx.h"
#define CONCAT(a, b) a##b

//CLOCK FREQ
#define CLK_FREQ_APB1_MHZ       (16)

// Define GPIOB pin mappings for our LED and button.
#define BUTTON_PIN      (13) //PC13
#define LED_PIN         (7) //PB7

//UART Defines (DATA LOGGING)
#define UART2_TX_PIN            (5) //PD5
#define UART2_RX_PIN            (6) //PD6
#define UART2_PORT              (GPIOD)
#define UART2_BAUDRATE          (115200)
#define UART2_DMA_TX_STREAM     (DMA1_Stream6)
#define UART2_DMA_RX_STREAM     (DMA1_Stream5)
#define UART2_DMA_CHANNEL       (4)

// Buffer size for formatted output
#define PRINTF_BUFFER_SIZE      (512)
#define UART_BUFFER_SIZE   32

//TIMERS
#define SYSTICK_TIMER       (TIM2)

//-----------ADC Pins----------------//

//ADC Pin for Signal Aquazition PA6->ADC1_IN6
#define ADC_GPIO_PIN_CW                    (6)
#define ADC_GPIO_PORT_CW                   (GPIOA)              
#define ADC_PORT_CW_SIGNAL_AQUAZITION      (ADC1)
#define ADC_CHANNEL_CW_SIGNAL_AQUAZITION   (6)

//ADC Pin for Changing The Frequency - Potentiometer PA7->ADC2_IN7
#define ADC_GPIO_PIN_POT                    (7)
#define ADC_GPIO_PORT_POT                   (GPIOA)  
#define ADC_PORT_POT_SIGNAL_AQUAZITION      (ADC2)
#define ADC_CHANNEL_POT_SIGNAL_AQUAZITION   (7)

//-----------------------------------//


#endif //__CONFIG_H__