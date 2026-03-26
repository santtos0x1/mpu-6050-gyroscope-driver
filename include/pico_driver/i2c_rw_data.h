#ifndef I2C_RW_DATA_H
#define I2C_RW_DATA_H

#include <stdint.h>

typedef struct {
    uint32_t bit_arr;
} bit_bank_t;

void rp2040_setup_hwr(void);

uint8_t rp2040_i2c_read_byte(uint8_t sensor_reg_addr);

uint8_t rp2040_sensor_recon(void);

#endif // !I2C_RW_DATA_H