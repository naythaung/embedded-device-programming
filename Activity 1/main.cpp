/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "platform/mbed_thread.h"

//LEDs
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

//Button
InterruptIn button(USER_BUTTON);

//Interrupt variables
Timeout button_debounce_timeout;
float debounce_time_interval = 0.3;

//Array recording inputs
int a[5];
int i = 0;

//LED cycle variable
int t = 0;

//Create onButtonStopDebouncing so that it can be used in onButtonPress
void onButtonStopDebouncing(void);

//Interrupt on button press
void onButtonPress(void) {
    a[i] = t;
    i++;
    button.rise(NULL);
    button_debounce_timeout.attach(onButtonStopDebouncing, debounce_time_interval);
}

//Reattachment of onButtonPress interrupt handler
void onButtonStopDebouncing(void)
{
    button.rise(onButtonPress);
}

//Selection of which LED to cycle to
void cycle_led(int l) {
    if (l==0) {
        led1 = true;
        led2 = false;
        led3 = false;
    } else if (l==1) {
        led1 = false;
        led2 = true;
        led3 = false;
    } else if (l==2) {
        led1 = false;
        led2 = false;
        led3 = true;
    }
}

//Turn of all LEDs
void turn_off_all_leds() {
    led1 = false;
    led2 = false;
    led3 = false;
}

//Main functionality - Iterating t + Output of LED inputs
int main() {
    while(i<5) {
        cycle_led(t);
        button.rise(onButtonPress);
        wait(1);
        t = (t+1)%3;
    }
    for (int i = 0; i < 5; i++) {
        cycle_led(a[i]);
        wait(1);
    }
    turn_off_all_leds();
}