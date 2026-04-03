#ifndef GYRO_FILTER_H
#define GYRO_FILTER_H

#include <stdint.h>
#include "driver/gyro_driver.h"

/*
 * @brief Calculates the pitch angle (rotation around the X axis)
 * using raw accelerometer/gyroscope values obtained from the MPU6050.
 */
float mpu6050_get_pitch_x(raw_out_t raw_values);

/*
 * @brief Calculates the roll angle (rotation around the Y axis)
 * using raw accelerometer/gyroscope values obtained from the MPU6050.
 */
float mpu6050_get_roll_y(raw_out_t raw_values);

#endif