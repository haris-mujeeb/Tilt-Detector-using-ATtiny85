#include "i2c_primary.h"

void i2c_init(void) {
    // Set SDA and SCL as outputs
    I2C_DDR |= (1 << I2C_SDA_PIN) | (1 << I2C_SCL_PIN);

    // Set both lines high (idle)
    I2C_PORT |= (1 << I2C_SDA_PIN) | (1 << I2C_SCL_PIN);

    // Default USIDR to released
    USIDR = 0xFF;

    // Configure USI for 2-wire mode with external clock source
    USICR = (1 << USIWM1) | (1 << USICS1) | (1 << USICLK);

    // Clear flags and reset counter
    USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC);
}

void i2c_start(void) {
    // Generate START condition
    I2C_PORT |= (1 << I2C_SDA_PIN); // SDA high
    I2C_PORT |= (1 << I2C_SCL_PIN); // SCL high
    while (!(I2C_PORT & (1 << I2C_SCL_PIN)));

    I2C_PORT &= ~(1 << I2C_SDA_PIN); // SDA low
    _delay_us(I2C_DELAY_LONG);

    I2C_PORT &= ~(1 << I2C_SCL_PIN); // SCL low
    I2C_PORT |= (1 << I2C_SDA_PIN);  // SDA high (ready for data)
}

void i2c_stop(void) {
    // Generate STOP condition
    I2C_PORT &= ~(1 << I2C_SDA_PIN); // SDA low
    I2C_PORT |= (1 << I2C_SCL_PIN);  // SCL high
    while (!(I2C_PIN & (1 << I2C_SCL_PIN)));
    _delay_us(I2C_DELAY_LONG);
    I2C_PORT |= (1 << I2C_SDA_PIN);  // SDA high
    _delay_us(I2C_DELAY_SHORT);
}

uint8_t i2c_transfer(uint8_t usisr_mask) {
    I2C_PORT &= ~(1 << I2C_SCL_PIN); // Ensure SCL low
    USISR = usisr_mask;

    do {
        _delay_us(I2C_DELAY_LONG);
        USICR |= (1 << USITC); // Toggle SCL
        while (!(I2C_PIN & (1 << I2C_SCL_PIN)));
        _delay_us(I2C_DELAY_SHORT);
        USICR |= (1 << USITC);
    } while (!(USISR & (1 << USIOIF)));

    _delay_us(I2C_DELAY_LONG);

    uint8_t data = USIDR;
    USIDR = 0xFF; // Release SDA
    return data;
}

uint8_t i2c_write_byte(uint8_t data) {
    USIDR = data;
    i2c_transfer(USISR_8BIT_TRANSFER);

    I2C_DDR &= ~(1 << I2C_SDA_PIN); // SDA input for ACK
    uint8_t ack = i2c_transfer(USISR_1BIT_TRANSFER);
    I2C_DDR |= (1 << I2C_SDA_PIN);  // SDA output again

    return ack;
}

uint8_t i2c_read_byte(uint8_t ack) {
    I2C_DDR &= ~(1 << I2C_SDA_PIN); // Set SDA as input
    uint8_t data = i2c_transfer(USISR_8BIT_TRANSFER);
    I2C_DDR |= (1 << I2C_SDA_PIN);  // Set SDA as output

    USIDR = ack ? 0x00 : 0xFF;   // ACK = 0, NACK = 1
    i2c_transfer(USISR_1BIT_TRANSFER);

    return data;
}
