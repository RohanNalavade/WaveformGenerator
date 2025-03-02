#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "stm32f207xx.h"
#define CONCAT(a, b) a##b

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

//I2C Defines
#define __I2C_2__ I2C2
#define __RCC_APB1ENR_I2C__ CONCAT(RCC_APB1ENR_, CONCAT(__I2C_2__, EN))
#define I2C2_SCL_PIN    (1)
#define I2C2_SDA_PIN    (0)       
#define I2C2_PORT       (GPIOF)
#define CLK_FREQ_APB1_MHZ   (16)
#define ICM20948_I2C_ADDR   (0x69)
#define SYSTICK_TIMER       (TIM2)
//#define __ENABLE_WDT__
#define __DEBUG_UART__
#endif //__CONFIG_H__