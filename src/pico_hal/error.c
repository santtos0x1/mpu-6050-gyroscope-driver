#include <stdint.h>

#include "pico_driver/error.h"
#include "pico_driver/registers.h"

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

char *pico_err_to_name(pico_err_t error)
{
    switch(error)
    {
        case PICO_OK_T: return "PICO_OK_T";
        case PICO_TIMEOUT_ERR_T: return "PICO_TIMEOUT_ERR_T";
        case PICO_GENERIC_ERR_T: return "PICO_GENERIC_ERR_T";
    }
}