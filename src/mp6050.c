#include "MPU6050.h"
#include "i2c_primary.h"

void MPU6050_init(void) {
	i2c_init();
	i2c_start();
	i2c_write_byte((MPU6050_ADDR << 1) | 0); // write
	i2c_write_byte(MPU6050_PWR_MGMT_1);
	i2c_write_byte(0x00); // Wake up
	i2c_stop();
}

int16_t MPU6050_read_word(uint8_t reg) {
	int16_t value;
	uint8_t high, low;

	i2c_start();
	i2c_write_byte((MPU6050_ADDR << 1) | 0); // write
	i2c_write_byte(reg);
	i2c_start();
	i2c_write_byte((MPU6050_ADDR << 1) | 1); // read

	high = i2c_read_byte(1);
	low = i2c_read_byte(0);
	i2c_stop();

	value = (high << 8) | low;
	return value;
}

void MPU6050_read_accel(int16_t* ax, int16_t* ay, int16_t* az) {
	*ax = MPU6050_read_word(MPU6050_ACCEL_XOUT_H);
	*ay = MPU6050_read_word(MPU6050_ACCEL_XOUT_H + 2);
	*az = MPU6050_read_word(MPU6050_ACCEL_XOUT_H + 4);
}

void MPU6050_read_gyro(int16_t* gx, int16_t* gy, int16_t* gz) {
	*gx = MPU6050_read_word(MPU6050_GYRO_XOUT_H);
	*gy = MPU6050_read_word(MPU6050_GYRO_XOUT_H + 2);
	*gz = MPU6050_read_word(MPU6050_GYRO_XOUT_H + 4);
}
