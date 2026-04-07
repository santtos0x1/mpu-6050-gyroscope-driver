#ifndef TEST_REGISTERS_H
#define TEST_REGISTERS_H

#include <stdint.h>
#include "pico_driver/error.h"

void test_sensor_reset(void);

uint8_t test_i2c_r_byte(uint8_t reg_addr);

pico_err_t test_i2c_w_byte(uint8_t reg_addr, uint8_t data);

#endif