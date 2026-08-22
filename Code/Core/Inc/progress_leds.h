#ifndef PROGRESS_LEDS_H
#define PROGRESS_LEDS_H

#include "app.h"

// Progress status LEDs module, to show a percentage of goal completion with four LEDs

void progressLEDsInit(void);
void progressLEDsTaskExecute(Step_Values_t* step_memory);


#endif // PROGRESS_LEDS_H
