#include "progress_leds.h"
#include "rgb.h"
#include "tim.h"
#include "pwm.h"
#include "app.h"

void progressLEDsInit(void)
{
    rgb_colour_all_on();
    rgb_led_all_off();
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, 0);
}

void progressLEDsTaskExecute(Step_Values_t* step_memory)
{
	uint16_t percentage = ((step_memory->step_count * 100) / step_memory->step_goal);
	if (percentage <= 25)
	{
		uint16_t duty_cycle = (percentage * 4);
		pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, duty_cycle);
		rgb_led_off(RGB_LEFT);
		rgb_led_off(RGB_DOWN);
		rgb_led_off(RGB_RIGHT);
	}
	else if ((percentage > 25) && (percentage < 50))
	{

		rgb_led_on(RGB_UP);
		rgb_led_off(RGB_LEFT);
		rgb_led_off(RGB_DOWN);
		rgb_led_off(RGB_RIGHT);
	}
	else if ((percentage >= 50) && (percentage < 75))
	{
		rgb_led_on(RGB_UP);
		rgb_led_on(RGB_LEFT);
		rgb_led_off(RGB_DOWN);
		rgb_led_off(RGB_RIGHT);
	}
	else if ((percentage >= 75) && (percentage < 100))
	{
		rgb_led_on(RGB_UP);
		rgb_led_on(RGB_LEFT);
		rgb_led_on(RGB_DOWN);
		rgb_led_off(RGB_RIGHT);
	}
	else if (percentage >= 100)
	{
		rgb_led_on(RGB_UP);
		rgb_led_on(RGB_LEFT);
		rgb_led_on(RGB_DOWN);
		rgb_led_on(RGB_RIGHT);
	}
}
