#include "init.h"


void clk_init()
{
    /*RCC Clock Setup*/
    // Enable the GPIOB & GPIOC peripheral in 'RCC_AHBENR'.
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

    // USART 2 Pin 'GPIOD' = AHB1, USART2 = APB1 BUS Enable
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIODEN);
    RCC->APB1ENR |= (RCC_APB1ENR_USART2EN);

    //Enable TIM3 in RCC_APB1ENR
    RCC->APB1ENR |= (RCC_APB1ENR_TIM2EN);

    //Enable I2C2 in RCC_APB1ENR and GPIOF in RCC_ANB1ENR
    RCC->APB1ENR |= (RCC_APB1ENR_I2C2EN);
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOFEN);
}

void initializationSystem()
{
    __disable_irq();
    clk_init();
    init_gpio();
    // init_uart2();
    uartLoggingInit();
    dma1Stream6Init();
    
    // TIM4_PWM_Init();


    (void)configure_timer(SYSTICK_TIMER);
    (void)buttonLedCTRLProcessInit();

    adcInitDevices();
    dacInitDevices();
    __enable_irq();  // Enable global interrupts

    
    uart_printf("Program Started%d \r\n", 100);
    
}