#include <string.h>

#include "driver/gyro_driver.h"
#include "driver/registers.h"
#include "reg_utils.h"
#include "pico_driver/i2c_rw_data.h"
#include "driver/gyro_driver.h"
#include "driver/gyro_filter.h"
#include "pico_driver/time.h"
#include "pico_driver/error.h"

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

pico_err_t setup_driver_registers(void)
{   
    uint8_t s_found = 0;

    // Waits until the sensor responds with the correct WHOAMI value
    for(int i = 0; i < 1000; i++)
    {
        if(rp2040_i2c_read_byte(WHOAMI_REG) == 0x68)
        {
            s_found = 1;
            break;
        }

        // 1ms delay between attempts
        delay_clk_cycle(MS_TO_CYCLES(1));
    }

    // Sensor not found
    if(!s_found)
    {
        return PICO_SENSOR_ACK_ERROR;
    }

    // Resets all bits from Power Management register (wakes up the sensor)
    rp2040_i2c_write_byte(PWR_MGMT_1_REG, RESET_ALL_BITS);

    // Waits for the sensor internal reset to complete
    delay_clk_cycle(MS_TO_CYCLES(10));

    // Sets Digital Low Pass Filter (DLPF) to mode 2:
    // Bandwidth: 98Hz
    // Delay: 2.8ms
    // Fs: 1kHz
    rp2040_i2c_write_byte(CONFIGURATION_REG, DLPF_CFG_2_BIT);

    // Configures gyroscope sensibility to ±500°/s
    rp2040_i2c_write_byte(GYRO_CONFIGURATION_REG, GYRO_SENSIBILITY_500_BIT);

    // Configures accelerometer sensibility to ±4g
    rp2040_i2c_write_byte(ACCEL_CONFIGURATION_REG, ACCEL_FULL_SCALE_RANGE_4G);

    // Enables interrupt when new sensor data is ready
    rp2040_i2c_write_byte(INTR_ENABLE_REG, DATA_RDY_EN_BIT);

    // Sets sample rate divider.
    // 1kHz / (1 + 9) = 100Hz → one sample every 10ms
    rp2040_i2c_write_byte(SMPRT_DIV_REG, SMPRT_DIV_VALUE_BIT);

    return PICO_OK_T;
}

raw_out_t get_raw_values(void)
{
    int16_t gyro_xout, gyro_yout, gyro_zout;
    int16_t accel_xout, accel_yout, accel_zout;
    
    gyro_axis_hl_v gyro_axis = {0};
    accel_axis_hl_v accel_axis = {0};

    raw_out_t g_data = {0};

    // Read gyroscope high/low register values
    gyro_axis.gyro_x_out_h = rp2040_i2c_read_byte(GYRO_REG_XOUT_H);
    gyro_axis.gyro_x_out_l = rp2040_i2c_read_byte(GYRO_REG_XOUT_L);

    gyro_axis.gyro_y_out_h = rp2040_i2c_read_byte(GYRO_REG_YOUT_H);
    gyro_axis.gyro_y_out_l = rp2040_i2c_read_byte(GYRO_REG_YOUT_L);

    gyro_axis.gyro_z_out_h = rp2040_i2c_read_byte(GYRO_REG_ZOUT_H);
    gyro_axis.gyro_z_out_l = rp2040_i2c_read_byte(GYRO_REG_ZOUT_L);

    // Read accelerometer high/low register values
    accel_axis.accel_x_out_h = rp2040_i2c_read_byte(ACCEL_REG_XOUT_H);
    accel_axis.accel_x_out_l = rp2040_i2c_read_byte(ACCEL_REG_XOUT_L);

    accel_axis.accel_y_out_h = rp2040_i2c_read_byte(ACCEL_REG_YOUT_H);
    accel_axis.accel_y_out_l = rp2040_i2c_read_byte(ACCEL_REG_YOUT_L);

    accel_axis.accel_z_out_h = rp2040_i2c_read_byte(ACCEL_REG_ZOUT_H);
    accel_axis.accel_z_out_l = rp2040_i2c_read_byte(ACCEL_REG_ZOUT_L);

    // Check if the sensor has new data ready in the Interrupt Status register
    if(rp2040_i2c_read_byte(INTR_ENABLE_STATUS) & DATA_RDY_EN_BIT){        

        // Combine High/Low registers into signed 16-bit values
        gyro_xout = reg_uniter_8to16(gyro_axis.gyro_x_out_h, gyro_axis.gyro_x_out_l);
        gyro_yout = reg_uniter_8to16(gyro_axis.gyro_y_out_h, gyro_axis.gyro_y_out_l);
        gyro_zout = reg_uniter_8to16(gyro_axis.gyro_z_out_h, gyro_axis.gyro_z_out_l);

        // Combine High/Low registers into signed 16-bit values
        accel_xout = reg_uniter_8to16(accel_axis.accel_x_out_h, accel_axis.accel_x_out_l);
        accel_yout = reg_uniter_8to16(accel_axis.accel_y_out_h, accel_axis.accel_y_out_l);
        accel_zout = reg_uniter_8to16(accel_axis.accel_z_out_h, accel_axis.accel_z_out_l);
    }

    // Store gyroscope values into output struct
    g_data.GYRO_XOUT_V = gyro_xout;
    g_data.GYRO_YOUT_V = gyro_yout;
    g_data.GYRO_ZOUT_V = gyro_zout;

    // Store accelerometer values into output struct
    g_data.ACCEL_XOUT_V = accel_xout;
    g_data.ACCEL_YOUT_V = accel_yout;
    g_data.ACCEL_ZOUT_V = accel_zout;

    return g_data;
}

xy_angles_t mpu6050_get_gyro_angles(void)
{
    raw_out_t raw_data = {0};

    xy_angles_t xy_angles = {0};

    // Read raw sensor values
    raw_data = get_raw_values();

    xy_angles.x_angle_value = mpu6050_get_pitch_x(raw_data);    
    xy_angles.y_angle_value = mpu6050_get_roll_y(raw_data);

    return xy_angles;
}