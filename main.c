#include <avr/io.h>
#include <util/delay.h>

#include "common.h"
#include "adc.h"
#include "oled.h"
#include "temperature.h"



int main()
{
    oled_init();
    adc_init(ADC_PIN);
    
    oled_clear();
    while(1)
    {
        uint16_t millivolts = adc_measure_millivolts();
        int32_t temperature = calculate_temperature(millivolts);

        //int32_t temperature = 155;
        //temperature *= -1;

        oled_print_temperature(temperature);
        _delay_ms(UPDATE_DELAY_MS);
        oled_clear_temperature();
    }

    return 0;
}