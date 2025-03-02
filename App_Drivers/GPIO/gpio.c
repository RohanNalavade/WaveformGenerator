#include "gpio.h"

int init_gpio()
{
    //Set GPIO PC13 as input
    //Set MODER Register to 00
    GPIOC->MODER &= ~(GPIO_MODER_MODER13_0 | GPIO_MODER_MODER13_1);
    
    //Set OTYPER Register to 0 for pushpull 
    //GPIOC->OTYPER &= ~(GPIO_OTYPER_OT13);

    //Set PUPDR to Pull-Up 01
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13_1 | GPIO_PUPDR_PUPD13_0); 
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPD13_1);

    /*Configure PB7 as LED Output*/
    //GPIO Configuration
    //Set MODER
    GPIOB->MODER &= ~(GPIO_MODER_MODER7);
    GPIOB->MODER |= (GPIO_MODER_MODE7_0);


    return 0;
}

int set_gpio(GPIO_TypeDef* gpio_port, int pin)
{
   gpio_port->ODR |= (1 << pin);
   gpio_port->BSRR |= (1 << (pin));
   gpio_port->BSRR &= ~(1 <<(pin+16));
   return 0;    
} 

int reset_gpio(GPIO_TypeDef* gpio_port, int pin)
{
    gpio_port->BSRR &= ~(1 << (pin));
    gpio_port->BSRR |= (1 <<(pin+16));
    return 0;
}

bool get_gpio_pin_status(GPIO_TypeDef* gpio_port, int pin)
{
    bool pin_status = 0;
    pin_status = (((gpio_port->IDR) >> pin) & 0x0001);
    
    return pin_status;
}

