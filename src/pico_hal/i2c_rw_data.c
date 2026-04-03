#include <stdint.h>
#include <string.h>

#include "reg_utils.h"
#include "pico_driver/i2c_rw_data.h"
#include "pico_driver/registers.h"
#include "pico_driver/error.h"
#include "pico/time.h"

// --- Reset and Peripheral Control Bits ---
// Resets or enables Digital IO (GPIOs)
#define IO_BANK0_BIT (1 << 5)

// Resets or enables physical pin controls (Pull-ups/downs)
#define PADS_BANK0_BIT (1 << 8)

// Resets or enables the I2C0 hardware block
#define I2C1_BIT (1 << 4)

// --- IC_CON Register Configuration (I2C Control) ---
// Sets I2C speed to 400kHz (bits 2:1)
#define FAST_MODE (2 << 1)

// Sets RP2040 as the Master of the bus (bit 0)
#define MASTER_MODE (1 << 0)

// Disables Slave mode to prevent bus conflicts (bit 6)
#define SLAVE_MODE_DISABLE (1 << 6)

// --- IC_STATUS ---
// Receive FIFO Non Empty bit, If receive one byte, will alert.
#define RFNE_BIT (1 << 3)
#define TFNF_BIT (1 << 1)
#define MST_ACT (1 << 5)

// --- System and Hardware Control Registers ---
// Register to take peripherals out of reset
#define RESETSREG_BIT_SET ((volatile uint32_t *)(RESETS_BASE))

// Configures GPIO14 for I2C SDA function
#define GPIO14_CTRL ((volatile uint32_t *)(IO_BANK0_BASE + 0x074))

// Configures GPIO15 for I2C SCL function
#define GPIO15_CTRL ((volatile uint32_t *)(IO_BANK0_BASE + 0x07C))

// Enables the clock signal for peripherals
#define CLK_PERI_CTRL ((volatile uint32_t *)(CLOCKS_BASE + 0x48))

// --- I2C1 Specific Registers ---
// Main switch: Enable (1) or Disable (0) I2C
#define IC_ENABLE ((volatile uint32_t *)(I2C1_BASE + 0x6c))

// Clock cycles for SCL HIGH level in Fast Mode
#define IC_FS_SCL_HCNT ((volatile uint32_t *)(I2C1_BASE + 0x1c))

// Clock cycles for SCL LOW level in Fast Mode
#define IC_FS_SCL_LCNT ((volatile uint32_t *)(I2C1_BASE + 0x20))

// Main configuration register (Mode, Speed, etc.)
#define IC_CON ((volatile uint32_t *)(I2C1_BASE + 0x00))
#define IC_RESTART_EN_BIT (1 << 5)

#define IC_TX_ABRT_SOURCE ((volatile uint32_t *)(I2C1_BASE + 0x80))
#define IC_CLR_TX_ABRT ((volatile uint32_t *)(I2C1_BASE + 0x54))

// Slave Address: The address of the target sensor (0x68)
#define IC_TAR ((volatile uint32_t *)(I2C1_BASE + 0x04))

// Data buffer: Used to send commands and read received bytes
#define IC_DATA_CMD ((volatile uint32_t *)(I2C1_BASE + 0x10))

// Hardware status: Shows if the buffer is empty or full
#define IC_STATUS ((volatile uint32_t *)(I2C1_BASE + 0x70))

#define PAD_GPIO14 ((volatile uint32_t *)(PADS_BANK0_BASE + 0x3C))
#define PAD_GPIO15 ((volatile uint32_t *)(PADS_BANK0_BASE + 0x40))

#define PADS_PULL_UP (1 << 3)
#define PADS_SCHMITT_TRIGGER (1 << 1)

// --- CLOCK ---
// Enables the peripheral clock (clk_peri) in the CLK_PERI_CTRL register
#define ENABLE_CLOCK_PERI (1 << 11)

// --- IC_DATA_CMD ---
// Signals an I2C Read operation by setting bit 8 (CMD bit) in the data register
#define IC_READ_CMD 0x100
#define IC_STOP_BIT (1 << 9)
#define IC_RESTART_BIT (1 << 10)

// Timeout max cycles
#define I2C_TIMEOUT_CYCLES 100000

uint8_t err;

