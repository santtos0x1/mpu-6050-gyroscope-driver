#include "reg_utils.h"
#include "pico_driver/registers.h"
#include <stdint.h>

// Resets done register
#define RESETS_DONE RESETS_BASE + 0x8

// Resets Digital IO (GPIOs)
#define IO_BANK0_BIT (1 << 5)

// Resets physical pin controls (Pull-ups/downs)
#define PADS_BANK0_BIT (1 << 8)

// Resets the I2C0 hardware block
#define I2C0_BIT (1 << 3)

// Checks if bit reset is done
uint8_t is_reset_done(bit_bank_t bit_bank)
{
    return ((*(volatile uint32_t *)(RESETS_DONE)) & (IO_BANK0_BIT | PADS_BANK0_BIT | I2C0_BIT));
}

// Combines 2 8-bit registers into one of 16-bit
uint16_t reg_uniter_8to16(uint8_t reg_h, uint8_t reg_l)
{
    // Initialize an empty 16-bit variable (00000000 00000000)
    uint16_t _16_bit_reg = 0x00;

    // Shift the High byte 8 positions to the left 
    // Example: 00000000 11111111 becomes 11111111 00000000
    _16_bit_reg |= reg_h << 8;
    
    // Merge the Low byte into the 8 empty bits on the right using OR
    // Example: 11111111 00000000 | 00000000 10101010 = 11111111 10101010
    _16_bit_reg |= reg_l;

    return _16_bit_reg;
}