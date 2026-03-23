#ifndef GYRO_FILTER_H
#define GYRO_FILTER_H

#include <stdint.h>
#include "gyro_filter.h"

float mpu6050_get_pitch_x(raw_out_t raw_values);

float mpu6050_get_roll_y(raw_out_t raw_values);

#endif