#include "dac.h"

dac_dev_t dacDevices[MAX_DAC_DEVICES] = {
    {
        .cfg = {
            DAC_PORT_CW_SIGNAL_GEN, 
            DAC_CHANEL_CW_SIGNAL_GEN, 
            DAC_GPIO_PORT_CW,
            DAC_GPIO_PIN_CW,
            dacDevice1_Callback
        },
        .dacValue = 0,                   // Initialize DAC value to 0
        .isDacUpdated = false            
    }
};

/**
 * @brief Callback Function
 * 
 */
void dacDevice1_Callback(uint16_t value)
{
    // Example: Store the DAC value or trigger some action
    dacDevices[DAC_DEVICE_1].dacValue = value;
}

/**
 * @brief DAC Initialization
 * 
 */
void dacInitDevices(void) 
{
    for (int i = 0; i < MAX_DAC_DEVICES; i++) 
    {
        dac_dev_t *device = &dacDevices[i];
        dac_config_t *config = &(device->cfg);

        // Enable DAC clock
        RCC->APB1ENR |= RCC_APB1ENR_DACEN;

        // Configure GPIO for DAC Output
        DAC_GPIO_Init(config->gpioPort, config->gpioPin);

        // Enable DAC channel
        if (config->channel == DAC_CHANNEL_1) 
        {
            config->dac->CR |= DAC_CR_EN1;
        }
        else if (config->channel == DAC_CHANNEL_2) 
        {
            config->dac->CR |= DAC_CR_EN2;
        }

        // Set default value (optional)
        device->dacValue = 0;
        device->isDacUpdated = false;

        // Write initial value to DAC
        dacWriteValue(device, 0);
    }
}

void DAC_GPIO_Init(GPIO_TypeDef *GPIOx, uint8_t pin) 
{
    // Enable clock for GPIO port
    if (GPIOx == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (GPIOx == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (GPIOx == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else;

    // Set pin to analog mode (MODER = 11)
    GPIOx->MODER |= (3 << (pin * 2));

    // Disable pull-up/pull-down resistors (PUPDR = 00)
    GPIOx->PUPDR &= ~(3 << (pin * 2));
}

/**
 * @brief Function to Update/Write new value into the DAC
 * 
 * @param device 
 * @param value 
 */
void dacWriteValue(dac_dev_t *device, uint16_t value) 
{
    if (value > 4095) value = 4095;  // 12-bit DAC max value

    device->dacValue = value;
    device->isDacUpdated = true;

    if (device->cfg.channel == DAC_CHANNEL_1) 
    {
        device->cfg.dac->DHR12R1 = value;  // Write value to DAC1 12-bit right-aligned
    } 
    else if (device->cfg.channel == DAC_CHANNEL_2) 
    {
        device->cfg.dac->DHR12R2 = value;  // Write value to DAC2 12-bit right-aligned
    }
    else;

    // Trigger callback if available
    if (device->cfg.callback) 
    {
        device->cfg.callback(value);
    }
}

/**
 * @brief Set the Value To DAC Device1 object
 * 
 * @param value 
 */
void setValueToDacDevice1(uint16_t value)
{
    dacWriteValue(&dacDevices[DAC_DEVICE_1], value);
} 