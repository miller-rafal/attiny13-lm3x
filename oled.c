#include "oled.h"

#define SCL_LOW()     IIC_DDR |=  (1 << SCL_PIN)
#define SCL_HIGH()    IIC_DDR &= ~(1 << SCL_PIN)
#define SDA_LOW()     IIC_DDR |=  (1 << SDA_PIN)
#define SDA_HIGH()    IIC_DDR &= ~(1 << SDA_PIN)

#define OLED_CMD_MODE   0x00
#define OLED_DAT_MODE   0x40

static void oled_print_minus(void);
static void oled_print_degree_symbol(void);
static void oled_print_unit(void);
static void oled_print_digit(uint8_t digit);
static void iic_init(void);
static void iic_start(uint8_t);
static void iic_stop(void);
static void iic_write(uint8_t);

static const uint8_t digits[] PROGMEM =
{
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
};

static const uint8_t minus[] PROGMEM =
{
      0x08, 0x08, 0x08, 0x08, 0x08 // -
};

// TODO - change to hex:
static const uint8_t degree_symbol[] PROGMEM =
{
    0b00000000,
    0b00000110,
    0b00001001,
    0b00001001,
    0b00000110
};

#if defined(LM35)
    static const uint8_t unit[] PROGMEM =
    {
        0x3E, 0x41, 0x41, 0x41, 0x22 // C
    };
#elif defined(LM34)
    static const uint8_t unit[] PROGMEM =
    {
        0x7F, 0x09, 0x09, 0x09, 0x01 // F
    };
#endif

void oled_print_temperature(int32_t temperature)
{
    oled_set_cursor(TEMPERATURE_X, TEMPERATURE_Y);

    iic_start(OLED_ADDR);
    iic_write(OLED_DAT_MODE);

    uint8_t mean_value;
    if(temperature < 0)
        mean_value = temperature * -1;
    else
        mean_value = temperature;

    uint8_t modulo;

    uint8_t hundreds = mean_value / 100;
    modulo = mean_value % 100;

    uint8_t tens = modulo / 10;
    modulo = modulo % 10;

    uint8_t ones = modulo;

    /*
        What will happen when a negative temperature value is passed?
    */

    if(temperature < 0)
        oled_print_minus();

    if(hundreds > 0)
        oled_print_digit(hundreds);
    if(tens > 0)
        oled_print_digit(tens);
    oled_print_digit(ones);

    oled_print_degree_symbol();
    oled_print_unit();

    iic_stop();
}

void oled_clear_temperature(void)
{
    oled_set_cursor(TEMPERATURE_X, TEMPERATURE_Y);               

    iic_start(OLED_ADDR);                   
    iic_write(OLED_DAT_MODE);               
    for(uint16_t i = 0; i < 5 * CHAR_W; i++)
        iic_write(0x00);                    
    iic_stop();                             
}

void oled_init()
{
    iic_init();
    iic_start(OLED_ADDR);
    iic_write(OLED_CMD_MODE);
    
    /*
        Sequentially write all initialization commands 
    */

    iic_write(0xA8);
    iic_write(0x1F);
    iic_write(0x22);
    iic_write(0x00);
    iic_write(0x03);
    iic_write(0x20);
    iic_write(0x00);
    iic_write(0xDA);
    iic_write(0x02);
    iic_write(0x8D);
    iic_write(0x14);
    iic_write(0xAF);
    iic_write(0xA1);
    iic_write(0xC8);

    iic_stop();
}

void oled_set_cursor(uint8_t x, uint8_t line)
{
    iic_start(OLED_ADDR);
    iic_write(OLED_CMD_MODE);
    iic_write(x & 0x0F);
    iic_write(0x10 | (x >> 4));
    iic_write(0xB0 | (line & 0x07));
    iic_stop();
}

void oled_clear(void)
{
    oled_set_cursor(0, 0);               

    iic_start(OLED_ADDR);                   
    iic_write(OLED_DAT_MODE);               
    for(uint16_t i = 0; i < 512; i++)
        iic_write(0x00);                    
    iic_stop();                             
}

static void oled_print_minus()
{
    iic_write(0x00);

    for(uint8_t i = 0; i < 5; i++)
        iic_write(pgm_read_byte(&minus[i]));
}

static void oled_print_degree_symbol()
{
    iic_write(0x00);

    for(uint8_t i = 0; i < 5; i++)
        iic_write(pgm_read_byte(&degree_symbol[i]));
}

static void oled_print_unit()
{
    iic_write(0x00);

    for(uint8_t i = 0; i < 5; i++)
        iic_write(pgm_read_byte(&unit[i]));
}

static void oled_print_digit(uint8_t digit)
{
    if(digit >= 10)
        return;

    iic_write(0x00);

    uint8_t offset = digit * 5;
    for(uint8_t i = 0; i < 5; i++)
        iic_write(pgm_read_byte(&digits[offset + i]));
}

static void iic_init()
{
    IIC_DDR  &= ~((1 << SDA_PIN) | (1 << SCL_PIN));
    IIC_PORT &= ~((1 << SDA_PIN) | (1 << SCL_PIN));
}

static void iic_start(uint8_t address)
{
    SDA_LOW();
    SCL_LOW();
    iic_write(address);
}

static void iic_stop()
{
    SDA_LOW();
    SCL_HIGH();
    SDA_HIGH();
}

static void iic_write(uint8_t data)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        SDA_LOW();
        if(data & 0x80)
            SDA_HIGH();
        SCL_HIGH();
        data <<= 1;
        SCL_LOW();
    }
    SDA_HIGH();
    SCL_HIGH();
    asm("nop");
    SCL_LOW();
}