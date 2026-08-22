#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"
#include "timing.h"


// Static variables private to module
static uint32_t ticks;

static uint32_t uartTaskNextRun;
static uint32_t adcTaskNextRun;
static uint32_t displayTaskNextRun;
static uint32_t buttonTaskNextRun;
static uint32_t buzzerTaskNextRun;
static uint32_t ledTaskNextRun;

void initTiming(void)
{
	uartTaskNextRun 	= (HAL_GetTick() + UART_TASK_PERIOD_TICKS);
	adcTaskNextRun 		= (HAL_GetTick() + ADC_TASK_PERIOD_TICKS);
	displayTaskNextRun 	= (HAL_GetTick() + DISPLAY_TASK_PERIOD_TICKS);
	buttonTaskNextRun 	= (HAL_GetTick() + BUTTON_TASK_PERIOD_TICKS);
	buzzerTaskNextRun 	= (HAL_GetTick() + BUZZER_TASK_PERIOD_TICKS);
	ledTaskNextRun 		= (HAL_GetTick() + LED_TASK_PERIOD_TICKS);
}

void loopTimer(void)
{
	ticks = HAL_GetTick();
}

bool uartTimingCheckRun(void)
{

	bool run_task = (ticks > uartTaskNextRun);

	if (run_task)
	{
		uartTaskNextRun += UART_TASK_PERIOD_TICKS;
	}

	return run_task;
}


bool adcTimingCheckRun(void)
{
	bool run_task = (ticks > adcTaskNextRun);

	if (run_task)
	{
		adcTaskNextRun += ADC_TASK_PERIOD_TICKS;
	}

	return run_task;
}

bool displayTimingCheckRun(void)
{
	bool run_task = (ticks > displayTaskNextRun);

	if (run_task)
	{
		displayTaskNextRun += DISPLAY_TASK_PERIOD_TICKS;
	}

	return run_task;
}

bool buttonTimingCheckRun(void)
{
	bool run_task = (ticks > buttonTaskNextRun);

	if (run_task)
	{
		buttonTaskNextRun += BUTTON_TASK_PERIOD_TICKS;
	}

	return run_task;
}

bool buzzerTimingCheckRun(void)
{
	bool run_task = (ticks > buzzerTaskNextRun);

	if (run_task)
	{
		buzzerTaskNextRun += BUZZER_TASK_PERIOD_TICKS;
	}

	return run_task;
}

bool ledTimingCheckRun(void)
{
	bool run_task = (ticks > ledTaskNextRun);

	if (run_task)
	{
		ledTaskNextRun += LED_TASK_PERIOD_TICKS;
	}

	return run_task;
}
