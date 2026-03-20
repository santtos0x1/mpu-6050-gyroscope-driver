#ifndef REGISTERS_H
#define REGISTERS_H

#define WHOAMI_REG             0x75
#define GYRO_CONFIGURATION_REG 0x1B
#define PWR_MGMT_1_REG         0x6B
#define CONFIGURATION_REG      0x1A
#define INTR_ENABLE_REG        0x38
#define INTR_ENABLE_STATUS     0x3A

#define GYRO_REG_XOUT_H        0x43
#define GYRO_REG_XOUT_L        0x44
#define GYRO_REG_YOUT_H        0x45
#define GYRO_REG_YOUT_L        0x46
#define GYRO_REG_ZOUT_H        0x47
#define GYRO_REG_ZOUT_L        0x48

#define R_WHOAMI_REG         ((volatile uint8_t *)(WHOAMI_REG))
#define W_INTR_ENABLE        (*(volatile uint8_t *)(INTR_ENABLE_REG))
#define W_PWR_MGMT_1         (*(volatile uint8_t *)(PWR_MGMT_1_REG))
#define W_CONFIGURATION      (*(volatile uint8_t *)(CONFIGURATION_REG))
#define W_GYRO_CONFIGURATION (*(volatile uint8_t *)(GYRO_CONFIGURATION_REG))

#define R_GYRO_XOUT_H        ((volatile uin8_t *)(GYRO_REG_XOUT_H))
#define R_GYRO_XOUT_L        ((volatile uin8_t *)(GYRO_REG_XOUT_L))
#define R_GYRO_YOUT_H        ((volatile uin8_t *)(GYRO_REG_YOUT_H))
#define R_GYRO_YOUT_L        ((volatile uin8_t *)(GYRO_REG_YOUT_L))
#define R_GYRO_ZOUT_H        ((volatile uin8_t *)(GYRO_REG_ZOUT_H))
#define R_GYRO_ZOUT_L        ((volatile uin8_t *)(GYRO_REG_ZOUT_L))

#endif //! REGISTERS_H

