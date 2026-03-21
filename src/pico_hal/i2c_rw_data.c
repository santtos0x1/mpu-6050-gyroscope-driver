#include <stdint.h>
#include "i2c_rw_data.h"
#include "registers.h"

#define IO_BANK0_BIT (1 << 5)
#define PADS_BANK0_BIT (1 << 8)
#define I2C0_BIT (1 << 3)    
#define RESETSREG_BIT_SET ((volatile uint32_t *)(RESETS_BASE)) 

void rp2040_setup_hwr(void)
{
    bit_bank_t bit_bank;

    // Sets bits to 1 in a 32-bit bitset
    bit_bank.bit_arr |= (IO_BANK0_BIT | PADS_BANK0_BIT);

    // Reset bits from reset register
    *RESETSREG_BIT_SET &= ~(bit_bank.bit_arr);
    while(!IS_RESET_DONE(bit_bank)){}

    // Resets bitset from struct
    memset(&bit_bank, 0, sizeof(bit_bank_t));


}

uint8_t rp2040_i2c_read_byte(uint8_t mpu_addr, uint8_t reg_byte)
{

}