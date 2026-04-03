# RP2040 MPU-6050 Bare Metal Driver

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Microcontroller](https://img.shields.io/badge/Microcontroller-RP2040-orange.svg)
![License](https://img.shields.io/badge/License-Apache_2.0-green.svg)

A lightweight, low-level C driver for the **MPU-6050** (Gyroscope) running on the **RP2040** without using high-level SDK libraries.

## 🚀 Overview

This project focuses on **Bare Metal** programming, interacting directly with hardware registers to control I2C communication. It is designed for high performance and educational purposes in embedded systems development.

### Key Features

- **Direct Register Access:** Uses memory-mapped I2C0 registers of the RP2040.
- **Burst Read Support:** Reads 6-axis data (X, Y, Z) in a single I2C transaction for better synchronization.
- **Fast Mode:** Configured for 400kHz I2C communication.
- **Pointer-Based Logic:** Optimized using `volatile` pointers for hardware safety.
- **Accelerometer-based Compensation:** Uses accelerometer data to correct gyroscope drift and provide smoother values.

## 🛠️ Hardware Setup

To use this driver, connect the MPU-6050 to the RP2040 as follows:

| MPU-6050 Pin | RP2040 Pin | Function | Description |
| :--- | :--- | :--- | :--- |
| **VCC** | **3V3 (Out)** | Power | 3.3V Power Supply |
| **GND** | **GND** | Ground | Common Ground |
| **SCL** | **GPIO 15** | I2C0 SCL | Serial Clock Line |
| **SDA** | **GPIO 14** | I2C0 SDA | Serial Data Line |
| **AD0** | **GND** | Address | Sets I2C Address to `0x68` |

- Note: The **WHO_AM_I** register (address **0x75**) should return a default value of **0x68**. This is used to verify the I2C connection and identify the device

## 💻 Technical Implementation

The driver implements the I2C protocol manually by managing the `IC_DATA_CMD` FIFO and monitoring the `IC_STATUS` register for **RFNE** (Receive FIFO Not Empty) flags.

## Usage example

```c
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

        printf("X-Axis angle: %f", xy_angles.x_angle_value);
        printf("Y-Axis angle: %f", xy_angles.y_angle_value);

        // 10ms delay (100Hz)
        sleep_ms(10);
    }

    return 0;
}
```

## Project Structure

mpu6050-rp2040-driver/
├── src/
│   ├── mpu6050/
│   │   ├── gyro_driver.c
│   │   └── gyro_filter.c
│   ├── pico_hal/
│   │   ├── i2c_rw_data.c
│   │   ├── time.c
│   │   └── error.c
│   └── reg_utils.c
│
├── include/
│   ├── driver/
│   │   ├── gyro_driver.h
│   │   └── gyro_filter.h
│   └── pico_driver/
│       ├── i2c_rw_data.h
│       ├── time.h
│       └── error.h
│
├── CMakeLists.txt
├── README.md
├── LICENSE
├── CONTRIBUTING.md
└── CODE_OF_CONDUCT.md
