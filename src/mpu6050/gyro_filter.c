#include <math.h>
#include <stdint.h>

#include "driver/gyro_filter.h"
#include "pico_driver/time.h"
#include "driver/gyro_driver.h"

// Constants for time conversion and unit transformation
#define SEC_DIVIDER 1000000.0f   // Converts microseconds → seconds
#define RAD_TO_DEG (180.0f / 3.14159265f) // Radians → degrees conversion

float mpu6050_get_pitch_x(raw_out_t raw_values)
{
    // Static variables to keep state between function calls
    static uint32_t last_time = 0;
    static float angle_x = 0;
    
    // Complementary filter weights
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

    // Convert raw gyroscope value to degrees per second
    // Sensitivity for ±500°/s is 65.5 LSB/°/s
    float gyro_x_conv = (float)raw_values.GYRO_XOUT_V / 65.5;

    // Integrate gyroscope angular velocity to estimate new angle
    float gyro_part = angle_x + (gyro_x_conv * dt);

    // Convert raw accelerometer values to g units
    // Sensitivity for ±4g is 8192 LSB/g
    float ax = (float)raw_values.ACCEL_XOUT_V / 8192.0f;
    float ay = (float)raw_values.ACCEL_YOUT_V / 8192.0f;
    float az = (float)raw_values.ACCEL_ZOUT_V / 8192.0f;

    // Estimate pitch angle using accelerometer geometry
    float accel_angle_x = atan2(ay, sqrt(ax * ax + az * az)) * RAD_TO_DEG;
    
    // Fuse both values: Gyroscope (fast response) + Accelerometer (long-term stability)
    angle_x = (alpha * gyro_part) + (alpha_complement * accel_angle_x);
    
    return angle_x;
}

float mpu6050_get_roll_y(raw_out_t raw_values)
{
    // Keeps previous time and filtered angle
    static uint32_t last_time = 0;
    static float angle_y = 0; 
    
    float alpha = 0.98f;
    float alpha_complement = 1.0f - alpha;

    // Calculate time difference between measurements
    uint32_t now = get_time_us();
    if (last_time == 0) {
        last_time = now;
        return 0.0f;
    }

    float dt = (float)(now - last_time) / SEC_DIVIDER;
    last_time = now;

    // Convert gyroscope raw value to °/s
    float gyro_y_conv = (float)raw_values.GYRO_YOUT_V / 65.5f;

    // Integrate gyro angular velocity
    float gyro_part_y = angle_y + (gyro_y_conv * dt);

    // Convert accelerometer raw values to g units
    float ax = (float)raw_values.ACCEL_XOUT_V / 8192.0f;
    float az = (float)raw_values.ACCEL_ZOUT_V / 8192.0f;

    // Calculate roll angle from accelerometer
    // Negative X axis aligns the coordinate system
    float accel_angle_y = atan2(-ax, az) * RAD_TO_DEG;    
    
    // Complementary filter fusion
    angle_y = (alpha * gyro_part_y) + (alpha_complement * accel_angle_y);

    return angle_y;
}