#include "adc.h"

static uint16_t adc_read(void);

void adc_init()
{
    ADCSRA |= (1 << ADEN);                                      // enable ADC
    ADCSRA |= ((1 << ADPS0) | (1 <<  ADPS1) | (1 << ADPS2));    // set prescaler 128
    ADMUX |= (1 << REFS0);                                      // select 1.1 V internal reference source
    
    ADMUX |= (1 << MUX0);                                       // set ADC1 (PB2) as ADC input

    ADCSRA |= (1 << ADSC);                                      // start first conversion
}

uint32_t adc_measure_millivolts()
{
    uint16_t value_read = adc_read();
    return ((uint32_t) value_read * AREF_MILLIVOLTS) / ADC_MAX;
}

static uint16_t adc_read()
{
    uint16_t adc_result = ADC;  // get result for 10-bit resolution
    ADCSRA |= (1 << ADSC);      // start next conversion
    return adc_result;
}