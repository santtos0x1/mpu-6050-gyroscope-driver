/**
 * @file main.c
 * @author Ruan
 * @brief Test application for the MPU6050 RP2040 driver
 *
 * This file contains a simple test program used to validate the functionality
 * of the MPU6050 driver implemented for the RP2040 microcontroller.
 *
 * The program initializes the RP2040 hardware, configures the MPU6050 sensor,
 * and continuously reads the gyroscope angles from the device.
 *
 * The sensor status and calculated angles are printed through the standard
 * output interface (USB serial).
 *
 * Project: mpu6050-rp2040-driver
 * Target: RP2040 (Raspberry Pi Pico)
 * Interface: I2C
 *
 * This file is intended only for driver testing and demonstration purposes.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "driver/gyro_driver.h"
#include "pico_driver/i2c_rw_data.h"
#include "pico_driver/error.h"

int main()
{
    pico_err_t err;

    // Initialize standard I/O (USB serial)
    stdio_init_all();

    /**
     * ---------------------------------------------------------
     * RP2040 HARDWARE INITIALIZATION
     * ---------------------------------------------------------
     * Configure I2C peripherals and internal registers required
     * for communication with the MPU6050 sensor.
     */
    err = rp2040_setup_hwr();
    if(err != PICO_OK_T)
    {
        printf("ERROR: %s", pico_err_to_name(err));
        return 1;
    }

    /**
     * ---------------------------------------------------------
     * SENSOR INITIALIZATION
     * ---------------------------------------------------------
     * Configure MPU6050 internal registers required for gyro
     * readings and driver operation.
     */
    setup_driver_registers();

    /**
     * ---------------------------------------------------------
     * MAIN LOOP
     * ---------------------------------------------------------
     * Continuously reads gyro angles from the sensor and checks
     * the sensor connection status.
     */
    while(1)
    {
        xy_angles_t xy_angles = mpu6050_get_gyro_angles();

        err = rp2040_sensor_recon();
        if(err != PICO_OK_T)
        {
            printf("Sensor status: %s", pico_err_to_name(err));
            return 1;
        }
        else
        {
            printf("Sensor status: %s\n", pico_err_to_name(err));
        }

        printf("X-Axis angle: %f\n", xy_angles.x_angle_value);
        printf("Y-Axis angle: %f\n", xy_angles.y_angle_value);

        // 10 ms delay (100 Hz sampling rate)
        sleep_ms(10);
    }

    return 0;
}