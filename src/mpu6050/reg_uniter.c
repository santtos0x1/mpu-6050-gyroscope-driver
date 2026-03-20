#include "reg_uniter.h"
#include <stdint.h>

uint16_t reg_uniter_8to16(uint8_t reg_h, uint8_t reg_l)
{
    // 0000000000000000
    uint16_t _16_bit_reg = 0x00;

    _16_bit_reg |= reg_h << 8;
    _16_bit_reg |= reg_l;

    return _16_bit_reg;
}