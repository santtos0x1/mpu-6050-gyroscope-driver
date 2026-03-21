#include <stdint.h>
#include "i2c_rw_data.h"
#include "registers.h"

// --- Reset and Peripheral Control Bits ---
#define IO_BANK0_BIT (1 << 5)       // Resets or enables Digital IO (GPIOs)
#define PADS_BANK0_BIT (1 << 8)     // Resets or enables physical pin controls (Pull-ups/downs)
#define I2C0_BIT (1 << 3)           // Resets or enables the I2C0 hardware block

// --- IC_CON Register Configuration (I2C Control) ---
#define FAST_MODE (2 << 1)          // Sets I2C speed to 400kHz (bits 2:1)
#define MASTER_MODE (1 << 0)        // Sets RP2040 as the Master of the bus (bit 0)
#define SLAVE_MODE_DISABLE (1 << 6) // Disables Slave mode to prevent bus conflicts (bit 6)

// --- System and Hardware Control Registers ---
#define RESETSREG_BIT_SET ((volatile uint32_t *)(RESETS_BASE)) // Register to take peripherals out of reset
#define GPIO14_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x074)) // Configures GPIO14 for I2C SDA function
#define GPIO15_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x07C)) // Configures GPIO15 for I2C SCL function
#define CLK_PERI_CTRL (*(volatile uint32_t *)(CLOCKS_BASE + 0x48))  // Enables the clock signal for peripherals

// --- I2C0 Specific Registers ---
#define IC_ENABLE (*(volatile uint32_t *)(I2C0_BASE + 0x6c))    // Main switch: Enable (1) or Disable (0) I2C
#define IC_FS_SCL_HCNT (*(volatile uint32_t *)(I2C0_BASE + 0x1c)) // Clock cycles for SCL HIGH level in Fast Mode
#define IC_FS_SCL_LCNT (*(volatile uint32_t *)(I2C0_BASE + 0x20)) // Clock cycles for SCL LOW level in Fast Mode
#define IC_CON (*(volatile uint32_t *)(I2C0_BASE + 0x00))       // Main configuration register (Mode, Speed, etc.)
#define IC_SAR (*(volatile uint32_t *)(I2C0_BASE + 0x08))       // Slave Address: The address of the target sensor (0x68)
#define IC_DATA_CMD (*(volatile uint32_t *)(I2C0_BASE + 0x10))  // Data buffer: Used to send commands and read received bytes
#define IC_STATUS (*(volatile uint32_t *)(I2C0_BASE + 0x70))    // Hardware status: Shows if the buffer is empty or full

void rp2040_setup_hwr(void)
{
    bit_bank_t bit_bank;

    // Enabling clk_peri
    CLK_PERI_CTRL |= (1 << 11);

    // Sets bits to 1 in a 32-bit bitset
    bit_bank.bit_arr |= (IO_BANK0_BIT | PADS_BANK0_BIT | I2C0_BIT);

    // Reset bits from reset register
    *RESETSREG_BIT_SET &= ~(bit_bank.bit_arr);
    while(!IS_RESET_DONE(bit_bank)){}

    // Resets bitset from struct
    memset(&bit_bank, 0, sizeof(bit_bank_t));

    // Setting GPIO14 and GPIO15 to I2C
    GPIO14_CTRL = 0x03; // I2C SDA
    GPIO15_CTRL = 0x03; // I2C SCL

    // Disables I2C
    IC_ENABLE &= ~0x1;

    // 120 + 192 = 312 clock cycles
    IC_FS_SCL_HCNT = 120;
    IC_FS_SCL_LCNT = 192;

    // I2C configuration
    IC_CON = (FAST_MODE | MASTER_MODE | SLAVE_MODE_DISABLE);

    // Sets slave address register to 0x68 (MPU-6050 default address)
    IC_SAR = 0x68;

    // Enables I2C
    IC_ENABLE |= 0x1;
}

uint8_t rp2040_i2c_read_byte(uint8_t sensor_reg_addr)
{
    // Points to the sensor address  
    IC_DATA_CMD = sensor_reg_addr;

    // Sends read command
    IC_DATA_CMD = 0x100;
    
    // Waits receive 1 byte of data from sensor
    while(!(IC_STATUS & (1 << 3)));

    // Returns 1 byte of data
    return (uint8_t)(IC_DATA_CMD & 0xFF);
}