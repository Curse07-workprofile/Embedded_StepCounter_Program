#ifndef STEP_UPDATE_H
#define STEP_UPDATE_H

#include <stdbool.h>
#include "app.h"

// Step update file IMU reading and interrupt flag handling

void stepUpdateInit(void);
void stepUpdateTaskExecute(Step_Values_t* step_memory, bool* step_flag);

#endif // STEP_UPDATE_H
