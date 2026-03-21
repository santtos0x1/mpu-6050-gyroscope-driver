#ifndef MPU_DRIVER_H
#define MPU_DRIVER_H

#include <stdint.h>

typedef struct {
    uint16_t GYRO_XOUT_V;
    uint16_t GYRO_YOUT_V;
    uint16_t GYRO_ZOUT_V;
} gyro_out_t;

typedef struct {
    uint8_t gyro_x_out_h;
    uint8_t gyro_x_out_l;
    uint8_t gyro_y_out_h;
    uint8_t gyro_y_out_l;
    uint8_t gyro_z_out_h;
    uint8_t gyro_z_out_l;
} gyro_axis_hl_v;

void set_registers(void);

gyro_out_t get_gyro_values(void);


#endif

