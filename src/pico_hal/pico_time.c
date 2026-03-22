#include "pico_time.h"
#include <stdint.h>
#include "pico_registers.h"

#define TIMERAWL ((volatile uint32_t *)(TIMER_BASE + 0x28))

uint32_t get_time_us(void)
{
    return TIMERAWL;
}