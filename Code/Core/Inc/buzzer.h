#ifndef BUZZER_H
#define BUZZER_H

#include "app.h"

// Buzzer module for startup tone and victory tone completion
// Improvements: Context switching with a tone FSM to update display and service other tasks

void buzzerInit(void);
void buzzerCheckGoal(Step_Values_t* step_memory);

#endif // BUZZER_H
