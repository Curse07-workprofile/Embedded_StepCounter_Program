#ifndef DISPLAY_H
#define DISPLAY_H

// Display module with Display FSM for operation of tasks,
// with refresh on internal value changes
// Improvements: render FSM to allow context switching to service other tasks

void displayInit(void);
void displayStatusBehaviour(FSM_t* fsm_memory, Step_Values_t* step_memory, Flags_t* sys_flags);


#endif // DISPLAY_H
