#include <stdint.h>
#include <stdbool.h>
#include "step_update.h"
#include "imu_lsm6ds.h"
#include "gpio.h"

void stepUpdateInit(void)
{
    imu_lsm6ds_write_byte(CTRL1_XL, CTRL1_XL_HIGH_PERFORMANCE);
	imu_lsm6ds_write_byte(INT1_CTRL, INT1_CTRL_ENABLE);
	imu_lsm6ds_write_byte(CTRL10_C, CTRL10_C_ENABLE_STEP_ALGORITHM);
}

void stepUpdateTaskExecute(Step_Values_t* step_memory, bool* step_flag)
{
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	uint8_t steps_low 		= imu_lsm6ds_read_byte(STEP_COUNTER_L);
	uint8_t steps_high 		= imu_lsm6ds_read_byte(STEP_COUNTER_H);
	*step_flag 						= false;
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
	uint16_t current_imu 	= ((((uint16_t)steps_high) << 8) | steps_low);
	step_memory->step_count 		+= (current_imu - step_memory->previous_imu_steps);
	step_memory->previous_imu_steps	= current_imu;
}
