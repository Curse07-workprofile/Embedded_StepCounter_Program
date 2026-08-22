#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "uart.h"
#include "usart.h"
#include "gpio.h"
#include "app.h"

void uartSerialOutput(FSM_t* fsm_memory, Step_Values_t* step_memory)
{
	char buffer[62];
	snprintf(buffer, sizeof(buffer), "Steps=%d, Goal=%d, CurrentMode=%d, CurrentState=%d\r\n",
			step_memory->step_count, step_memory->step_goal, fsm_memory->current_mode,
			fsm_memory->current_display);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 10);
}
