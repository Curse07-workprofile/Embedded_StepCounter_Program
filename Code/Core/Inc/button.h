#ifndef BUTTON_H
#define BUTTON_H

#include "app.h"

// Button module for joy-stick button and 4 other buttons with FSM coordination

void buttonInit(void);
void buttonInputBehaviour(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags);

#endif // BUTTON_H
