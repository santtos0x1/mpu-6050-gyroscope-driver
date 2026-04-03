#ifndef ERROR_H
#define ERROR_H

/*
 * Enumeration of error codes used across the driver.
 */
typedef enum {
    /*
     * Indicates that an I2C operation exceeded the expected
     * waiting time (timeout occurred).
     */
    I2C_TIMEOUT_ERR
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

#endif // !ERROR_H