#include "adc.h"

adc_dev_t adcDevices[MAX_ADC_DEVICES] = {
    {{ADC1, CHANNEL_0, RANK_1, 1, adcDevice1_Callback}}, // ADC1, Channel 0
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

        // Enable end-of-conversion interrupt
        config->adc->CR1 |= ADC_CR1_EOCIE;
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

}

