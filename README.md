# Embedded Device Programming

**University of Cambridge Engineering Tripos**  
**Completed: December 2024**  
**Languages/Tools: C++, ARM MBed OS, I2C Communication**

Embedded C++ programming exercises completed collaboratively as part of University of Cambridge Engineering coursework using an ARM Mbed-enabled microcontroller.

The project explores fundamental embedded systems concepts including digital I/O, hardware interrupts, button debouncing, timers, I2C sensor communication and serial data transfer.

## Activity 1 — LED Sequence Recorder

A user-controlled LED sequence recorder implemented using the microcontroller's onboard LEDs and push button.

The program:

- Cycles between three onboard LEDs
- Records the currently displayed LED when the user presses the button
- Uses hardware interrupts to detect button presses
- Implements button debouncing using a timeout
- Stores the selected sequence in memory
- Plays the recorded sequence back using the LEDs

This activity demonstrates digital I/O, interrupts, timing and basic state handling on a microcontroller.

## Activity 2 — Temperature Monitoring System

A temperature monitoring and alarm system using an LM75 temperature sensor connected over I2C.

The program:

- Communicates with the LM75 sensor using the I2C protocol
- Records a temperature measurement every second
- Maintains a rolling history of the previous 60 readings
- Configures temperature threshold and hysteresis registers
- Uses a hardware interrupt when the threshold is exceeded
- Stops data acquisition when an alarm occurs
- Flashes the onboard LEDs as an alarm signal
- Sends the recorded temperature history to a computer over serial communication

## Embedded Systems Concepts

The project demonstrates:

- C++
- ARM Mbed OS
- Digital input and output
- Hardware interrupts
- Interrupt service routines
- Button debouncing
- Timers and tickers
- I2C communication
- Sensor register configuration
- Serial communication
- Embedded data acquisition

## Hardware

The coursework was developed for an Mbed-compatible STM32 development board, with the second activity using an LM75 temperature sensor.

## Project Structure

- `Activity 1/main.cpp` — LED sequence recorder
- `Activity 1/mbed-os.lib` — Mbed OS dependency
- `Activity 2/main.cpp` — I2C temperature monitoring and alarm system
- `Activity 2/mbed-os.lib` — Mbed OS dependency

## Academic Context

Completed as part of Engineering coursework at the University of Cambridge.

The exercises use the ARM Mbed framework. Existing third-party copyright and licence notices in the source files have been retained.
