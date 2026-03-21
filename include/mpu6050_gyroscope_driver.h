#ifndef MPU_DRIVER_H
#define MPU_DRIVER_H

#include <stdint.h>

// Structure to store the final 16-bit gyroscope values for all 3 axes
typedef struct {
    uint16_t GYRO_XOUT_V; // X-axis final value
    uint16_t GYRO_YOUT_V; // Y-axis final value
    uint16_t GYRO_ZOUT_V; // Z-axis final value
} gyro_out_t;

// Structure to store the raw 8-bit values (High and Low) straight from the sensor's registers
typedef struct {
    uint8_t gyro_x_out_h; // X-axis High byte
    uint8_t gyro_x_out_l; // X-axis Low byte
    uint8_t gyro_y_out_h; // Y-axis High byte
    uint8_t gyro_y_out_l; // Y-axis Low byte
    uint8_t gyro_z_out_h; // Z-axis High byte
    uint8_t gyro_z_out_l; // Z-axis Low byte
} gyro_axis_hl_v;

// Initializes and configures the MPU-6050 registers
void setup_driver_registers(void);

// Reads the raw 8-bit registers, unites them into 16-bit values, and returns the struct
gyro_out_t get_gyro_values(void);

#endif

