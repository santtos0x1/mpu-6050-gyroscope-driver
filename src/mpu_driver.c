#include "mpu_driver.h"
#include "registers.h"

#define GYRO_SENSIBILITY_500 0x08
#define DATA_RDY_EN          0x01
#define WHOAMI_REG_DEFAULT_V 0x68

void set_registers(void)
{
    // Waits until find sensor
    while((*R_WHOAMI_REG) != WHOAMI_REG_DEFAULT_V){};

    // Resets all bits from PWR management register
    W_PWR_MGMT_1 &= 0x00;

    // Configures gyroscope sensibility to ±500°/s
    W_GYRO_CONFIGURATION |= GYRO_SENSIBILITY_500;

    // Enables interrupt when data is ready
    W_INTR_ENABLE |= DATA_RDY_EN;
}

gyro_out_t get_gyro_values(void)
{
    
}
