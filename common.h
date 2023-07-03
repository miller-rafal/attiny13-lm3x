
/*
    Sensor type (centigrade or fahrenheit)
*/

#define LM35
//#define LM34  // uncomment this line and comment out the line above if you're using an LM34 fahrenheit sensor

/*
    Sensor configuration
*/

//#define FULL_RANGE // uncomment this line if you're using your sensor in full-range configuration (see datasheet)

/*
    General definitions
*/

#define ADC_PIN           PB1   // analog input pin
#define UPDATE_DELAY_MS   3000  // delay (in milliseconds) between consecutive measurements and display updates