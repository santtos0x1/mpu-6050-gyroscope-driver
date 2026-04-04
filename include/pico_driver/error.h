#ifndef ERROR_H
#define ERROR_H

/*
 * Enumeration of error codes used across the driver.
 */
typedef enum {
    PICO_OK_T = 0,
    PICO_GENERIC_ERR_T = -1,
    PICO_TIMEOUT_ERR_T = -2,
    PICO_SENSOR_ACK_ERROR = -3,
} pico_err_t;

/*
 * Restarts the I2C peripheral.
 *
 * This function is intended to recover the I2C controller from
 * error states such as timeouts, bus lockups, or unfinished
 * transactions. It typically resets and reinitializes the I2C
 * hardware so communication can resume normally.
 */
void pico_restart_i2c(void);

char *pico_err_to_name(pico_err_t error);

#endif // !ERROR_H