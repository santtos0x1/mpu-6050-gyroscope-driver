#include "mpu6050_processing.h"
#include <stdint.h>
#include "pico_time.h"
#include "mpu6050_gyroscope_driver.h"
#include <math.h>

#define SEC_DIVIDER 1000000.0f
#define RAD_TO_DEG 180/3.14

float mpu6050_get_pitch_x(raw_out_t raw_values)
{
    static uint32_t last_time = 0;
    static float angle_x = 0;
    
    float alpha = 0.98;
    float alpha_complement = 1.0f - alpha;

    uint32_t now = get_time_us();
    float dt = (now - last_time) / SEC_DIVIDER;
    
    last_time = now;

    float gyro_x_conv = (float)raw_values.GYRO_XOUT_V / 65.5;
    float gyro_part = angle_x + (gyro_x_conv * dt);

    float accel_angle_x = atan2(raw_values.ACCEL_YOUT_V, sqrt(pow(raw_values.ACCEL_XOUT_V, 2) + pow(raw_values.ACCEL_ZOUT_V, 2))) * RAD_TO_DEG;
    angle_x = (alpha * gyro_part) + (alpha_complement * accel_angle_x);
    
    return angle_x;
}

float mpu6050_get_roll_y(raw_out_t raw_values)
{
    static uint32_t last_time = 0;
    static float angle_y = 0; 
    
    float alpha = 0.98;
    float alpha_complement = 1.0f - alpha;

    uint32_t now = get_time_us();
    float dt = (now - last_time) / SEC_DIVIDER;
    
    last_time = now;

    float gyro_y_conv = raw_values.GYRO_YOUT_V / 65.5f;
    float gyro_part_y = angle_y + (gyro_y_conv * dt);

    float accel_angle_y = atan2(-raw_values.ACCEL_XOUT_V, raw_values.ACCEL_ZOUT_V) * RAD_TO_DEG;
    angle_y = (alpha * gyro_part_y) + (alpha_complement * accel_angle_y);

    return angle_y;
}
