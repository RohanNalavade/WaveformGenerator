#ifndef __DAC_H__
#define __DAC_H__

#include <stdint.h>
#include <stdio.h>
#include "stm32f207xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "config.h"

typedef enum {
    DAC_CHANNEL_1 = 1,
    DAC_CHANNEL_2,
    MAX_DAC_CHANNELS
} dacChannel_t;

typedef enum {
    DAC_DEVICE_1 = 0,
    MAX_DAC_DEVICES
} dacDeviceIndex_t;

typedef struct {
    DAC_TypeDef *dac;           // DAC peripheral (DAC1, etc.)
    uint8_t channel;            // DAC channel number (1 or 2)
    GPIO_TypeDef *gpioPort;     // GPIO Port (e.g., GPIOA)
    uint8_t gpioPin;            // GPIO Pin number (e.g., 4 for PA4)
    void (*callback)(uint16_t);         // Optional callback function
} dac_config_t;

/**
 * DAC device descriptor
 */
typedef struct {
    dac_config_t cfg;              // DAC driver configuration
    uint16_t dacValue;             // Last written DAC value
    bool isDacUpdated;             // Flag indicating if the DAC was updated
} dac_dev_t;

void dacDevice1_Callback(uint16_t value);
void DAC_GPIO_Init(GPIO_TypeDef *GPIOx, uint8_t pin);
void dacInitDevices(void);
void dacWriteValue(dac_dev_t *device, uint16_t value);
void setValueToDacDevice1(uint16_t value);
#endif //__DAC_H__