#include "usi_uart_rx.h"
#include <util/delay_basic.h>

// Buffer for 1 received byte
static volatile uint8_t uart_rx_buffer;
static volatile bool uart_rx_ready = false;

// Delay function calibrated for baud timing
static void uart_bit_delay(void) {
	_delay_loop_2(UART_BIT_DELAY / 4); // 4 cycles per loop (approx.)
}

static void uart_half_bit_delay(void) {
	_delay_loop_2(UART_BIT_DELAY / 8);  // Half the bit delay
}

void usi_uart_init(void) {
	UART_RX_DDR &= ~(1 << UART_RX_BIT);   // Set RX pin as input
	UART_RX_PORT |= (1 << UART_RX_BIT);   // Enable pull-up
}

bool usi_uart_data_available(void) {
	return uart_rx_ready;
}

uint8_t usi_uart_receive(void) {
	// Wait for start bit (logic LOW)
	while (UART_RX_PIN & (1 << UART_RX_BIT));

	// Wait 1.5 bit times to sample at the middle of the first data bit
	uart_bit_delay();
	uart_half_bit_delay(); // half-bit delay (approx.)

	uint8_t data = 0;

	for (uint8_t i = 0; i < 8; i++) {
		uart_bit_delay();

		data >>= 1;
		if (UART_RX_PIN & (1 << UART_RX_BIT)) {
			data |= 0x80; // MSB first
		}
	}

	// Wait for stop bit
	uart_bit_delay();

	uart_rx_buffer = data;
	uart_rx_ready = true;

	return data;
}
