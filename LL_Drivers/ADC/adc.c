#include "adc.h"

adc_dev_t adcDevices[MAX_ADC_DEVICES] = {
    {
        {
            ADC_PORT_CW_SIGNAL_AQUAZITION, 
            ADC_CHANNEL_CW_SIGNAL_AQUAZITION, 
            RANK_1, 
            1, 
            adcDevice1_Callback, 
            ADC_GPIO_PORT_CW,
            ADC_GPIO_PIN_CW 
        }
    },
    {
        {
            ADC_PORT_POT_SIGNAL_AQUAZITION, 
            ADC_CHANNEL_POT_SIGNAL_AQUAZITION, 
            RANK_1, 
            1, 
            adcDevice2_Callback,
            ADC_GPIO_PORT_POT, 
            ADC_GPIO_PIN_POT
        }
    }
};

/**
 * @brief 
 * 
 */
void adcInitDevices(void) 
{
    for (int i = 0; i < MAX_ADC_DEVICES; i++)
    {
        adc_dev_t *device = &adcDevices[i];
        adc_config_t *config = &(device->cfg);

        // Enable ADC clock
        if (config->adc == ADC1) RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
        else if (config->adc == ADC2) RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
        else if (config->adc == ADC3) RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;

        // Configure corresponding GPIO pin for analog input
        ADC_GPIO_Init(config->gpioPort, config->gpioPin);

        // Configure channel and rank
        config->adc->SQR3 |= (config->channel << (5 * (config->rank - 1)));

        // Set sampling time
        if (config->channel <= 9) {
            config->adc->SMPR2 |= (config->samplingTime << (3 * config->channel));
        } else {
            config->adc->SMPR1 |= (config->samplingTime << (3 * (config->channel - 10)));
        }

        
        // Enable ADC
        config->adc->CR2 |= ADC_CR2_ADON;

        // Start conversion
        config->adc->CR2 |= ADC_CR2_SWSTART;

        // Enable end-of-conversion interrupt
        config->adc->CR1 |= ADC_CR1_EOCIE;

        device->isAdcConversionCompleted = false;
    }

    // Enable ADC IRQ in NVIC
    NVIC_EnableIRQ(ADC_IRQn);
}

/**
 * @brief: Call Back Function for ADC Device 1
 * 
 * @param value 
 */
void adcDevice1_Callback(uint16_t value) 
{
    adcDevices[ADC_DEVICE_1].adcRawValue = value;
}

/**
 * @brief: Call Back Function for ADC Device 2
 * 
 * @param value 
 */
void adcDevice2_Callback(uint16_t value) 
{
    adcDevices[ADC_DEVICE_2].adcRawValue = value;
}

/**
 * @brief Making the GPIO of the Pin as Analog Input
 * 
 * @param GPIOx 
 * @param pin 
 */
void ADC_GPIO_Init(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    // Enable clock for GPIO port
    if (GPIOx == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (GPIOx == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (GPIOx == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else if (GPIOx == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    else if (GPIOx == GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    else if (GPIOx == GPIOF) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;

    // Set pin to analog mode (MODER = 11)
    GPIOx->MODER |= (3 << (pin * 2));

    // Disable pull-up/pull-down resistors (PUPDR = 00)
    // GPIOx->PUPDR &= ~(3 << (pin * 2));
    GPIOx->PUPDR |= (1 << (2 * pin)); // Enable pull-up resistor

}

/**
 * @brief ADC IRQ GlobalHandler for all ADC1, ADC2 and ADC3
 * 
 */
void ADC_IRQHandler(void)
{
    for (int i = 0; i < MAX_ADC_DEVICES; i++)
    {
        adc_dev_t *device = &adcDevices[i];
        adc_config_t *config = &(device->cfg);

        // Check if the interrupt is from this ADC
        if (config->adc->SR & ADC_SR_EOC)
        {
            uint16_t adcValue = config->adc->DR;  // Read ADC result (clears EOC flag)

            // Call the registered callback function if set
            if (config->callback)
            {
                config->callback(adcValue);
            }

            // config->adc->CR2 |= ADC_CR2_SWSTART;  // Restart conversion manually
            device->isAdcConversionCompleted = true;
        }
    }
}

/**
 * @brief Start the ADC Conversion for the adcDevice
 * 
 * @param adcDeviceIndex 
 * @return uint16_t 
 */
void startAdcConversion()
{
    // if(adcDevices[ADC_DEVICE_1].isAdcConversionCompleted || adcDevices[ADC_DEVICE_2].isAdcConversionCompleted)
    if(1)
    {
        adcDevices[ADC_DEVICE_1].cfg.adc->CR2 |= ADC_CR2_SWSTART;  // Restart conversion manually
        adcDevices[ADC_DEVICE_2].cfg.adc->CR2 |= ADC_CR2_SWSTART;  // Restart conversion manually
        
        adcDevices[ADC_DEVICE_1].isAdcConversionCompleted = false;
        adcDevices[ADC_DEVICE_2].isAdcConversionCompleted = false;
        
    }
}

/**
 * @brief Helper Function to get ADC pin Voltage
 * 
 * @param adcDeviceIndex 
 */
uint16_t getADCpinVoltage(adcDeviceIndex_t adcDeviceIndex)
{
    return adcDevices[adcDeviceIndex].adcRawValue;
}