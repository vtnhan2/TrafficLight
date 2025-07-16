Overview
This project implements a traffic light control system using a PIC microcontroller with UART communication for configuration. The system supports both automatic timing mode and manual control mode for traffic lights.

Features
Two operating modes:

Automatic counter mode with configurable timing

Manual control mode for direct light control

UART interface for:

Configuring light timing

Switching between modes

Manual control commands

4-digit 7-segment display to show countdown timers

Supports 4-way traffic lights (two crossing directions)

Hardware Requirements
PIC microcontroller (specific model not stated)

7-segment displays (4 digits)

Traffic light LEDs (red, yellow, green for two directions)

UART communication interface (e.g., USB-to-serial converter)

Basic electronic components (resistors, etc.)

Configuration
Oscillator: HS (High Speed) oscillator

Watchdog Timer: Disabled

Power-up Timer: Disabled

Brown-out Reset: Disabled

Low Voltage Programming: Disabled

Memory Protection: All disabled

UART Commands
Mode Selection
M - Show mode selection menu

X - Enter automatic counter mode

O - Enter manual control mode

Automatic Mode
When in automatic mode (X), you will be prompted to enter:

Green light duration (3 digits, in seconds)

Yellow light duration (3 digits, in seconds)

C - Change timing while in automatic mode

Manual Control Mode
D - Set direction 1 & 3 green, direction 2 & 4 red

A - Set direction 2 & 4 green, direction 1 & 3 red

Usage
Connect via serial terminal at 9600 baud

Send M to see mode options

For automatic mode:

Send X

Enter green light duration (3 digits)

Enter yellow light duration (3 digits)

System will automatically cycle through lights

For manual mode:

Send O

Use D and A to control light states

Code Structure
UART functions: Initialization, transmit, receive

7-segment display drivers: Number output and multiplexing

Interrupt handler: Processes UART commands

Main loop: Implements traffic light state machine

Notes
Default oscillator frequency: 20MHz (_XTAL_FREQ 20000000)

The system uses PORTB, PORTC, and PORTD for I/O

7-segment displays are multiplexed with time division

