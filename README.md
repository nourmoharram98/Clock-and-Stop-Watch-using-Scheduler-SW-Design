# Clock-and-Stop-Watch-using-Scheduler-SW-Design

This project aims to create an application for a real-time clock along with a stopwatch using a Scheduler Software Design approach. The core concept involves two microcontrollers communicating via UART (Universal Asynchronous Receiver-Transmitter) communication protocol, each connected to a group of switches and a character LCD. The LCD displays either the clock or stopwatch application, with one microcontroller controlling the LCD of the other through its switches.

## Project Description

The project consists of two STM32F401cc (black pill) microcontrollers, two 16x2 character LCDs, and switches. Microcontroller 1 controls the LCD of Microcontroller 2 through its switches, and vice versa. Communication between the microcontrollers occurs over UART.

## Hardware Components

1. Two STM32F401cc (black pill) microcontrollers
2. Two Character LCD 16x2
3. Switches

## MCAL (Microcontroller Abstraction Layer) Peripherals Used

1. RCC (Reset and Clock Control)
2. GPIO (General-Purpose Input/Output)
3. NVIC (Nested Vector Interrupt Controller)
4. Systick

## Service Layer

A scheduler is implemented to manage and execute various tasks within the system. The scheduler periodically checks for runnables and ensures the timely execution of tasks. The following functionalities are handled by the service layer:

1. Run the clock application
2. Run the stopwatch application
3. Send commands to the other microcontroller
4. Receive commands from the other microcontroller
5. Read the status of switches
6. Perform switch de-bouncing

## How to Use

1. Connect the hardware components as per the specified configuration.
2. Ensure that both microcontrollers are powered and properly configured for UART communication.
3. Compile and upload the firmware to both microcontrollers.
4. Upon startup, the system will initialize and display the clock or stopwatch application on the respective LCD.
5. Use the switches on one microcontroller to control the LCD display and functionalities of the other microcontroller.
6. Commands can be sent and received over UART to trigger specific actions or exchange data between the microcontrollers.

## Contributors

- Aly Mahmoud
- Karim Salah
- Nour Moharram
- Yara Al-Saied