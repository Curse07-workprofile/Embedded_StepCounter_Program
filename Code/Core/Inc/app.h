#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <stdint.h>

// Main task run file with header structure definitions

typedef enum
{
	DISPLAY = 0,
	UART,
	BUTTONS,
	ADC_INTERFACE,
	BUZZER,
	LEDS
} Tasks_t;

typedef enum {
	CURRENT_STEPS = 0,
	GOAL_PROGRESS,
	DISTANCE_TRAVELLED
} Display_States_t;

typedef enum
{
	NORMAL_MODE = 0,
	TEST_MODE,
	SET_GOAL
} Mode_States_t;

typedef struct
{
	uint16_t step_goal;
	uint16_t step_count;
	uint16_t previous_imu_steps;
} Step_Values_t;

typedef struct
{
	bool units_of_display[2];
	bool transmit_state;
	bool update_display;
	bool update_LEDs;
} Flags_t;

typedef struct
{
	Display_States_t current_display;
	Mode_States_t current_mode;
} FSM_t;


void app_main(void);

#endif
