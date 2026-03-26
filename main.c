#include <stdio.h>
#include "pico/stdlib.h"
#include "driver/gyro_driver.h"
#include "pico_driver/i2c_rw_data.h"

int main()
{
    stdio_init_all();

    // Starts and configures rp2040 registers and peripherals
    rp2040_setup_hwr();
    
    // Starts and configures sensor registers
    setup_driver_registers();

    while(1)
    {
        xy_angles_t xy_angles = mpu6050_get_gyro_angles();

        printf("WHOAMI Value: %d\n", rp2040_sensor_recon());
        printf("X-Axis angle: %f\n", xy_angles.x_angle_value);
        printf("Y-Axis angle: %f\n", xy_angles.y_angle_value);

        // 10ms delay (100Hz)
        sleep_ms(10);
    }

    return 0;
}
