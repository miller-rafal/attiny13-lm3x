#include "adc.h"

static uint16_t adc_read(void);

void adc_init(uint8_t adc_pin)
{
    ADCSRA |= (1 << ADEN);                                      // enable ADC
    ADCSRA |= ((1 << ADPS0) | (1 <<  ADPS1) | (1 << ADPS2));    // set prescaler 128
    ADMUX |= (1 << REFS0);                                      // select 1.1 V internal reference source
    
    switch(adc_pin)
    {
        case PB0:
            break;
        case PB1:
            ADMUX |= (1 << MUX0);
            break;
        case PB2:
            ADMUX |= (1 << MUX1);
            break;
        case PB3:
            ADMUX |= ((1 << MUX0) | (1 << MUX1));
            break;
        default:
            break;
    }

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