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

### Data Structure

Instead of raw arrays, this driver uses a structured approach for better code clarity:

```c
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} gyro_data_t;
```

## Usage example

```c
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
        // Initialize and gets the sensor values
        gyro_out_t g_data = {0};
        g_data = get_gyro_values();

        // Verify if X-Axis is moving
        if (g_data.GYRO_XOUT_V > 16000 || g_data.GYRO_XOUT_V < -16000) {
            printf("[GYRO] Motion Detected! X-Axis: %d\n", g_data.GYRO_XOUT_V);
        }

        // 10ms delay (100Hz)
        sleep_ms(10);
    }

    return 0;
}
```
