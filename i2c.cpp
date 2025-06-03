// i2c.cpp

#include "i2c.h"

void I2C_init() {
	TinyWireM.begin(); // USI I2C init
}

void I2C_writeByte(uint8_t addr, uint8_t reg, uint8_t data) {
	TinyWireM.beginTransmission(addr);
	TinyWireM.send(reg);
	TinyWireM.send(data);
	TinyWireM.endTransmission();
}

uint8_t I2C_readByte(uint8_t addr, uint8_t reg) {
	TinyWireM.beginTransmission(addr);
	TinyWireM.send(reg);
	TinyWireM.endTransmission();
	TinyWireM.requestFrom(addr, (uint8_t)1);
	return TinyWireM.receive();
}

