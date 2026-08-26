#include "mbed.h"
#include "stdint.h" // This allow the use of integers of a known width
#define LM75_REG_TEMP (0x00) // Temperature Register
#define LM75_REG_CONF (0x01) // Configuration Register
#define LM75_ADDR     (0x90) // LM75 address

#define LM75_REG_TOS (0x03) // TOS Register
#define LM75_REG_THYST (0x02) // THYST Register

I2C i2c(I2C_SDA, I2C_SCL);

//LEDs
DigitalOut green_led(LED1);
DigitalOut blue_led(LED2);
DigitalOut red_led(LED3);

InterruptIn lm75_int(D7); // Make sure you have the OS line connected to D7

Serial pc(SERIAL_TX, SERIAL_RX);

float temps[60]; // Stores temperature readings

int16_t i16; // This variable needs to be 16 bits wide for the TOS and THYST conversion to work

Ticker reading_ticker; // Tickers
Ticker led_ticker;

bool led_state = false;

float reading_interval = 1.0;

float current_temp = 0.0;

bool alarm_triggered = false;

void toggle_leds() {
    green_led = led_state;
    blue_led = led_state;
    red_led = led_state;
    led_state = !led_state;
}

// Triggers LED alarm
void alarm_signal() {
    led_ticker.attach(&toggle_leds, 0.5);
}

void above_threshold() {
    alarm_triggered = true; // Triggers threshold alarm
    reading_ticker.detach(); // Stops ticker
    alarm_signal();
}

// Appends temperature readings into temps array, pushing in from the end of the array
void onReadingTicker(void) {
    for (int i = 0 ; i < 59 ; i++) {
        temps[i] = temps[i+1];
    }
    temps[59] = current_temp;
}

int main() {
    char data_write[3];
    char data_read[3];

    /* Configure the Temperature sensor device STLM75:
           - Thermostat mode Interrupt
           - Fault tolerance: 0
           - Interrupt mode means that the line will trigger when you exceed TOS and stay triggered until a register is read - see data sheet
        */
    data_write[0] = LM75_REG_CONF;
    data_write[1] = 0x02;
    int status = i2c.write(LM75_ADDR, data_write, 2, 0);
    if (status != 0) { // Error detection
        while (1) {
            red_led = !red_led;
            wait(0.2);
        }
    }

    float tos = 28; // TOS temperature
    float thyst = 26; // THYST tempertuare

    // This section of code sets the TOS register
    data_write[0]=LM75_REG_TOS;
    i16 = (int16_t)(tos*256) & 0xFF80;
    data_write[1]=(i16 >> 8) & 0xff;
    data_write[2]=i16 & 0xff;
    i2c.write(LM75_ADDR, data_write, 3, 0);

    //This section of codes set the THYST register
    data_write[0]=LM75_REG_THYST;
    i16 = (int16_t)(thyst*256) & 0xFF80;
    data_write[1]=(i16 >> 8) & 0xff;
    data_write[2]=i16 & 0xff;
    i2c.write(LM75_ADDR, data_write, 3, 0);

    // This line attaches the interrupt.
    // The interrupt line is active low so we trigger on a falling edge
    lm75_int.fall(&above_threshold);

    reading_ticker.attach(onReadingTicker, reading_interval);

    while (!alarm_triggered) {
        // Read temperature register
        data_write[0] = LM75_REG_TEMP;
        i2c.write(LM75_ADDR, data_write, 1, 1); // no stop
        i2c.read(LM75_ADDR, data_read, 2, 0);

        // Calculate temperature value in Celcius
        int16_t i16 = (data_read[0] << 8) | data_read[1];
        // Read data as twos complement integer so sign is correct
        current_temp = i16 / 256.0;
    }
    for (int i = 0; i < 60; i++) {
        pc.printf("Temperature = %.3f\r\n",temps[i]); // Transmits temperature history by USB communication
    } 
}