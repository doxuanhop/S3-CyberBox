# ESP32-S3 CyberBox – Hardware Documentation

> **Project**: ESP32-S3 CyberBox  \
> **MCU**: ESP32-S3-WROOM-1 (N16R8)  \
> **Schematic**: EasyEDA – Sheet 1/1  \
> **Revision**: v1.0 (2025-12-09)

---

## 1. Project Overview

CyberBox là một thiết bị nhúng dựa trên **ESP32-S3**, tích hợp:

- Màn hình TFT ST7789 (SPI)
- Thẻ nhớ microSD (SPI)
- Cảm biến môi trường BME280 (I²C)
- USB-C (native USB + USB-UART CH340)
- Pin Li-ion + mạch sạc BQ24074
- Joystick, buttons, buzzer

Tài liệu này mô tả **pin mapping**, **kiểm tra xung đột GPIO** và **các lưu ý phần cứng** để phục vụ phát triển firmware và public trên GitHub.

---

## 2. ESP32-S3 GPIO Pin Mapping

### 2.1 Power & System

| GPIO | Function | Notes |
|------|---------|-------|
| 3V3 | Power | Main 3.3V rail |
| GND | Ground | Common ground |
| EN  | Reset | RC reset circuit |
| GPIO1 | ADC | Battery voltage sense (VBAT) |
| GPIO0 | BOOT | Boot button (must be HIGH at boot) |

---

### 2.2 USB Interfaces

#### Native USB (ESP32-S3)

| GPIO | USB | Notes |
|------|-----|------|
| GPIO19 | D- | Dedicated USB pin |
| GPIO20 | D+ | Dedicated USB pin |

#### USB-UART (CH340X)

| GPIO | UART | Notes |
|------|------|------|
| GPIO43 | RXD | UART RX |
| GPIO44 | TXD | UART TX |

---

### 2.3 TFT Display – ST7789 (SPI)

| GPIO | TFT Pin | Function |
|------|--------|----------|
| GPIO47 | SCL | SPI Clock |
| GPIO14 | SDA | SPI MOSI |
| GPIO48 | CS | Chip Select |
| GPIO21 | DC | Data / Command |
| GPIO38 | RES | Reset |
| GPIO39 | BL | Backlight (via transistor) |

> TFT uses a **dedicated SPI bus**, separate from SD card SPI.

---

### 2.4 microSD Card (SPI)

| GPIO | SD Pin | Notes |
|------|-------|------|
| GPIO11 | CLK | SPI Clock (bootstrap pin) |
| GPIO13 | CMD | MOSI (bootstrap pin) |
| GPIO12 | DAT0 | MISO (bootstrap pin) |
| GPIO10 | DAT3 / CS | Chip Select (bootstrap pin) |
| GPIO9 | DET | Card detect (input-only) |

⚠️ **Bootstrap GPIOs** must not be hard-pulled HIGH or LOW at boot.

---

### 2.5 I²C – BME280 Sensor

| GPIO | I²C | Notes |
|------|-----|------|
| GPIO6 | SCL | I²C Clock |
| GPIO7 | SDA | I²C Data |

- Pull-up resistors: **3.3 kΩ → 3V3**
- Safe on ESP32-S3 (Flash is internal to module)

---

### 2.6 Buttons & Joystick

#### Buttons

| GPIO | Input | Notes |
|------|------|------|
| GPIO12 | Button | Bootstrap pin |
| GPIO13 | Button | Bootstrap pin |
| GPIO36 | KEY1 | Input-only |
| GPIO37 | KEY2 | Input-only |
| GPIO0 | BOOT | Boot / user button |

#### Joystick

| GPIO | Direction |
|------|-----------|
| GPIO5 | COM |
| GPIO4 | A |
| GPIO6 | B |
| GPIO7 | C |
| GPIO15 | D |

---

### 2.7 Buzzer

| GPIO | Function | Notes |
|------|---------|------|
| GPIO41 | Buzzer | PWM via transistor (safe) |

---

## 3. GPIO Conflict Analysis

### 3.1 No Critical Conflicts

- TFT SPI and SD SPI use **separate buses**
- USB native and USB-UART are isolated
- ADC pin is not reused
- I²C does not overlap SPI

✅ **No blocking GPIO conflicts detected**

---

### 3.2 Sensitive / Special GPIOs

| GPIO | Reason | Recommendation |
|------|-------|----------------|
| GPIO0 | Boot strap | Use INPUT_PULLUP |
| GPIO10–13 | Boot strap | Avoid fixed pull-up/down |
| GPIO36/37 | Input-only | Do not configure OUTPUT |
| GPIO19/20 | USB | Do not reuse |
| GPIO1 | ADC / UART | Avoid Serial TX |

---

## 4. Firmware Notes (Arduino / ESP-IDF)

```cpp
// SD card SPI
SPI.begin(11, 12, 13, 10);

// I2C
Wire.begin(7, 6);

// Boot button
pinMode(0, INPUT_PULLUP);
```

---

## 5. Hardware Design Verdict

| Item | Status |
|------|--------|
| Boot stability | ✅ OK |
| GPIO usage | ✅ Safe |
| Expandable | ✅ Yes |
| GitHub ready | ✅ Yes |

---

## 6. Repository Suggestions

Recommended GitHub structure:

```
CyberBox/
├─ hardware/
│  ├─ schematic.pdf
│  └─ pcb/
├─ firmware/
│  ├─ pinmap.h
│  └─ main.ino
├─ docs/
│  └─ hardware.md   <-- this file
└─ README.md
```

---

## 7. License

This hardware design is provided under:

- **CERN-OHL-S** or **MIT License** (recommended)

---

✅ Document ready for GitHub publishing.


