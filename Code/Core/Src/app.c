// Standard Libraries
#include <stdbool.h>
#include <stdint.h>
// Our Modules
#include "app.h"
#include "adc_interface.h"
#include "button.h"
#include "display.h"
#include "uart.h"
#include "timing.h"
#include "buzzer.h"
#include "step_update.h"
#include "progress_leds.h"

#define NUM_INIT_TASKS 	6
#define NUM_TASKS 		6
#define STEP_INT_PIN 	0x0400

static bool step_flag = false;

void (*constructor[])(void) = {
		initTiming,
		displayInit,
		buttonInit,
		buzzerInit,
		stepUpdateInit,
		progressLEDsInit
};

bool (*taskTiming[])(void) = {
		displayTimingCheckRun,
		uartTimingCheckRun,
		buttonTimingCheckRun,
		adcTimingCheckRun,
		buzzerTimingCheckRun,
		ledTimingCheckRun
};

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin & ((uint16_t)(STEP_INT_PIN)))
	{
		step_flag = true;
	}
}

void app_main(void)
{
	Step_Values_t step_memory = {
			1000,
			0,
			0
	};

	Flags_t sys_flags = {
			{0, 0},
			false,
			true,
			true
	};

	FSM_t fsm_memory = {
			CURRENT_STEPS,
			NORMAL_MODE
	};

	step_flag = false; 	// On restart set interrupt flag to false

	// Initialization functions to improve modularity
	for (size_t i = 0; i < NUM_INIT_TASKS; i++)
	{
		constructor[i]();
	}

	while(1)
	{
		loopTimer(); // 1000 Hz
		// needs to be here once to prevent race conditions
		for (Tasks_t i = DISPLAY; i < NUM_TASKS; i++)
		{
			if (step_flag)	// Step interrupt
			{
				stepUpdateTaskExecute(&step_memory, &step_flag);
				sys_flags.update_display 	= true;
				sys_flags.update_LEDs 		= true;
			}
			if (taskTiming[i]())
			{
				switch (i)
				{
				case DISPLAY:
					displayStatusBehaviour(&fsm_memory, &step_memory, &sys_flags);
					break;
				case UART:
					if (sys_flags.transmit_state)
					{
						uartSerialOutput(&fsm_memory, &step_memory);
					}
					break;
				case BUTTONS:
					buttonInputBehaviour(&fsm_memory, &step_memory, &sys_flags);
					break;
				case ADC_INTERFACE:
					joystickADCBehaviour(&fsm_memory, &step_memory, &sys_flags);
					break;
				case BUZZER:
					buzzerCheckGoal(&step_memory);
					break;
				case LEDS:
					if (sys_flags.update_LEDs)
					{
						sys_flags.update_LEDs = false;
						progressLEDsTaskExecute(&step_memory);
					}
					break;
				}
			}
		}
	}
}
