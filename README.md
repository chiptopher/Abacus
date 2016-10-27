# Abacus

Abacus is an alarm clock created using the Arduino playform. The primary feature of the Abacus is that when the alarm goes up a math problem displayed on the LCD is required to terminate it.

Components and techniques used to develop the Abacus include:
- Multiplexed 4 Digit 7-Segment Display
- Liquid Crystal Display
- Button Matrix for Input
- State Transition Table for Machine State
- 74HC595 Output Register
- Atmega328p and ATTiny85 microcontroller programming

The libraries were developed in C, but since the Arduino playform required C++ they were converted.

The Atmega328p is prgrammed with the Abacus.ino file and the Attiny85 (which powers the alarm) is programmed with the AbacusAlarm.ino file.

Additionally, the [Time Library] (http://playground.arduino.cc/Code/Time) would be required to compile the program.
