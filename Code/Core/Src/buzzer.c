#include <stdint.h>
#include <stdbool.h>
#include "buzzer.h"
#include "tim.h"
#include "pwm.h"
#include "app.h"

#define TIMER_CLOCK 12000000 // 12MHz Time Clock

#define NOTE_REST  0

// Octave 5
#define NOTE_C5    523
#define NOTE_CS5   554
#define NOTE_D5    587
#define NOTE_DS5   622
#define NOTE_E5    659
#define NOTE_F5    698
#define NOTE_FS5   740
#define NOTE_G5    784
#define NOTE_GS5   831
#define NOTE_A5    880
#define NOTE_AS5   932
#define NOTE_B5    988

// Octave 6
#define NOTE_C6    1047
#define NOTE_CS6   1109
#define NOTE_D6    1175
#define NOTE_DS6   1245
#define NOTE_E6    1319
#define NOTE_F6    1397
#define NOTE_FS6   1480
#define NOTE_G6    1568
#define NOTE_GS6   1661
#define NOTE_A6    1760
#define NOTE_AS6   1865
#define NOTE_B6    1976

#define NOTE_END   {0, 0}

typedef struct
{
    uint16_t frequency;
    uint16_t duration_ms;
} Note_t;


static const Note_t tune1[] = {
    {NOTE_C5,   150},
    {NOTE_REST,  50},
    {NOTE_E5,   150},
    {NOTE_REST,  50},
    {NOTE_G5,   150},
    {NOTE_REST,  50},
    {NOTE_C6,   300},
    {NOTE_REST, 100},
    {NOTE_G5,   150},
    {NOTE_C6,   500},
    NOTE_END,
};

static const Note_t tune2[] = {
    {NOTE_C5,   100},
    {NOTE_E5,   100},
    {NOTE_G5,   100},
    {NOTE_C6,   200},
    {NOTE_REST,  80},
    {NOTE_C6,   100},
    {NOTE_E6,   100},
    {NOTE_G6,   400},
    NOTE_END,
};

void buzzerSetFrequency(uint16_t freq)
{
	if (freq == NOTE_REST)
	{
		__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, 0);
		return;
	}
    uint32_t arr = TIMER_CLOCK / freq - 1;
    __HAL_TIM_SET_AUTORELOAD(&htim16, arr);
    __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, arr / 2); // %50 of duty cycle for max volume
}

void playNote(Note_t note)
{
    buzzerSetFrequency(note.frequency);
    HAL_Delay(note.duration_ms);
}

void playTune(const Note_t* tune)
{
    while (tune->duration_ms != 0)
    {
        playNote(*tune);
        tune++;
    }
    __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim16, TIMER_CLOCK / NOTE_C5 - 1);
}

void buzzerInit(void)
{
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, 0);
    playTune(tune2);
}


void buzzerCheckGoal(Step_Values_t* step_memory)
{
	static bool victory_latch = false;

	if ((step_memory->step_count >= step_memory->step_goal) && (victory_latch == false))
	{
		victory_latch = true;
		playTune(tune1);
	}
	else if ((step_memory->step_count < step_memory->step_goal) && (victory_latch == true))
	{
		victory_latch = false;
	}
}


