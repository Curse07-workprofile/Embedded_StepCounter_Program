# STM32 Step-Counter — Embedded Systems Project
 
This step-counter program is based around an STM32C071 microcontroller and the custom RCAP extension (designed and made by the University of Canterbury).
The program features an interrupt-driven kernel with cooperative task scheduling. 
The device tracks steps, goal progress, and distance travelled, with a full peripheral setup for input and output functionality.

## Resources of Project

- [Full Design Report](./StepcounterReport.pdf) — detailed write-up of architecture, sub-systems, and design decisions
- [Source Code (.c)](./Code/Core/Src/Source_code.md) — Our code written source files
- [Source Code (.h)](./Code/Core/Inc/Include_code.md) — Our code written header files
- [Hardware Documentation](./Hardware_documentation/HardwareDocLinks.md) — component user guides and manuals

## Features
 
- **Live step tracking** via an LSM6DS IMU (SPI), triggered by interrupt on step detection
- **Adjustable step goal**, set via joystick (click) + potentiometer (adjustable dial where analog input is read via DMA)
- **Three display modes** (current steps, goal progress, distance travelled) navigated by a joystick-driven GUI FSM on an OLED I2C display
- **RGB LED progress indicator**, scaling brightness/colour with goal completion
- **Audio feedback** via a piezoelectric buzzer, including a distinct "goal reached" tune generated from a custom note/tune library
- **UART serial output** for robust debugging/data logging, togglable by a button
- **Built-in test mode** for simulating step input without physical movement

## Architecture General Description
 
The firmware is structured as a layered, modular application on top of the STM32 HAL:
 
- **Application layer** — owns task scheduling, module configuration, and shared data structures; has no direct hardware access, keeping business logic decoupled from drivers
- **Driver modules** — one module per peripheral (IMU, display, LEDs, buzzer, joystick/potentiometer, UART), each independently testable and portable
- **HAL abstraction** — hardware-specific operations utilize the STM HAL interface, making the program portable to other STM target boards with minimal modification
 
## Skills Demonstrated
 
- **Embedded C** development on ARM Cortex-M0+ (STM32 Nucleo; STM32C071)
- Interrupt-driven kernel design and **race condition mitigation**
- Custom implementation of **cooperative task scheduler** design (no RTOS)
- Peripheral driver development with use of: **SPI, UART, DMA, PWM/Timers, ADC**
- Sensor integration (**IMU** step-count internal algorithm)
- **Modular software architecture** — decoupled, testable, portable driver layers
- Mealy Finite state machine design (GUI navigation)
- Power/performance-aware design (conditional task execution)
- Technical documentation and design explainations

---
*Developed as a university Embedded Systems project in pair collaboration with Andy Armour and myself (Curtis Christian).*
