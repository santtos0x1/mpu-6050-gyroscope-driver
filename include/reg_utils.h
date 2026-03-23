#ifndef REG_UTILS_H
#define REG_UTILS_H

#include <stdint.h>
#include "pico_driver/i2c_rw_data.h"

uint8_t is_reset_done(bit_bank_t bit_bank);

uint16_t reg_uniter_8to16(uint8_t reg_h, uint8_t reg_l);

#endif // !REG_UTILS_H
