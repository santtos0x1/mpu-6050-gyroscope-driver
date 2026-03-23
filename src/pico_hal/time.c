#include <stdint.h>
#include "pico_driver/registers.h"
#include "pico_driver/time.h"

// Define the memory address for the system timer (low 32 bits)
#define TIMERAWL ((volatile uint32_t *)(TIMER_BASE + 0x28))

/**
 * Get current system time in microseconds
 */
uint32_t get_time_us(void)
{
    // Read and return the value from the timer register
    return *TIMERAWL;
}