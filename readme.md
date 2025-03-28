# Simple FreeRTOS Monitor System

This project implements a simple monitoring system using FreeRTOS. It is designed to run on the BitDogLab board, demonstrating basic task creation, inter-task communication with queues and semaphores, and GPIO control. This project fulfills the requirements for the EmbarcaTech residency program, specifically the "Sistema de Monitoramento Simples com 3 Tarefas" activity.

## Overview

The system consists of three FreeRTOS tasks:

1.  **Button Read Task:** Reads the state of a button connected to a GPIO pin.
2.  **Button Process Task:** Receives the button state from a queue and determines whether the button is pressed.
3.  **LED Control Task:** Controls an LED connected to a GPIO pin, turning it on or off based on the button state, signaled by a semaphore.

## Hardware

- BitDogLab Board
- Button connected to GPIO pin 5 (PIN_BUTTON_A) - Configured with a pull-up resistor.
- LED connected to GPIO pin 12 (PIN_LED_BLUE)

## Software

- FreeRTOS
- Pico SDK (configured for BitDogLab)

## Tasks

- **Button Read Task:**
  - Reads the state of the button using `gpio_get()`.
  - Sends the button state to a queue (`buttonQueue`).
  - Runs periodically with a delay defined by `BUTTON_READ_DELAY_MS`.
- **Button Process Task:**
  - Receives the button state from the `buttonQueue`.
  - Sets the `ledState` variable based on the button press (active low).
  - Signals the `Led Control Task` using the `ledSemaphore`.
- **LED Control Task:**
  - Waits for a signal from the `ledSemaphore`.
  - Toggles the LED state using `gpio_put()` based on the `ledState` variable.

## Pinout

| Component | BitDogLab Pin (RP2040 GPIO) |
| --------- | --------------------------- |
| Button A  | GPIO 5                      |
| LED       | GPIO 12                     |

## How to Install

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/luisfpatrocinio/freertos-3task-monitor
    ```
2.  **Install on the BitDogLab board:** Follow the BitDogLab documentation for setting up the development environment and flashing the firmware.
3.  **Press Button A to toggle the LED:** Once the program is running, pressing Button A on the BitDogLab board will turn the LED on or off.

## Build Instructions

1.  Ensure you have the Pico SDK and FreeRTOS configured for the BitDogLab board. Refer to the BitDogLab and Raspberry Pi Pico documentation for instructions.
2.  Set the environment variable `FREERTOS_KERNEL_PATH` to the location of your FreeRTOS kernel.
3.  Build the project using CMake.
4.  Flash the generated UF2 file to your BitDogLab board.

## Code Structure

- `main.c`: Contains the main application logic, task definitions, queue and semaphore creation, and GPIO initialization.

## Author

Luis Felipe dos Santos Patrocinio
