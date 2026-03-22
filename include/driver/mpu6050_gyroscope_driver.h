#ifndef MPU_DRIVER_H
#define MPU_DRIVER_H

#include <stdint.h>

// Structure to store the final 16-bit gyroscope values for all 3 axes
typedef struct {
    int16_t GYRO_XOUT_V; // X-axis final value
    int16_t GYRO_YOUT_V; // Y-axis final value
    int16_t GYRO_ZOUT_V; // Z-axis final value
    int16_t ACCEL_XOUT_V; // X-axis final value
    int16_t ACCEL_YOUT_V; // Y-axis final value
    int16_t ACCEL_ZOUT_V; // Z-axis final value
} raw_out_t;

// Structure to store the raw 8-bit values (High and Low) straight from the sensor's registers
typedef struct {
    int8_t gyro_x_out_h; // X-axis High byte
    int8_t gyro_x_out_l; // X-axis Low byte
    int8_t gyro_y_out_h; // Y-axis High byte
    int8_t gyro_y_out_l; // Y-axis Low byte
    int8_t gyro_z_out_h; // Z-axis High byte
    int8_t gyro_z_out_l; // Z-axis Low byte
} gyro_axis_hl_v;

// Structure to store the raw 8-bit values (High and Low) straight from the sensor's registers
typedef struct {
    int8_t accel_x_out_h; // X-axis High byte
    int8_t accel_x_out_l; // X-axis Low byte
    int8_t accel_y_out_h; // Y-axis High byte
    int8_t accel_y_out_l; // Y-axis Low byte
    int8_t accel_z_out_h; // Z-axis High byte
    int8_t accel_z_out_l; // Z-axis Low byte
} accel_axis_hl_v;

// Structure to store calculated angles
typedef struct {
    float x_angle_value; // Calculated angle for the X axis (Pitch)
    float y_angle_value; // Calculated angle for the Y axis (Roll)
} xy_angles_t;

// Initializes and configures the MPU-6050 registers
void setup_driver_registers(void);

// Reads the raw 8-bit registers, unites them into 16-bit values, and sets the struct
raw_out_t get_raw_values(void);

// Gets X and Y axis angles and fills a struct
xy_angles_t mpu6050_get_gyro_angles(void);

#endif