void rp2040_setup_hwr(void)
{
    bit_bank_t bit_bank = {0};

    // Enabling clk_peri
    *CLK_PERI_CTRL |= ENABLE_CLOCK_PERI;

    // Sets bits to 1 in a 32-bit bitset
    bit_bank.bit_arr |= (IO_BANK0_BIT | PADS_BANK0_BIT | I2C1_BIT);

    // Reset bits from reset register
    *RESETSREG_BIT_SET &= ~(bit_bank.bit_arr);

    // Waits until reset bits
    while(!is_reset_done(bit_bank));

    // Resets bitset from struct
    memset(&bit_bank, 0, sizeof(bit_bank_t));

    // Setting GPIO14 and GPIO15 to I2C
    *GPIO14_CTRL = 0x03; // I2C SDA
    *GPIO15_CTRL = 0x03; // I2C SCL

    *PAD_GPIO14 = PADS_SCHMITT_TRIGGER;
    *PAD_GPIO15 = PADS_SCHMITT_TRIGGER;

    // Disables I2C
    *IC_ENABLE = 0x0;

    // 120 + 192 = 312 clock cycles
    *IC_FS_SCL_HCNT = 120;
    *IC_FS_SCL_LCNT = 192;

    // I2C configuration
    *IC_CON = (FAST_MODE | MASTER_MODE | SLAVE_MODE_DISABLE | IC_RESTART_EN_BIT);

    // Sets slave address register to 0x68 (MPU-6050 default address)
    *IC_TAR = 0x68;

    // Enables I2C
    *IC_ENABLE = 0x1;
}

// Waits until receive RFNE status
static int8_t i2c_wait_rfne(void)
{
    uint32_t timeout = I2C_TIMEOUT_CYCLES;

    // Waits receive 1 byte of data from sensor
    while(!(*IC_STATUS & RFNE_BIT)){
        if(--timeout == 0)
        {
            return -1;
        }
    };

    return 0;
}

uint8_t rp2040_i2c_read_byte(uint8_t sensor_reg_addr)
{
    uint32_t timeout;

    // Flush IC_DATA
    while((*IC_STATUS & RFNE_BIT))
    {
        (void)*IC_DATA_CMD;    
    }

    timeout = I2C_TIMEOUT_CYCLES;

    // Waits until free FIFO
    while(!(*IC_STATUS & TFNF_BIT) && --timeout);
    if(timeout == 0) return 1;

    // Points to the sensor address  
    *IC_DATA_CMD = sensor_reg_addr;

    timeout = I2C_TIMEOUT_CYCLES;
    
    // Waits until free FIFO
    while(!(*IC_STATUS & TFNF_BIT) && --timeout);
    if(timeout == 0) return 2;

    // Sends read command
    *IC_DATA_CMD = IC_READ_CMD | IC_RESTART_BIT | IC_STOP_BIT;

    //Waits until receive RFNE status
    err = i2c_wait_rfne();
    if(err != 0)
    {
        pico_restart_i2c();
        return 0;
    }
   
    timeout = I2C_TIMEOUT_CYCLES;
   
    // Waits until receive Master activity bit status
    while((*IC_STATUS & MST_ACT) && --timeout);
    if(timeout == 0) return 3;

    // Returns 1 byte of data by setting the first 8 bits to 1 and the remnant to 0, working as a bit filter
    return (uint8_t)(*IC_DATA_CMD & 0xFF);
}

uint8_t rp2040_i2c_write_byte(uint8_t sensor_reg_addr, uint8_t data)
{
    uint32_t timeout;
    
    // Flush IC_DATA
    while((*IC_STATUS & RFNE_BIT))
    {
        (void)*IC_DATA_CMD;
    }

    timeout = I2C_TIMEOUT_CYCLES;
    
    // Waits until free FIFO
    while(!(*IC_STATUS & TFNF_BIT) && --timeout);
    if(timeout == 0) return 1;

    // Points to the sensor address  
    *IC_DATA_CMD = sensor_reg_addr;

    
    timeout = I2C_TIMEOUT_CYCLES;
    
    // Waits until free FIFO
    while(!(*IC_STATUS & TFNF_BIT) && --timeout);
    if(timeout == 0) return 2;

    // Sends data to write in address
    *IC_DATA_CMD = data | IC_STOP_BIT;

    timeout = I2C_TIMEOUT_CYCLES;

    // Waits until receive Master activity bit status
    while((*IC_STATUS & MST_ACT) && --timeout);
    if(timeout == 0) return 3;

    return 0;
}

uint8_t rp2040_sensor_recon(void)
{
    return rp2040_i2c_read_byte(0x75);
}
