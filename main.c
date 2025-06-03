/*
 * ATtiny85_Blink_LED.c
 *
 * Created: 03.06.2025 12:17:40
 * Author : Muhammad Haris Mujeeb
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "mpu6050.h"
#include "i2c_primary.h"
#include "usi_uart_tx.h"

// === Constants ===
#define DEG_PER_RAD     57.2958
#define MAX_INTERVAL    500    // ms at 0°
#define MIN_INTERVAL    10     // ms at MAX_ANGLE
#define MAX_ANGLE       25.0    // Angle for minimum blink rate
#define LED_PIN         PB4

// === State variables ===
volatile uint32_t blinkInterval = MAX_INTERVAL;
volatile uint8_t ledState = 0;

// === Function Prototypes ===
void timer_init(void);
void send_angle(float angle);

// === Timer Setup ===
void timer_init(void) {
    TCCR0A = (1 << WGM01);                 // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00);    // Prescaler 64
    OCR0A = (F_CPU / 64 / 1000) - 1;       // 1 ms tick
    TIMSK |= (1 << OCIE0A);                // Enable compare interrupt
}

// === UART Send Float as String ===
void send_angle(float angle) {
    char buffer[16];
    dtostrf(angle, 6, 2, buffer);
    for (char *p = buffer; *p; p++) {
        usi_uart_transmit(*p);
    }
    usi_uart_transmit('\r');
    usi_uart_transmit('\n');
}

// === Main ===
int main(void) {
    // Setup LED pin
    DDRB |= (1 << LED_PIN);

    // Initialize peripherals
    i2c_init();
    MPU6050_init();
    usi_uart_tx_init();
    timer_init();
    sei();

    int16_t ax, ay, az;
    float angle_raw = 0, angle_filtered = 0;
    const float alpha = 0.1f;

    while (1) {
        // Read accelerometer
        MPU6050_read_accel(&ax, &ay, &az);

        // Compute tilt angle
        angle_raw = atan2((float)ax, (float)az) * DEG_PER_RAD;
        angle_filtered = alpha * angle_raw + (1 - alpha) * angle_filtered;

        // Transmit over UART
        send_angle(angle_filtered);

        // Adjust blink interval based on angle
        float abs_angle = fabs(angle_filtered);
        blinkInterval = (abs_angle <= MAX_ANGLE)
            ? MAX_INTERVAL - (abs_angle / MAX_ANGLE) * (MAX_INTERVAL - MIN_INTERVAL)
            : MIN_INTERVAL;

        _delay_ms(30); // Small delay to reduce I2C/UART load
    }
}

// === Timer Interrupt ===
ISR(TIMER0_COMPA_vect) {
    static uint32_t millisCounter = 0;

    if (++millisCounter >= blinkInterval / 2) {
        millisCounter = 0;

        // Toggle LED
        if (ledState) {
            PORTB &= ~(1 << LED_PIN);  // OFF
            ledState = 0;
        } else {
            PORTB |= (1 << LED_PIN);   // ON
            ledState = 1;
        }
    }
}
