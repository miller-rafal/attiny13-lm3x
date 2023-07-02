#include "temperature.h"

int32_t calculate_temperature(uint32_t millivolts)
{
    return (TEMPERATURE_BASE + (millivolts / MILLIVOLTS_PER_DEGREE));
}