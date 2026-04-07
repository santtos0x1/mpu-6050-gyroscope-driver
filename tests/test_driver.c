#include <assert.h>
#include <stdint.h>
#include "test_driver.h"
#include "pico_driver/error.h"
#include "test_registers.h"
#include "test_mpu6050.h"
#include "driver/registers.h"


void test_whoami_ack_is_set(void)
{
    test_sensor_reset();
    test_mpu_setup();

    uint8_t whoami_reg_v = test_i2c_r_byte(0x75);

    assert(whoami_reg_v == 0x68);
}

void test_reset_pwr_mgmt1_reg_is_set(void)
{
    test_sensor_reset();

    test_mpu_setup();

    uint8_t expected_data = 0x68;

    assert(test_registers[PWR_MGMT_1_REG] == expected_data);
}

void test_config_dlpf_cfg2_is_set(void)
{
    test_sensor_reset();

    test_mpu_setup();
    
    uint8_t expected_data = 0x02;

    assert(test_registers[CONFIGURATION_REG] == expected_value);
}

void test_gyro_config_sensibility_500_is_set(void)
{
    test_sensor_reset();

    test_mpu_setup();

    uint8_t expected_data = 0x08;

    assert(test_registers[GYRO_CONFIGURATION_REG] == expected_data);
}

void test_accel_config_full_scale_range_4g_is_set(void)
{
    test_sensor_reset();

    test_mpu_setup();

    uint8_t expected_value = 0x08;

    assert(test_registers[ACCEL_CONFIGURATION_REG] == expected_value);
}

void test_intr_enable_data_rdy_en_is_set(void)
{
    test_sensor_reset();

    test_mpu_setup();

    uint8_t expected_data = 0x01;

    assert(test_registers[INTR_ENABLE_REG] == expected_data);
}

void test_smprt_div_1kHz_value_is_set(void)
{
    test_sensor_reset();

    test_mpu_setup();

    uint8_t expected_value = 0x09;

    assert(test_registers[SMPRT_DIV_REG] == expected_value);
}