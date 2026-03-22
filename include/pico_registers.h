#ifndef PICO_REGISTERS_H
#define PICO_REGISTER_H

// ---------- RP2040 MEMORY-MAPPED REGISTERS ----------
// Reset block. Used to bring the I2C0 peripheral out of reset during setup.
#define RESETS_BASE 0x4000c000

// GPIO control block. Used to multiplex pins to the I2C SDA/SCL functions.
#define IO_BANK0_BASE 0x40014000 

// Clocks block. Used to enable the peripheral clock (clk_peri) for the I2C hardware.
#define CLOCKS_BASE 0x40008000   

// Main I2C0 control block. Contains data FIFOs and communication status registers.
#define I2C0_BASE 0x40044000     

#endif