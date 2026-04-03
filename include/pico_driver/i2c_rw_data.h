#ifndef I2C_RW_DATA_H
#define I2C_RW_DATA_H

#include <stdint.h>

/*
 * Bit bank structure used to manipulate hardware control
 * registers that operate as bitsets (reset control, clock
 * enable, peripheral masks, etc).
 *
 * Each bit in bit_arr corresponds to a hardware
 * peripheral or control flag.
 */
typedef struct {
    uint32_t bit_arr;
} bit_bank_t;

/*
 * @brief Initializes and configures the RP2040 hardware required
 * for I2C communication.
*/
 void rp2040_setup_hwr(void);

/*
 * @brief Reads a single byte from a sensor register over I2C.
*/
uint8_t rp2040_i2c_read_byte(uint8_t sensor_reg_addr);

/*
 * @brief Writes a single byte to a sensor register over I2C.
*/
uint8_t rp2040_i2c_write_byte(uint8_t sensor_reg_addr, uint8_t data);

/*
 * @brief Returns the value in WHOAMI_REG (0x75), expected value: 0x68
*/
uint8_t rp2040_sensor_recon(void);

#endif // !I2C_RW_DATA_H