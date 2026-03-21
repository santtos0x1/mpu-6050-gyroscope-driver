#include <stdio.h>
#include "pico/stdlib.h"
#include "mpu6050_gyroscope_driver.h"
#include "i2c_rw_data.h"

int main()
{
    stdio_init_all();

    // Starts and configures rp2040 registers and peripherals
    rp2040_setup_hwr();
    
    // Starts and configures sensor registers
    setup_driver_registers();

    while(1)
    {
        // Initialize and gets the sensor values
        gyro_out_t g_data = {0};
        g_data = get_gyro_values();

        // Verify if X-Axis is moving
        if (g_data.GYRO_XOUT_V > 16000 || g_data.GYRO_XOUT_V < -16000) {
            printf("[GYRO] Motion Detected! X-Axis: %d\n", g_data.GYRO_XOUT_V);
        }

        // 10ms delay (100Hz)
        sleep_ms(10);
    }

    return 0;
}
