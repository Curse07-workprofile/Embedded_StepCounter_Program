#ifndef UART_H
#define UART_H

#include "app.h"

// UART module for debugging to serial port, speed 115200 bauds

void uartSerialOutput(FSM_t* fsm_memory, Step_Values_t* step_memory);

#endif // UART_H
