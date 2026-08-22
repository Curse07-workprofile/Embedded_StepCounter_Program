#include <stdint.h>
#include <stdbool.h>
#include "number_processing.h"



// potentiometer scaling values
#define MAX_POT 4095
#define MIN_POT 121

#define MAX_GOAL 15000
#define MIN_GOAL 500

#define SCALE_FACTOR 100

#define SCALED_MAX_GOAL (MAX_GOAL/SCALE_FACTOR)
#define SCALED_MIN_GOAL (MIN_GOAL/SCALE_FACTOR)

/*typedef enum
{
	NEUTRAL = 0,
	POSITIVE,
	NEGATIVE
}Axial_dir_t;
*/
typedef struct
{
	uint16_t origin_offset;
	uint16_t max_reciprical;
	uint16_t min_reciprical;

	Directions_t direction;

	uint8_t calculated_bit_shift;
	uint8_t min_percent_limit;

	uint8_t action_margin; // as a percentage the margin the joy-stick must be more than to actuate

} Scaling_values_t;

typedef struct
{
	uint16_t max_value;
	uint16_t min_value;

	uint8_t max_goal; // divided by 100 to give step increments in the 100s
	uint8_t min_goal;

	uint16_t pot_range;
	uint8_t goal_range;

} Potentiometer_values_t;

static Scaling_values_t x_values = {
		.origin_offset 			= 2265, 	// for zeroing the rest position of the joy-stick
		.max_reciprical 		= 3855,		// used to avoid division, (the compiler probably does this already)
		.min_reciprical 		= 4369,
		.calculated_bit_shift 	= 16,
		.min_percent_limit 		= 5,		// For setting the joy-stick to center and reseting the latch
		.action_margin			= 50
};

static Scaling_values_t y_values = {
		.origin_offset 			= 2231,
		.max_reciprical 		= 4096,
		.min_reciprical 		= 3676,
		.calculated_bit_shift 	= 16,
		.min_percent_limit 		= 5,
};

static const Potentiometer_values_t pot_values = {
		.max_value		= MAX_POT,
		.min_value		= MIN_POT,

		.max_goal		= SCALED_MAX_GOAL,
		.min_goal		= SCALED_MIN_GOAL,

		.pot_range		= (MAX_POT - MIN_POT),
		.goal_range		= (SCALED_MAX_GOAL - SCALED_MIN_GOAL)

};

uint8_t processADCValues(uint16_t raw_varible, Scaling_values_t* varible_values)
{
	int16_t varible = (-(raw_varible)) + varible_values->origin_offset;
	uint32_t percent_value = 0;
	if (varible < 0)
	{
		percent_value = ((-varible) * varible_values->min_reciprical) >> varible_values->calculated_bit_shift; // multiplying by the reciprocal, then left shifting to avoid division
	}
	else if (varible > 0)
	{
		percent_value = (varible * varible_values->max_reciprical) >> varible_values->calculated_bit_shift;
	}
	if ((percent_value <= varible_values->min_percent_limit))
	{
		varible_values->direction = CENTER;
		percent_value = 0;
	}
	else if (percent_value > 100)
	{
		percent_value = 100;
	}
	return percent_value;
}


void getJoystickDir(uint16_t x_val, uint16_t y_val, Directions_t* joystick_dir)
{
	int16_t x_diff = x_val - (x_values.origin_offset);
	int16_t y_diff = y_val - (y_values.origin_offset);

	x_values.direction = (x_diff < 0) ? RIGHT : LEFT;
	y_values.direction = (y_diff < 0) ? UP : DOWN;

	uint8_t x_ratio = processADCValues(x_val, &x_values);
	uint8_t y_ratio = processADCValues(y_val, &y_values);

	// will actuate a change if greater than the action_margin or set the state to rest and reset the latch
	if ((x_ratio >= x_values.action_margin) || (y_ratio >= x_values.action_margin) || ((x_ratio == 0) && (y_ratio == 0)))
	{
		if (x_ratio >= y_ratio)
		{
			*joystick_dir = x_values.direction;
		}
		else
		{
			*joystick_dir = y_values.direction;
		}
	}

}

void getTestingModeJoystick(uint16_t x_val, uint16_t y_val, Directions_t* joystick_dir_x, uint8_t* percentage, bool* increment_dir)
{
	int16_t x_diff = x_val - (x_values.origin_offset);
	int16_t y_diff = y_val - (y_values.origin_offset);

	x_values.direction = (x_diff < 0) ? RIGHT : LEFT;
	y_values.direction = (y_diff < 0) ? UP : DOWN;

	uint8_t x_percentage = processADCValues(x_val, &x_values);
	uint8_t y_percentage = processADCValues(y_val, &y_values);

	if ((x_percentage >= x_values.action_margin) || (x_percentage == 0))
	{
		*joystick_dir_x = x_values.direction;
	}

	*percentage = y_percentage;
	if (y_values.direction == UP)
	{
		*increment_dir = true;
	}
	else
	{
		*increment_dir = false;
	}
}

// potentiometer values
uint16_t processPotentiometerADC(uint16_t potentiometer_raw)
{

	uint16_t steps = (((pot_values.goal_range) * (potentiometer_raw - MIN_POT)) / (pot_values.pot_range));
	steps += pot_values.min_goal;

	if (steps > (pot_values.max_goal))
	{
		steps = (pot_values.max_goal);
	}
	else if (steps < (pot_values.min_goal))
	{
		steps = (pot_values.min_goal);
	}
	return (steps * SCALE_FACTOR);
}
