#include "test_registers.h"
#include "string.h"
#include "test_mpu6050.h"

void test_sensor_reset(void)
{
    memset(test_registers, 0, sizeof(uint8_t));
}

uint8_t test_i2c_r_byte(uint8_t reg_addr)
{
    return test_registers[reg_addr];
}

pico_err_t test_i2c_w_byte(uint8_t reg_addr, uint8_t data)
{
    test_registers[reg_addr] = data;

    return PICO_OK_T;
}

void test_set_register(uint8_t reg_addr, uint8_t data)
{   
    test_registers[reg_addr] = data;
}