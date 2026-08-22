#ifndef NUMBER_PROCESSING_H
#define NUMBER_PROCESSING_H

// Number processing module for ADC values
// Improvements: more functionality into joy-stick (ADC) module

typedef enum
{
	CENTER = 0,
	RIGHT,
	LEFT,
	UP,
	DOWN,
	NOT_SET
}Directions_t;

void getJoystickDir(uint16_t x_val, uint16_t y_val, Directions_t* joystick_dir);
void getTestingModeJoystick(uint16_t x_val, uint16_t y_val, Directions_t* joystick_dir_x, uint8_t* percentage, bool* joystick_dir);

uint16_t processPotentiometerADC(uint16_t potentiometer_raw);

#endif // NUMBER_PROCESSING_H
