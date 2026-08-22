/*
 * imu_lsm6ds.h
 *
 *  Created on: Nov 27, 2024
 *      Author: fsy13
 *
 */

#ifndef INC_IMU_LSM6DS_H_
#define INC_IMU_LSM6DS_H_

#include <stdint.h>

typedef enum {
	INT1_CTRL 	= 0x0D,
	CTRL1_XL	= 0x10,
	CTRL10_C	= 0x19,
	OUTX_L_XL 	= 0x28,
	OUTX_H_XL,
	OUTY_L_XL,
	OUTY_H_XL,
	OUTZ_L_XL,
	OUTZ_H_XL,
	STEP_COUNTER_L = 0x4B,
	STEP_COUNTER_H = 0x4C
} imu_register_t;

// Standard options
#define CTRL1_XL_HIGH_PERFORMANCE 0xA0U
#define CTRL10_C_ENABLE_STEP_ALGORITHM 0x16
#define INT1_CTRL_ENABLE 0x80
//#define CTRL10_C_RESET 0x14 ^ 0x02

void imu_lsm6ds_write_byte(imu_register_t register_address, uint8_t value);

uint8_t imu_lsm6ds_read_byte(imu_register_t register_address);

#endif /* INC_IMU_LSM6DS_H_ */
