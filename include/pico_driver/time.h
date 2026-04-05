#ifndef TIME_H
#define TIME_H

#include <stdint.h>

// Converts microseconds to CPU cycles (125 MHz clock)
#define US_TO_CYCLES(x) (x * 125)

// Converts microseconds to milliseconds
#define US_TO_MS(x) (x * 1000)

// Converts milliseconds to CPU cycles
#define MS_TO_CYCLES(x) (x * 125000)

// Returns current time in microseconds
uint32_t get_time_us(void);

// Busy-wait delay using CPU cycles
void delay_clk_cycle(volatile uint32_t cycles);

#endif // !TIME_H