#ifndef MPU6050_REGISTERS_H
#define MPU6050_REGISTERS_H

// ---------- MPU-6050 INTERNAL REGISTERS (Accessed via I2C) ----------
#define WHOAMI_REG 0x75 // Reads the sensor's ID (should return 0x68). Useful for sanity checks.
#define GYRO_CONFIGURATION_REG 0x1B // Configures the gyroscope full-scale range (e.g., ±250, ±500 °/s).
#define PWR_MGMT_1_REG 0x6B // Power management. Used to wake the sensor from sleep mode and select the clock source.
#define CONFIGURATION_REG 0x1A // General configuration, mainly the Digital Low Pass Filter (DLPF) to reduce noise.
#define INTR_ENABLE_REG 0x38 // Enables hardware interrupts (e.g., Data Ready interrupt).
#define INTR_ENABLE_STATUS 0x3A // Reads the interrupt status to check which interrupt was triggered.
#define SMPRT_DIV_REG 0x19 // Sample Rate Divider. Sets the data output rate (Hz).

// Gyroscope Data Registers (16-bit values split into High and Low bytes)
#define GYRO_REG_XOUT_H 0x43 // X-Axis (High byte)
#define GYRO_REG_XOUT_L 0x44 // X-Axis (Low byte)
#define GYRO_REG_YOUT_H 0x45 // Y-Axis (High byte)
#define GYRO_REG_YOUT_L 0x46 // Y-Axis (Low byte)
#define GYRO_REG_ZOUT_H 0x47 // Z-Axis (High byte)
#define GYRO_REG_ZOUT_L 0x48 // Z-Axis (Low byte)

#endif