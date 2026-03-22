#ifndef MPU6050_PROCESSING_H
#define MPU6050_PROCESSING_H

#include <stdint.h>
#include "mpu6050_gyroscope_driver.h"

float mpu6050_get_pitch_x(raw_out_t raw_values);

float mpu6050_get_roll_y(raw_out_t raw_values);

#endif