#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "adc.h"
#include "number_processing.h"
#include "adc_interface.h"

#include "app.h"

static uint16_t raw_adc[3];

// Call back function

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{

}

// Private Functions

void getRawADCData(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 3);
}

void joystickBehaviour(FSM_t* fsm_memory, Flags_t* sys_flags)
{
	// Get joy-stick position
	Directions_t joystick_dir = NOT_SET;
	getJoystickDir(raw_adc[2], raw_adc[1], &joystick_dir);

	static bool spam_latch = false;

	switch (joystick_dir) // action either change states or units
	{
	case CENTER:
		spam_latch = false;
		break;

	case RIGHT:
		if (!(spam_latch))
		{
			spam_latch = true;
			(fsm_memory->current_display)++;
			if ((fsm_memory->current_display) > DISTANCE_TRAVELLED)
			{
				(fsm_memory->current_display) = CURRENT_STEPS;
			}
			sys_flags->update_display = true;
		}
		break;

	case LEFT:
		if (!(spam_latch))
		{
			spam_latch = true;
			if ((fsm_memory->current_display) != CURRENT_STEPS)
			{
				(fsm_memory->current_display)--;
			}
			else
			{
				(fsm_memory->current_display) = DISTANCE_TRAVELLED;
			}
			sys_flags->update_display = true;
		}
		break;

	case UP:
	case DOWN:
		if (!(spam_latch))
		{
			spam_latch = true;
			switch (fsm_memory->current_display)
			{
			case CURRENT_STEPS:
				(sys_flags->units_of_display[0]) ^= 1;
				sys_flags->update_display = true;
				break;
			case GOAL_PROGRESS:
				break;
			case DISTANCE_TRAVELLED:
				(sys_flags->units_of_display[1]) ^= 1;
				sys_flags->update_display = true;
				break;
			}
		}
		break;
	case NOT_SET:
		break;
	}
}

void testingModeTaskExecute(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags)
{
	Directions_t joystick_dir_x = NOT_SET;
	bool joystick_dir;
	uint8_t percentage = 0;

	getTestingModeJoystick(raw_adc[2], raw_adc[1], &joystick_dir_x, &percentage, &joystick_dir);

	static bool spam_latch = false;
	// do the side switching:

	switch (joystick_dir_x)
	{
	case CENTER:
		spam_latch = false;
		break;

	case RIGHT:
		if (spam_latch == false)
		{
			spam_latch = true;
			if (joystick_dir_x == RIGHT)
			{
				(fsm_memory->current_display)++;
				if ((fsm_memory->current_display) > DISTANCE_TRAVELLED)
				{
					(fsm_memory->current_display) = CURRENT_STEPS;
				}
				sys_flags->update_display = true;
			}
		}
		break;

	case LEFT:
		if (spam_latch == false)
		{
			spam_latch = true;
			if (fsm_memory->current_display != CURRENT_STEPS)
			{
				(fsm_memory->current_display)--;
			}
			else
			{
				(fsm_memory->current_display) = DISTANCE_TRAVELLED;
			}
			sys_flags->update_display = true;
		}
		break;
	case UP:
	case DOWN:
	case NOT_SET:
		break;

	}


	static uint8_t count = 0;


	// get the percentages and work with them:
	if (count >= 4) // 2 Hz value changes (could go faster and decrease the scale
	{
		if (percentage != 0)
		{
			sys_flags->update_display 	= true;
			sys_flags->update_LEDs 		= true;
			uint16_t scaled_increment;
			if ((percentage > 0) && (percentage <= 25))
			{
				scaled_increment = 1;
			}
			else
			{
				scaled_increment = (percentage * (step_memory->step_goal / 1000));
			}
			count = 0;
			if (joystick_dir)
			{
				if ((((step_memory->step_goal - 10) - scaled_increment) >= step_memory->step_count))
				{
					step_memory->step_count += scaled_increment;
				}
				else
				{
					step_memory->step_count = ((step_memory->step_goal) - 10);
				}
			}
			else
			{
				if (scaled_increment <= step_memory->step_count)
				{
					step_memory->step_count -= scaled_increment;
				}
				else
				{
					step_memory->step_count = 0;
				}
			}
		}
	}
	else
	{
		count++;
	}
}



void setGoalTaskExecute(Step_Values_t* step_memory, Flags_t* sys_flags)
{
	static uint8_t count = 0;

	if (count >= 1) // at 5 Hz to prevent display glitches
	{
		count = 0;
		// potentiometer changes step goal
		uint16_t processed_potentiometer = processPotentiometerADC(raw_adc[0]);
		if (step_memory->step_goal != processed_potentiometer)
		{
			step_memory->step_goal = processed_potentiometer;
			sys_flags->update_display = true;
			sys_flags->update_LEDs = true;
		}
	}
	else
	{
		count++;
	}

}

// Public Functions

void joystickADCBehaviour(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags)
{
	getRawADCData();
	switch (fsm_memory->current_mode)
	{
	case NORMAL_MODE:
		// Changes display based off joy-stick
		joystickBehaviour(fsm_memory, sys_flags);
		break;

	case TEST_MODE:
		// Ability to change display but not the units
		testingModeTaskExecute(fsm_memory, step_memory, sys_flags);
		break;

	case SET_GOAL:
		setGoalTaskExecute(step_memory, sys_flags);
		break;
	}
}


