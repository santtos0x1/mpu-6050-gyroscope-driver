#include <stdio.h>
#include "pico/stdlib.h"
#include "driver/gyro_driver.h"
#include "pico_driver/i2c_rw_data.h"
#include "pico_driver/error.h"

int main()
{
    pico_err_t err;

    stdio_init_all();

    // Starts and configures rp2040 registers and peripherals
    err = rp2040_setup_hwr();
    if(err != PICO_OK_T)
    {
        printf("ERROR: %s", pico_err_to_name(err));
        return 1;
    }
    
    // Starts and configures sensor registers
    setup_driver_registers();

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

        // 10ms delay (100Hz)
        sleep_ms(10);
    }

    return 0;
}
