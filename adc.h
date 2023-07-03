#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

#define ADC_MAX            1023
#define AREF_MILLIVOLTS    1100

void adc_init(uint8_t adc_pin);
uint32_t adc_measure_millivolts(void);

#endif