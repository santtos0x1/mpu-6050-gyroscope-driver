#ifndef MPU6050_REGISTERS_H
#define MPU6050_REGISTERS_H

// ---------- MPU-6050 INTERNAL REGISTERS ----------
// Reads the sensor's ID (should return 0x68). Useful for sanity checks.
#define WHOAMI_REG 0x75

// Configures the gyroscope full-scale range (e.g., ±250, ±500 °/s).
#define GYRO_CONFIGURATION_REG 0x1B

// Power management. Used to wake the sensor from sleep mode and select the clock source.
#define PWR_MGMT_1_REG 0x6B

// General configuration, mainly the Digital Low Pass Filter (DLPF) to reduce noise.
#define CONFIGURATION_REG 0x1A

// Enables hardware interrupts (e.g., Data Ready interrupt).
#define INTR_ENABLE_REG 0x38

// Reads the interrupt status to check which interrupt was triggered.
#define INTR_ENABLE_STATUS 0x3A

// Sample Rate Divider. Sets the data output rate (Hz).
#define SMPRT_DIV_REG 0x19

// Gyroscope Data Registers (16-bit values split into High and Low bytes)
// X-Axis (High byte)
#define GYRO_REG_XOUT_H 0x43

// X-Axis (Low byte)
#define GYRO_REG_XOUT_L 0x44

// Y-Axis (High byte)
#define GYRO_REG_YOUT_H 0x45

// Y-Axis (Low byte)
#define GYRO_REG_YOUT_L 0x46

// Z-Axis (High byte)
#define GYRO_REG_ZOUT_H 0x47

// Z-Axis (Low byte)
#define GYRO_REG_ZOUT_L 0x48

#endif