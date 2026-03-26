#include "driver/gyro_filter.h"
#include <stdint.h>
#include "pico_driver/time.h"
#include "driver/gyro_driver.h"
#include <math.h>

// Constants for time conversion and unit transformation

#define SEC_DIVIDER 1000000.0f
#define RAD_TO_DEG (180.0f / 3.14159265f)

/**
 * Calculates the Pitch (X-axis) using a Complementary Filter
 */
float mpu6050_get_pitch_x(raw_out_t raw_values)
{
    // Static variables to keep values between function calls
    static uint32_t last_time = 0;
    static float angle_x = 0;
    
    float alpha = 0.98;
    float alpha_complement = 1.0f - alpha;

    // Calculate time elapsed (delta time)
    uint32_t now = get_time_us();
    if (last_time == 0) {
        last_time = now;
        return 0;
    }

    float dt = (float)(now - last_time) / SEC_DIVIDER;
    last_time = now;

    // Convert raw gyroscope data to degrees per second
    float gyro_x_conv = (float)raw_values.GYRO_XOUT_V / 65.5;

    // Integrate gyroscope data to predict the new angle
    float gyro_part = angle_x + (gyro_x_conv * dt);

    float ax = (float)raw_values.ACCEL_XOUT_V / 8192.0f;
    float ay = (float)raw_values.ACCEL_YOUT_V / 8192.0f;
    float az = (float)raw_values.ACCEL_ZOUT_V / 8192.0f;

    // Calculate angle from accelerometer data
    float accel_angle_x = atan2(ay, sqrt(ax * ax + az * az)) * RAD_TO_DEG;
    
    // Fuse both values: 98% Gyroscope (precision) + 2% Accelerometer (stability)
    angle_x = (alpha * gyro_part) + (alpha_complement * accel_angle_x);
    
    return angle_x;
}

/**
 * Calculates the Roll (Y-axis) using a Complementary Filter
 */
float mpu6050_get_roll_y(raw_out_t raw_values)
{
    static uint32_t last_time = 0;
    static float angle_y = 0; 
    
    float alpha = 0.98f;
    float alpha_complement = 1.0f - alpha;

    uint32_t now = get_time_us();
    if (last_time == 0) {
        last_time = now;
        return 0.0f;
    }
    float dt = (float)(now - last_time) / SEC_DIVIDER;
    last_time = now;

    float gyro_y_conv = (float)raw_values.GYRO_YOUT_V / 65.5f;
    float gyro_part_y = angle_y + (gyro_y_conv * dt);

    float ax = (float)raw_values.ACCEL_XOUT_V / 8192.0f;
    float az = (float)raw_values.ACCEL_ZOUT_V / 8192.0f;

    // Calculate angle from accelerometer data (note the negative X-axis)
    float accel_angle_y = atan2(-ax, az) * RAD_TO_DEG;    
    
    // Fuse values to get the final Roll angle
    angle_y = (alpha * gyro_part_y) + (alpha_complement * accel_angle_y);

    return angle_y;
}
