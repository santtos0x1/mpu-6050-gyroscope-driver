#ifndef MPU_DRIVER_H
#define MPU_DRIVER_H

#include <stdint.h>

typedef struct {
    uint16_t GYRO_XOUT_V;
    uint16_t GYRO_YOUT_V;
    uint16_t GYRO_ZOUT_V;
} gyro_out_t;

void set_registers(void);

gyro_out_t get_gyro_values(void);

uint16_t reg_uniter(uint8_t reg_H, uint8_t reg_L);

#endif

