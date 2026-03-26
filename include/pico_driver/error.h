#ifndef ERROR_H
#define ERROR_H

typedef enum {
    I2C_TIMOUT_ERR
} pico_err_t;

void pico_restart_i2c(void);

#endif // !ERROR_H