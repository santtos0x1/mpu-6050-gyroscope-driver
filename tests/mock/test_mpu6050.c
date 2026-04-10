#include "test_mpu6050.h"
#include "test_registers.h"
#include "driver/registers.h"
#include <stdint.h>

void test_mpu_setup(void)
{
    // WHOAMI_REG default value
    test_registers[WHOAMI_REG] = 0x68;

    // PWR_MGMT
    test_i2c_w_byte(PWR_MGMT_1_REG, 0x00);

    // CONFIGURATION
    test_i2c_w_byte(CONFIGURATION_REG, 0x02);

    // GYRO CONFIG
    test_i2c_w_byte(GYRO_CONFIGURATION_REG, 0x08);

    // ACCEL CONFIG
    test_i2c_w_byte(ACCEL_CONFIGURATION_REG, 0x08);

    // INTR_ENABLE
    test_i2c_w_byte(INTR_ENABLE_REG, 0x01);

    // SMPRT_DIV
    test_i2c_w_byte(SMPRT_DIV_REG, 0x09);
}

uint16_t test_get_gyro(uint8_t reg_addr_h, uint8_t reg_addr_l)
{
    uint8_t gyro_x_h_reg = test_registers[reg_addr_h];
    uint8_t gyro_x_l_reg = test_registers[reg_addr_l];

    uint16_t value = (gyro_x_h_reg << 8) | gyro_x_l_reg;
    
    return value;
}

// Accel values

uint16_t test_get_accel(uint8_t reg_addr_h, uint8_t reg_addr_l)
{
    uint8_t accel_x_h_reg = test_registers[reg_addr_h];
    uint8_t accel_x_l_reg = test_registers[reg_addr_l];

    uint16_t value = (accel_x_h_reg << 8) | accel_x_l_reg;
    
    return value;
}