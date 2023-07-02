#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

#define ADC_MAX            1024
#define AREF_MILLIVOLTS    1100

void adc_init(void);
uint32_t adc_measure_millivolts(void);

#endif