#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdint.h>

#include "common.h"

#if defined(FULL_RANGE)
    #if defined(LM35)
        #define TEMPERATURE_BASE    -55
    #elif defined(LM34)
        #define TEMPERATURE_BASE    -50
    #endif
#else
    #if defined(LM35)
        #define TEMPERATURE_BASE     2
    #elif defined(LM34)
        #define TEMPERATURE_BASE     5
    #endif
#endif

#define MILLIVOLTS_PER_DEGREE   10

int32_t calculate_temperature(uint32_t millivolts);

#endif