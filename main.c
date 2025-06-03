/*
 * ATtiny85_Blink_LED.c
 *
 * Created: 03.06.2025 12:17:40
 * Author : harri
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "mpu6050.h"
#include "i2c_primary.h"
#include "usi_uart_tx.h"

#define DEG_PER_RAD 57.2958

int main(void)
{
    DDRB |= (1 << PB4); // set PB4 as output pin
    
	// Initialize I2C and MPU6050
	i2c_init();       // Your bit-banged I2C init
	MPU6050_init();
    usi_uart_tx_init();
	
	int16_t ax, ay, az;
    float angle;
	char buffer[16];
		
	while (1) {
		MPU6050_read_accel(&ax, &ay, &az);
		
		// Convert raw acceleration to tilt angle in degrees (pitch-like estimation)
		angle = atan2((float)ax, (float)az) * DEG_PER_RAD;
		dtostrf(angle, 6, 2, buffer);
		
		// Transmit string over UART
		for (char *p = buffer; *p != '\0'; p++) {
			usi_uart_transmit(*p);
		}
		usi_uart_transmit('\r');
		usi_uart_transmit('\n');
		
		// Blink fast if tilted more than 30 degrees, else slow
		if (fabs(angle) > 10.0) {
			PORTB |= (1 << PB4); // LED on
			_delay_ms(1000);      // Fast blink
			PORTB &= ~(1 << PB4); // LED off
			_delay_ms(1000);
		} else {
			PORTB |= (1 << PB4); // LED on
			_delay_ms(500);      // Slow blink
			PORTB &= ~(1 << PB4); // LED off
			_delay_ms(500);
		}
    }
}