// Standard Libraries
#include <stdbool.h>
#include <stdint.h>
// Supporting Libraries and Modules
#include "buttons.h"
// Our Libraries and Modules
#include "button.h"
#include "app.h"

#define TASK_FREQUENCY 		100	// Hz

#define DOUBLE_CLICK_PERIOD	0.3	// seconds
#define JS_HOLD_PERIOD 		1	// seconds

#define REALEASE_MARGIN 	(DOUBLE_CLICK_PERIOD * TASK_FREQUENCY)
#define HOLD_MARGIN			(JS_HOLD_PERIOD * TASK_FREQUENCY)


// Private Functions

void buttonTaskExecute(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags)
{
	static uint16_t count = 0;

	if (buttons_checkButton(RIGHT) == PUSHED)
	{
		sys_flags->transmit_state ^= 1;
	}

	if (buttons_checkButton(DOWN) == PUSHED)
	{

		if (count != 0)
		{
			if (fsm_memory->current_mode == TEST_MODE)
			{
				fsm_memory->current_mode = NORMAL_MODE;
			}
			else if (step_memory->step_count < step_memory->step_goal)
			{
				fsm_memory->current_mode = TEST_MODE;
			}
			count = 0;
		}
		else
		{
			count = REALEASE_MARGIN;
		}
	}

	if (count != 0)
	{
		count--;
	}
}

void buttonDepressionCheck(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags, bool* zero_timer)
{
	static uint16_t count = 0;
	static uint16_t previous_goal;
	static Mode_States_t previous_mode;

	buttonState_t joystick_button = buttons_checkButton(JS_SW1);

	if (*zero_timer)
	{
		count 		= 0;
		*zero_timer = false;
	}

	if ((joystick_button == PUSHED) || ((joystick_button == NO_CHANGE) && (count != 0)))
	{
		count++;
	}
	else if ((joystick_button == RELEASED) && (count != 0))
	{
		if (fsm_memory->current_mode == SET_GOAL)
		{
			fsm_memory->current_mode 	= previous_mode;
			step_memory->step_goal 		= previous_goal;
			sys_flags->update_display 	= true;
		}
		count = 0;
	}

	if (count >= HOLD_MARGIN)
	{
		if (fsm_memory->current_mode == NORMAL_MODE)
		{
			previous_goal 				= step_memory->step_goal;
			previous_mode 				= fsm_memory->current_mode;
			fsm_memory->current_mode 	= SET_GOAL;
			sys_flags->update_display 	= true;
		}
		else
		{
			fsm_memory->current_mode 	= previous_mode;
			sys_flags->update_display	= true;
		}
		count = 0;
	}
}

// Public Functions

void buttonInit(void)
{
	buttons_init();
}


void buttonInputBehaviour(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags)
{
	static bool zero_timer = true;
	buttons_update();
	if (fsm_memory->current_mode != SET_GOAL)
	{
		// Normal button task and goal check
		buttonTaskExecute(fsm_memory, step_memory, sys_flags);

	}
	if (fsm_memory->current_display == GOAL_PROGRESS)
	{
		buttonDepressionCheck(fsm_memory, step_memory, sys_flags, &zero_timer);
	}
	else
	{
		zero_timer = true;
	}
}
