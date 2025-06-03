#ifndef I2C_PRIMARY_H
#define I2C_PRIMARY_H

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

// I2C Pins
#define I2C_SDA_PIN         PB0
#define I2C_SCL_PIN         PB2

// Timing Delays (µs)
#define I2C_DELAY_LONG      5   // ~4.7 µs
#define I2C_DELAY_SHORT     4   // ~4.0 µs

// USISR Masks
#define USISR_8BIT_TRANSFER 0b11110000
#define USISR_1BIT_TRANSFER 0b11111110

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write_byte(uint8_t data);
uint8_t i2c_read_byte(uint8_t ack);
uint8_t i2c_transfer(uint8_t usisr_mask);

#endif