#include <stdint.h>

#include "pico_driver/registers.h"
#include "pico_driver/time.h"

// Define the memory address for the system timer (low 32 bits)
#define TIMERAWL ((volatile uint32_t *)(TIMER_BASE + 0x28))

uint32_t get_time_us(void)
{
    // Read and return the value from the timer register
    return *TIMERAWL;
}

void delay_cycle(volatile uint32_t cycles)
{
    while(--cycles != 0)
    {
        // One cycle jump
        __asm volatile("nop");
    }
}
