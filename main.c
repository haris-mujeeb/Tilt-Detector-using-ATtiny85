/*
 * ATtiny85_Blink_LED.c
 *
 * Created: 03.06.2025 12:17:40
 * Author : harri
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "mpu6050.h"
#include "i2c_primary.h"
#include "usi_uart_tx.h"

#define DEG_PER_RAD 57.2958

// State variables for LED blinking
volatile uint32_t lastToggleTime = 0;
volatile uint32_t blinkInterval = 0;
volatile uint8_t ledState = 0; // 0 = off, 1 = on

// Blink interval parameters
#define MAX_INTERVAL 1000 // 1000 ms at 0 degrees
#define MIN_INTERVAL 100  // 100 ms at max angle
#define MAX_ANGLE 90.0    // Maximum angle for min interval

void timer_init() {
	// Set up Timer0 for CTC mode
	TCCR0A = (1 << WGM01); // CTC mode
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
	OCR0A = (F_CPU / 64 / 1000) - 1; // Set compare value for 1 ms
	TIMSK |= (1 << OCIE0A); // Enable Timer0 compare interrupt
}


int main(void)
{
    DDRB |= (1 << PB4); // set PB4 as output pin
    
	// Initialize I2C and MPU6050
	i2c_init();       // Your bit-banged I2C init
	MPU6050_init();
    usi_uart_tx_init();
	
	int16_t ax, ay, az;
    float new_angle;
	char buffer[16];
	
	float alpha = 0.1; // Smoothing factor
	float filtered_angle = 0.0; // Initialize filtered angle
	
	// Initialize timer
	timer_init();
	sei(); // Enable global interrupts
	
	while (1) {
		MPU6050_read_accel(&ax, &ay, &az);
		
		// Convert raw acceleration to tilt angle in degrees (pitch-like estimation)
		new_angle = atan2((float)ax, (float)az) * DEG_PER_RAD;
		
		filtered_angle = alpha * new_angle + (1 - alpha) * filtered_angle;
		
		dtostrf(filtered_angle, 6, 2, buffer);
		
		// Transmit string over UART
		for (char *p = buffer; *p != '\0'; p++) {
			usi_uart_transmit(*p);
		}
		usi_uart_transmit('\r');
		usi_uart_transmit('\n');
		
		//// blink fast if tilted more than 30 degrees, else slow
		//if (fabs(angle) > 10.0) {
			//portb |= (1 << pb4); // led on
			//_delay_ms(1000);      // fast blink
			//portb &= ~(1 << pb4); // led off
			//_delay_ms(1000);
		//} else {
			//portb |= (1 << pb4); // led on
			//_delay_ms(500);      // slow blink
			//portb &= ~(1 << pb4); // led off
			//_delay_ms(500);
		//}

        // Calculate blink interval based on angle
        if (fabs(filtered_angle) <= MAX_ANGLE) {
	        blinkInterval = MAX_INTERVAL - (fabs(filtered_angle) / MAX_ANGLE) * (MAX_INTERVAL - MIN_INTERVAL);
	    } else {
	        blinkInterval = MIN_INTERVAL; // Cap at minimum interval if angle exceeds max
        }
		
		_delay_ms(30);
    }
}


// Timer interrupt service routine
ISR(TIMER0_COMPA_vect) {
	static uint32_t toggleTime = 0;

	toggleTime++; // Increment the timer count

	// Check if it's time to toggle the LED
	if (toggleTime >= blinkInterval / 2) {
		toggleTime = 0; // Reset the timer count
		if (ledState) {
			PORTB &= ~(1 << PB4); // Turn LED off
			ledState = 0;
			} else {
			PORTB |= (1 << PB4); // Turn LED on
			ledState = 1;
		}
	}
}