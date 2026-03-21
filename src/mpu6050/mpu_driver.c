#include "mpu_driver.h"
#include "registers.h"
#include "string.h"
#include "reg_uniter.h"
#include "i2c_rw_data.h"

/* --- Configuration Bits & Values --- */

// Sets Gyro Full Scale Range to ±500 °/s
#define GYRO_SENSIBILITY_500_BIT 0x08

// Enables Data Ready interrupt bit
#define DATA_RDY_EN_BIT 0x01

// Default I2C address/ID of MPU-6050
#define WHOAMI_REG_DEFAULT_V_BIT 0x68

// Sets Digital Low Pass Filter to Level 2 (98Hz)
#define DLPF_CFG_2_BIT 0x02

// Sets Sample Rate Divider to 9 (100Hz output)
#define SMPRT_DIV_VALUE_BIT 0x09

// Clear or reset register bits
#define RESET_ALL_BITS 0x00

/* --- Register Access Macros (Pointers & Dereferencing) --- */

// Pointer to device ID register
#define R_WHOAMI_REG ((volatile uint8_t *)(WHOAMI_REG))

// Pointer to interrupt status register
#define R_INTR_ENABLE_STATUS ((volatile uint8_t *)(INTR_ENABLE_STATUS))

// Access to write sample rate divider
#define W_SMPRT_DIV (*(volatile uint8_t *)(SMPRT_DIV_REG))

// Access to write interrupt enable register
#define W_INTR_ENABLE (*(volatile uint8_t *)(INTR_ENABLE_REG))

// Access to write power management 1
#define W_PWR_MGMT_1 (*(volatile uint8_t *)(PWR_MGMT_1_REG))

// Access to write general configuration (DLPF)
#define W_CONFIGURATION (*(volatile uint8_t *)(CONFIGURATION_REG))

// Access to write gyro scale config
#define W_GYRO_CONFIGURATION (*(volatile uint8_t *)(GYRO_CONFIGURATION_REG))

void set_registers(void)
{
    // Waits until find sensor
    while(!(*R_WHOAMI_REG & WHOAMI_REG_DEFAULT_V_BIT)){};

    // Resets all bits from PWR management register
    W_PWR_MGMT_1 &= RESET_ALL_BITS;

    // Sets Digital Low Pass Filter (DLPF) to mode 2:
    // Bandwidth: 98Hz
    // Delay: 2.8ms
    // Fs: 1kHz
    W_CONFIGURATION |= DLPF_CFG_2_BIT;

    // Configures gyroscope sensibility to ±500°/s
    W_GYRO_CONFIGURATION |= GYRO_SENSIBILITY_500_BIT;

    // Enables interrupt when data is ready
    W_INTR_ENABLE |= DATA_RDY_EN_BIT;

    // Sets 0x09 to divide to set sample rate value to 100hz, getting 1 sample each 10ms;
    W_SMPRT_DIV |= SMPRT_DIV_VALUE_BIT;
}

gyro_out_t get_gyro_values(void)
{
    uint16_t gyro_xout, gyro_yout, gyro_zout;
    gyro_axis_hl_v gyro_axis;
    gyro_out_t g_data;

    gyro_axis.gyro_x_out_h = GYRO_REG_XOUT_H;
    gyro_axis.gyro_x_out_l = GYRO_REG_XOUT_L;
    gyro_axis.gyro_y_out_h = GYRO_REG_YOUT_H;
    gyro_axis.gyro_y_out_l = GYRO_REG_YOUT_L;
    gyro_axis.gyro_z_out_h = GYRO_REG_ZOUT_H;
    gyro_axis.gyro_z_out_l = GYRO_REG_ZOUT_L;

    if(*R_INTR_ENABLE_STATUS & DATA_RDY_EN_BIT){        
        // Unites two 8-bits registers in one of 16-bit for each axis
        gyro_xout = reg_uniter_8to16(gyro_axis.gyro_x_out_h, gyro_axis.gyro_x_out_l);
        gyro_yout = reg_uniter_8to16(gyro_axis.gyro_y_out_h, gyro_axis.gyro_y_out_l);
        gyro_zout = reg_uniter_8to16(gyro_axis.gyro_z_out_h, gyro_axis.gyro_z_out_l);
    }

    g_data.GYRO_XOUT_V = gyro_xout;
    g_data.GYRO_YOUT_V = gyro_yout;
    g_data.GYRO_ZOUT_V = gyro_zout;

    return g_data;
}
