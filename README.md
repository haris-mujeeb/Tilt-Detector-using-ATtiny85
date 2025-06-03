# 📡 ATtiny85 Tilt-Sensitive LED Blink + UART Output

This project runs on an **ATtiny85** microcontroller and uses an **MPU6050** accelerometer to measure tilt. Based on the tilt angle, it:

- ⏱️ Blinks an LED faster or slower  
- 📤 Sends the tilt angle over UART using a software-based TX-only serial implemented via USI

## 🔧 Features

- Tilt detection using `atan2(ax, az)`
- UART TX on **PB3** (software-emulated)
- LED blinking on **PB4**
- Raw angle data smoothed with a complementary filter
- Angle sent every 30 ms as a readable string via UART

## 🎥 Video Demo


https://github.com/user-attachments/assets/89a8bb53-574f-4a65-8edf-30fec9289218



## 🧰 Requirements

- **ATtiny85** (8 MHz internal clock)
- **MPU6050** accelerometer module (I2C)
- **UART-to-USB adapter** for serial logging
- **AVR toolchain** + Microchip Studio, or `avr-gcc` + `avrdude`

## 📌 Pin Configuration

| Pin | Function                |
|-----|-------------------------|
| PB0 | I2C SDA (to MPU6050)    |
| PB2 | I2C SCL (to MPU6050)    |
| PB3 | UART TX (to USB adapter)|
| PB4 | LED Blink Output        |

## 📥 Setup

### Clone this repo:
```bash
git clone https://github.com/yourusername/attiny85-tilt-uart-led.git
cd attiny85-tilt-uart-led
````

### Flash to your ATtiny85:

```bash
make flash
```

Then connect a serial adapter to **PB3** (TX) at **9600 baud** and open a serial monitor.

#### 📈 Example Output (Serial Monitor)

```
-3.42  
-4.01  
-5.27  
...
```

---

## 🛠️ Customize

* Change `UART_TX_BIT` in `usi_uart_tx.h` to use a different TX pin.
* Adjust blink speed range using `MAX_INTERVAL`, `MIN_INTERVAL`, and `MAX_ANGLE` in `main.c`.

---

## 📄 License
MIT License — use freely in personal or commercial projects. Attribution appreciated.

