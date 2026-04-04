#ifndef I2C_RW_DATA_H
#define I2C_RW_DATA_H

#include <stdint.h>

#include "pico_driver/error.h"

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
pico_err_t rp2040_setup_hwr(void);

/*
 * @brief Reads a single byte from a sensor register over I2C.
*/
uint8_t rp2040_i2c_read_byte(uint8_t sensor_reg_addr);

/*
 * @brief Writes a single byte to a sensor register over I2C.
*/
uint8_t rp2040_i2c_write_byte(uint8_t sensor_reg_addr, uint8_t data);

/*
 * @brief Returns PICO_OK_T if founds sensor and PICO_SENSOR_ACK_ERROR if not found.
*/
pico_err_t rp2040_sensor_recon(void);

#endif // !I2C_RW_DATA_H