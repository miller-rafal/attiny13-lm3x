#ifndef OLED_H
#define OLED_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#include "common.h"

/*
    Definitions of IIC registers and pins used to drive the OLED display
*/

#define IIC_DDR     DDRB
#define IIC_PORT    PORTB
#define SCL_PIN     PB3
#define SDA_PIN     PB4

#define OLED_ADDR   0x78

/*
    Definitions of screen and character dimensions
*/

#define OLED_WIDTH  128
#define OLED_HEIGHT  64

#define CHAR_W  5
#define CHAR_H  8

/*
    Tweak these definitions to make the temperature display at your desired screen coordinates
*/

#define TEMPERATURE_X   ((OLED_WIDTH / 2) - 3 * CHAR_W)
#define TEMPERATURE_Y   1

/*
    OLED function prototypes
*/

void oled_print_temperature(int32_t temperature);
void oled_clear_temperature(void); // clears only the part of the display on which the temperature is shown (to minimize flickering)

void oled_init(void);
void oled_set_cursor(uint8_t x, uint8_t line);
void oled_clear(void); // clears the whole display

#endif