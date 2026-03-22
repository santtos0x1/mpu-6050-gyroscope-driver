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
        xy_angles_t xy_angles = mpu6050_get_gyro_angles();

        printf("X-Axis: %f", xy_angles.x_angle_value);
        printf("Y-Axis: %f", xy_angles.y_angle_value);

        // 10ms delay (100Hz)
        sleep_ms(10);
    }

    return 0;
}
