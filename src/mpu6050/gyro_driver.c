#include "driver/gyro_driver.h"
#include "driver/registers.h"
#include "string.h"
#include "reg_utils.h"
#include "pico_driver/i2c_rw_data.h"
#include "driver/gyro_driver.h"

/* --- Configuration Bits & Values --- */
// Sets Gyroscope sensitivity range to ±500 °/s
#define GYRO_SENSIBILITY_500_BIT (1 << 3)

// Sets Accelerometer scale range to 4g
#define ACCEL_FULL_SCALE_RANGE_4G (1 << 3)

// Enables the "Data Ready" interrupt signal bit
#define DATA_RDY_EN_BIT 0x01

// The expected ID value of the MPU-6050 sensor
#define WHOAMI_REG_DEFAULT_V_BIT 0x68

// Sets the Digital Low Pass Filter to 98Hz (Level 2)
#define DLPF_CFG_2_BIT 0x02

// Divides the sample rate to achieve a 100Hz output
#define SMPRT_DIV_VALUE_BIT 0x09

// Used to clear or reset all bits in a register
#define RESET_ALL_BITS 0x00

/* --- Register Access Macros (Pointers & Dereferencing) --- */
// Pointer to the Who Am I identification register
#define R_WHOAMI_REG ((volatile uint8_t *)(WHOAMI_REG))

// Pointer to check the current interrupt status
#define R_INTR_ENABLE_STATUS ((volatile uint8_t *)(INTR_ENABLE_STATUS))

// Direct access to write the Sample Rate Divider
#define W_SMPRT_DIV ((volatile uint8_t *)(SMPRT_DIV_REG))

// Direct access to write the Interrupt Enable register
#define W_INTR_ENABLE ((volatile uint8_t *)(INTR_ENABLE_REG))

// Direct access to write Power Management 1
#define W_PWR_MGMT_1 ((volatile uint8_t *)(PWR_MGMT_1_REG))

// Direct access to write General Configuration
#define W_CONFIGURATION ((volatile uint8_t *)(CONFIGURATION_REG))

// Direct access to write Gyroscope Range
#define W_GYRO_CONFIGURATION ((volatile uint8_t *)(GYRO_CONFIGURATION_REG))

// Direct access to write Accelerometer Range
#define W_ACCEL_CONFIGURATION ((volatile uint8_t *)(ACCEL_CONFIGURATION_REG))

void setup_driver_registers(void)
{
    // Waits until find sensor
    while(!(*R_WHOAMI_REG & WHOAMI_REG_DEFAULT_V_BIT)){};

    // Resets all bits from PWR management register
    *W_PWR_MGMT_1 &= RESET_ALL_BITS;

    // Sets Digital Low Pass Filter (DLPF) to mode 2:
    // Bandwidth: 98Hz
    // Delay: 2.8ms
    // Fs: 1kHz
    *W_CONFIGURATION |= DLPF_CFG_2_BIT;

    // Configures gyroscope sensibility to ±500°/s
    *W_GYRO_CONFIGURATION |= GYRO_SENSIBILITY_500_BIT;

    // Configures accelerometer sensibility to ±4g
    *W_ACCEL_CONFIGURATION |= ACCEL_FULL_SCALE_RANGE_4G;

    // Enables interrupt when data is ready
    *W_INTR_ENABLE |= DATA_RDY_EN_BIT;

    // Sets 0x09 to divide to set sample rate value to 100hz, getting 1 sample each 10ms;
    *W_SMPRT_DIV |= SMPRT_DIV_VALUE_BIT;
}

raw_out_t get_raw_values(void)
{
    int16_t gyro_xout, gyro_yout, gyro_zout;
    int16_t accel_xout, accel_yout, accel_zout;
    gyro_axis_hl_v gyro_axis;
    accel_axis_hl_v accel_axis;
    raw_out_t g_data;

    // Map register addresses to the local struct members
    gyro_axis.gyro_x_out_h = GYRO_REG_XOUT_H;
    gyro_axis.gyro_x_out_l = GYRO_REG_XOUT_L;
    gyro_axis.gyro_y_out_h = GYRO_REG_YOUT_H;
    gyro_axis.gyro_y_out_l = GYRO_REG_YOUT_L;
    gyro_axis.gyro_z_out_h = GYRO_REG_ZOUT_H;
    gyro_axis.gyro_z_out_l = GYRO_REG_ZOUT_L;

    accel_axis.accel_x_out_h = ACCEL_REG_XOUT_H;
    accel_axis.accel_x_out_l = ACCEL_REG_XOUT_L;
    accel_axis.accel_y_out_h = ACCEL_REG_YOUT_H;
    accel_axis.accel_y_out_l = ACCEL_REG_YOUT_L;
    accel_axis.accel_z_out_h = ACCEL_REG_ZOUT_H;
    accel_axis.accel_z_out_l = ACCEL_REG_ZOUT_L;

    // Check if the sensor has new data ready in the Interrupt Status register
    if(*R_INTR_ENABLE_STATUS & DATA_RDY_EN_BIT){        
        // Combine two 8-bit registers (High/Low) into a single 16-bit value for each axis
        gyro_xout = reg_uniter_8to16(gyro_axis.gyro_x_out_h, gyro_axis.gyro_x_out_l);
        gyro_yout = reg_uniter_8to16(gyro_axis.gyro_y_out_h, gyro_axis.gyro_y_out_l);
        gyro_zout = reg_uniter_8to16(gyro_axis.gyro_z_out_h, gyro_axis.gyro_z_out_l);

        // Combine two 8-bit registers (High/Low) into a single 16-bit value for each axis
        accel_xout = reg_uniter_8to16(accel_axis.accel_x_out_h, accel_axis.accel_x_out_l);
        accel_yout = reg_uniter_8to16(accel_axis.accel_y_out_h, accel_axis.accel_y_out_l);
        accel_zout = reg_uniter_8to16(accel_axis.accel_z_out_h, accel_axis.accel_z_out_l);
    }

    // Store the processed 16-bit values into the final output structure
    g_data.GYRO_XOUT_V = gyro_xout;
    g_data.GYRO_YOUT_V = gyro_yout;
    g_data.GYRO_ZOUT_V = gyro_zout;

    // Store the processed 16-bit values into the final output structure
    g_data.ACCEL_XOUT_V = accel_xout;
    g_data.ACCEL_YOUT_V = accel_yout;
    g_data.ACCEL_ZOUT_V = accel_zout;

    return g_data;
}

xy_angles_t mpu6050_get_gyro_angles(void)
{
    raw_out_t raw_data = {0};
    xy_angles_t xy_angles = {0};

    raw_data = get_raw_values();

    xy_angles.x_angle_value = mpu6050_get_pitch_x(raw_data);
    xy_angles.y_angle_value = mpu6050_get_roll_y(raw_data);

    return xy_angles;
}