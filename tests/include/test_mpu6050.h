#ifndef TEST_MPU6050_H
#define TEST_MPU6050_H

#include <stdint.h>

uint8_t test_registers[128];

void test_mpu_setup(void);

// Gyro values

uint16_t test_get_gyro(uint8_t reg_addr_h, uint8_t reg_addr_l);

// Accel values

uint16_t test_get_accel(uint8_t reg_addr_h, uint8_t reg_addr_l);

#endif