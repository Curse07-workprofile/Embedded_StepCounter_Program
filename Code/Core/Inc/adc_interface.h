#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdbool.h>
#include "app.h"

// ADC module with Joy-stick and Potentiometer functionality,
// for display changing and goal setting, (excluding joy-stick button because cohesion).
// Improvements: fix naming from joy-stick to ADC,
// and move number processing functionality into module.

void joystickADCBehaviour(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags);

#endif // JOYSTICK_H
