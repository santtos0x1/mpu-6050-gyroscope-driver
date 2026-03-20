#include "mpu_driver.h"
#include "registers.h"
#include "string.h"

#define GYRO_SENSIBILITY_500 0x08
#define DATA_RDY_EN          0x01
#define WHOAMI_REG_DEFAULT_V 0x68
#define DLPF_CFG_2           0x0A
#define SMPRT_DIV_VALUE      0x09

uint16_t reg_uniter(uint8_t reg_h, uint8_t reg_l)
{
    // 0000000000000000
    uint16_t _16_bit_reg = 0x00;

    _16_bit_reg |= reg_h << 8;
    _16_bit_reg |= reg_l;

    return _16_bit_reg;
}

void set_registers(void)
{
    // Waits until find sensor
    while(!(*R_WHOAMI_REG & WHOAMI_REG_DEFAULT_V)){};

    // Resets all bits from PWR management register
    W_PWR_MGMT_1 &= 0x00;

    // Sets Digital Low Pass Filter (DLPF) to mode 2:
    // Bandwidth: 98Hz
    // Delay: 2.8ms
    // Fs: 1kHz
    W_CONFIGURATION |= DLPF_CFG_2;

    // Configures gyroscope sensibility to ±500°/s
    W_GYRO_CONFIGURATION |= GYRO_SENSIBILITY_500;

    // Enables interrupt when data is ready
    W_INTR_ENABLE |= DATA_RDY_EN;

    // Sets 0x09 to divide to set sample rate value to 100hz, getting 1 sample each 10ms;
    W_SMPRT_DIV |= SMPRT_DIV_VALUE;
}

gyro_out_t get_gyro_values(void)
{
    gyro_out_t g_data;

    // Clears struct before gets a new sample data
    memcpy(&g_data, 0, sizeof(g_data));

    if(*R_INTR_ENABLE_STATUS & 0x01){
        // Unites two 8-bits registers in one of 16-bit for each vector
        uint16_t gyro_xout = reg_uniter(*R_GYRO_XOUT_H, *R_GYRO_XOUT_L);
        uint16_t gyro_yout = reg_uniter(*R_GYRO_YOUT_H, *R_GYRO_YOUT_L);
        uint16_t gyro_zout = reg_uniter(*R_GYRO_ZOUT_H, *R_GYRO_ZOUT_L);
        
        // Sets value to struct
        g_data.GYRO_XOUT_V |= gyro_xout;
        g_data.GYRO_YOUT_V |= gyro_yout;
        g_data.GYRO_ZOUT_V |= gyro_zout;
    }

    return g_data;
}
