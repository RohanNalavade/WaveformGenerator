#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "config.h"

typedef enum {
    ADC_DEVICE_1 = 0,   // ADC for Waveform Data Aquazition
    ADC_DEVICE_2,       // ADC for Potentiometer - Not Used
    MAX_ADC_DEVICES
} adcDeviceIndex_t;

typedef enum {
    CHANNEL_0 = 0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,
    CHANNEL_7,
    CHANNEL_8,
    CHANNEL_9,
    CHANNEL_10,
    CHANNEL_11,
    CHANNEL_12,
    CHANNEL_13,
    CHANNEL_14,
    CHANNEL_15,
    MAX_ADC_CHANNELS
} adcChannelIndex_t;

typedef enum {
    RANK_1 = 1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    RANK_10,
    RANK_11,
    RANK_12,
    RANK_13,
    RANK_14,
    RANK_15,
    MAX_ADC_CH_RANK
} adcChannelRank_t;

typedef struct {
    ADC_TypeDef *adc;           // ADC peripheral (ADC1, ADC2, etc.)
    uint8_t channel;            // ADC channel number (e.g., 1 for ADC1_CHANNEL_1)
    uint8_t rank;               // Channel rank in sequence
    uint32_t samplingTime;      // Sampling time for the channel
    void (*callback)(uint16_t); // Callback function for the channel
    GPIO_TypeDef *gpioPort;     // GPIO Port (GPIOA, GPIOB, etc.)
    uint8_t gpioPin;            // GPIO Pin number
} adc_config_t;

/**
 * ADC device descriptor
 */
typedef struct
{
    adc_config_t cfg;               //< ADC driver configuration
    uint16_t adcRawValue;           // ADCRaw Value
    bool isAdcConversionCompleted;  //Flag will become 1 when the ADC Conversion is Completed
} adc_dev_t;


/* Variable Decleration */
extern adc_dev_t adcDevices[MAX_ADC_DEVICES];


/* Function Decleration */
void adcDevice1_Callback(uint16_t value);
void adcDevice2_Callback(uint16_t value);
void adcInitDevices(void);
void ADC_GPIO_Init(GPIO_TypeDef *GPIOx, uint8_t pin);
void ADC_IRQHandler(void);
void ADC_GPIO_Init(GPIO_TypeDef *GPIOx, uint8_t pin);
uint16_t getADCpinVoltage(adcDeviceIndex_t adcDeviceIndex);
void startAdcConversion();
#endif //__ADC_H__