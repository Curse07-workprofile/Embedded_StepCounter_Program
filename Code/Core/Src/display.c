#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"
#include "app.h"

void displayCurrentStepsExecute(Step_Values_t* step_memory, Flags_t* sys_flags)
{
	ssd1306_Fill(Black);
	char buffer[20];
	if (!(sys_flags->units_of_display[0]))
	{
		snprintf(buffer, sizeof(buffer), "  %u steps", step_memory->step_count);
	}
	else
	{
		uint16_t percentage = ((step_memory->step_count * 100) / step_memory->step_goal);
		snprintf(buffer, sizeof(buffer), "  %u %%", percentage);
	}

	if (step_memory->step_goal <= step_memory->step_count)
	{
		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString("Goal Reached !!", Font_7x10, White);
	}

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("  Step Count\0", Font_7x10, White);
	ssd1306_SetCursor(0, 40);
	ssd1306_WriteString(buffer, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void displayProgressToGoal(Step_Values_t* step_memory, FSM_t* fsm_memory)
{
	ssd1306_Fill(Black);
	char buffer1[20];
	ssd1306_SetCursor(0, 0);
	if (fsm_memory->current_mode == SET_GOAL)
	{
		snprintf(buffer1, sizeof(buffer1), " %u steps", step_memory->step_goal);
		ssd1306_WriteString("Set New Goal\0", Font_7x10, White);
	}
	else
	{
		char buffer2[20];
		if (step_memory->step_goal > step_memory->step_count)
		{
			uint16_t steps_to_goal 	= (step_memory->step_goal - step_memory->step_count);
			snprintf(buffer1, sizeof(buffer1), " %u steps", steps_to_goal);
			snprintf(buffer2, sizeof(buffer2), " Goal: %u steps", step_memory->step_goal);
		}
		else
		{
			snprintf(buffer1, sizeof(buffer1), "Goal Reached !!");
			snprintf(buffer2, sizeof(buffer2), "Goal: %u steps", step_memory->step_goal);
		}
		ssd1306_WriteString("Progress To Goal\0", Font_7x10, White);
		ssd1306_SetCursor(0, 40);
		ssd1306_WriteString(buffer2, Font_7x10, White);
	}
	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString(buffer1, Font_7x10, White);
	ssd1306_UpdateScreen();
}

void displayDistanceTravelled(Step_Values_t* step_memory, Flags_t* sys_flags)
{
	ssd1306_Fill(Black);
	char buffer[20];
	if (!(sys_flags->units_of_display[1]))
	{
		uint16_t meters 	= ((step_memory->step_count << 2) / 5);
		uint8_t kilometers 	= 0;
		while (meters >= 1000) // Should only be 12 iterations at most (15000 steps = 12 Km)
		{
			meters -= 1000;
			kilometers++;
		}

		if (meters >= 100)
		{
			snprintf(buffer, sizeof(buffer), "  %u.%u Km", kilometers, meters);
		}
		else if (meters >= 10)
		{
			snprintf(buffer, sizeof(buffer), "  %u.0%u Km", kilometers, meters);
		}
		else
		{
			snprintf(buffer, sizeof(buffer), "  %u.00%u Km", kilometers, meters);
		}
	}
	else
	{
		uint16_t yards = (step_memory->step_count - (step_memory->step_count >> 3));
		snprintf(buffer, sizeof(buffer), "  %u Yards", yards);
	}

	if (step_memory->step_goal <= step_memory->step_count)
	{
		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString("Goal Reached !!", Font_7x10, White);
	}
	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("Distance Travelled\0", Font_7x10, White);
	ssd1306_SetCursor(0, 40);
	ssd1306_WriteString(buffer, Font_7x10, White);
	ssd1306_UpdateScreen();
}

// public functions


void displayInit(void)
{
	ssd1306_Init();
}

void displayStatusBehaviour(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags)
{
	if (sys_flags->update_display)
	{
		sys_flags->update_display = false;
		switch (fsm_memory->current_display)
		{
		case CURRENT_STEPS:
			displayCurrentStepsExecute(step_memory, sys_flags);
			break;

		case GOAL_PROGRESS:
			displayProgressToGoal(step_memory, fsm_memory);
			break;

		case DISTANCE_TRAVELLED:
			displayDistanceTravelled(step_memory, sys_flags);
			break;
		}
	}
}
