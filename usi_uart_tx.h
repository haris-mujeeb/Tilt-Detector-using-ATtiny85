#ifndef USI_UART_TX_H
#define USI_UART_TX_H

#include <stdint.h>
#include <avr/io.h>

// === Configuration ===
#define USI_UART_BAUD        9600
#define F_CPU                8000000UL
#define UART_BIT_CYCLES      (F_CPU / USI_UART_BAUD)

// === TX pin configuration ===
#define UART_TX_PORT         PORTB
#define UART_TX_DDR          DDRB
#define UART_TX_BIT          PB3

// === API ===
void usi_uart_tx_init(void);
void usi_uart_transmit(uint8_t data);

#endif
