#ifndef MPU6050_H
#define MPU6050_H

#include <avr/io.h>

#define MPU6050_ADDR       0x3B
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_GYRO_XOUT_H  0x43

void MPU6050_init(void);
int16_t MPU6050_read_word(uint8_t reg);
void MPU6050_read_accel(int16_t* ax, int16_t* ay, int16_t* az);
void MPU6050_read_gyro(int16_t* gx, int16_t* gy, int16_t* gz);

#endif
