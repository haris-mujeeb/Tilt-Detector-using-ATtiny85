// I2C.hpp

#ifndef I2C_HPP
#define I2C_HPP

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define CLK_PER                 (F_CPU / 2)

#define I2C_SCL_FREQ            100000
#define I2C_SLAVE_ADDRESS       0x4D

#define I2C_DIRECTION_WRITE     0
#define I2C_DIRECTION_READ      1

#define TWI0_BAUD(F_SCL, T_RISE) \
((((((float)CLK_PER / (float)F_SCL)) - 10 - ((float)CLK_PER * T_RISE))) / 2)

#define I2C_ACK_RECEIVED        (!(TWI_RXACK_bm & TWI0.MSTATUS))
#define I2C_DATA_AVAILABLE      (TWI_RIF_bm & TWI0.MSTATUS)

void I2C_init(void);
void I2C_writeByte(uint8_t data);
uint8_t I2C_readByte(uint8_t addr, uint8_t reg);

#endif // I2C_HPP
