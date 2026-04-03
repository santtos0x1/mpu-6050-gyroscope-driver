#ifndef TIME_H
#define TIME_H

#include <stdint.h>

#define US_TO_CYCLES(x) (x * 125)
#define US_TO_MS(x) (x * 1000)
#define MS_TO_CYCLES(x) (x * 125000)

uint32_t get_time_us(void);

void delay_cycle(volatile uint32_t cycles);

#endif // !TIME_H