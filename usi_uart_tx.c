#include "usi_uart_tx.h"
#include <util/delay_basic.h>

// Delay function for 1-bit duration
static void uart_tx_bit_delay(void) {
	_delay_loop_2(UART_BIT_CYCLES / 4);  // 4 cycles per loop
}

void usi_uart_tx_init(void) {
	UART_TX_DDR |= (1 << UART_TX_BIT);     // Set TX pin as output
	UART_TX_PORT |= (1 << UART_TX_BIT);    // Set high (idle)
}

void usi_uart_transmit(uint8_t data) {
	// Send start bit (low)
	UART_TX_PORT &= ~(1 << UART_TX_BIT);
	uart_tx_bit_delay();

	// Send 8 data bits (LSB first)
	for (uint8_t i = 0; i < 8; i++) {
		if (data & 0x01)
		UART_TX_PORT |= (1 << UART_TX_BIT);
		else
		UART_TX_PORT &= ~(1 << UART_TX_BIT);

		uart_tx_bit_delay();
		data >>= 1;
	}

	// Send stop bit (high)
	UART_TX_PORT |= (1 << UART_TX_BIT);
	uart_tx_bit_delay();
}
