#ifndef USI_UART_RX_H
#define USI_UART_RX_H

#include <stdint.h>
#include <avr/io.h>
#include <stdbool.h>

// === Configuration ===
#define USI_UART_BAUD        9600
#define F_CPU                8000000UL
#define UART_BIT_DELAY       (F_CPU / USI_UART_BAUD)

// Pin configuration (change if needed)
#define UART_RX_DDR          DDRB
#define UART_RX_PORT         PORTB
#define UART_RX_PIN          PINB
#define UART_RX_BIT          PB0

// === Public API ===
void usi_uart_init(void);
bool usi_uart_data_available(void);
uint8_t usi_uart_receive(void);

#endif
