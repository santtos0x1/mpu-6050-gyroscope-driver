#include "pico_driver/error.h"
#include "pico_driver/registers.h"
#include <stdint.h>

// Main switch: Enable (1) or Disable (0) I2C
#define IC_ENABLE ((volatile uint32_t *)(I2C1_BASE + 0x6c))

// Restarts rp2040 I2C communication
void pico_restart_i2c(void)
{
    // Disables I2C
    *IC_ENABLE &= ~0x1;

    // Enables I2C
    *IC_ENABLE |= 0x1;
}