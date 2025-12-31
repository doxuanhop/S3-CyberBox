# ESP32-S3 CyberBox – Hardware Documentation

> **Project**: ESP32-S3 CyberBox  
> **MCU**: ESP32-S3-WROOM-1 (N16R8)  
> **Schematic**: EasyEDA – Sheet 1/1  
> **Revision**: v1.0 (2025-12-09)

---

## 1. Project Overview

CyberBox là một thiết bị nhúng dựa trên **ESP32-S3**, tích hợp:

* Màn hình TFT ST7789 (SPI)
* Thẻ nhớ microSD (SPI)
* Cảm biến môi trường BME280 (I²C)
* USB-C (native USB + USB-UART CH340)
* Pin Li-ion + mạch sạc BQ24074
* Joystick, buttons, buzzer

Tài liệu này mô tả **pin mapping**, **kiểm tra xung đột GPIO** và **các lưu ý phần cứng** để phục vụ phát triển firmware và public trên GitHub.

---

## 2. ESP32-S3 GPIO Pin Mapping

### 2.1 TFT Display – ST7789 (SPI)

| Function   | GPIO   | Notes             |
| ---------- | ------ | ----------------- |
| BL / LED   | GPIO39 | Backlight control |
| D/C        | GPIO47 | Data / Command    |
| CS         | GPIO14 | Chip Select       |
| SCL / SCK  | GPIO48 | SPI Clock         |
| SDA / MOSI | GPIO12 | SPI MOSI          |
| RESET      | GPIO3  | Display reset     |

---

### 2.2 Buttons

```c
#define BTN_UP     40
#define BTN_DOWN   5
#define BTN_LEFT   4
#define BTN_RIGHT  36
#define BTN_A      37
#define BTN_B      45
```

| Button | GPIO   | Notes      |
| ------ | ------ | ---------- |
| UP     | GPIO40 | Input      |
| DOWN   | GPIO5  | Input      |
| LEFT   | GPIO4  | Input      |
| RIGHT  | GPIO36 | Input-only |
| A      | GPIO37 | Input-only |
| B      | GPIO45 | Input      |

---

### 2.3 microSD Card (SPI)

| ESP32-S3 GPIO | microSD Pin | Notes       |
| ------------- | ----------- | ----------- |
| GPIO10        | CD/DAT3     | Chip Select |
| GPIO11        | CMD         | MOSI        |
| GPIO13        | CLK         | SPI Clock   |
| GPIO9         | DAT0        | MISO        |
| GPIO38        | DET         | Card Detect |

---

### 2.4 Environment Sensor – BME280 (I²C)

| ESP32-S3 GPIO | BME280 | Function  |
| ------------- | ------ | --------- |
| GPIO06        | SCL    | I²C Clock |
| GPIO07        | SDA    | I²C Data  |

---

### 2.5 Buzzer

| ESP32-S3 GPIO | Component   | Function   |
| ------------- | ----------- | ---------- |
| GPIO41        | Q2 → Buzzer | PWM / Beep |

---

## 3. GPIO Conflict Analysis

### 3.1 No Critical Conflicts

* TFT SPI and SD SPI use **separate buses**
* USB native and USB-UART are isolated
* ADC pin is not reused
* I²C does not overlap SPI

✅ **No blocking GPIO conflicts detected**

---

### 3.2 Sensitive / Special GPIOs

| GPIO      | Reason     | Recommendation           |
| --------- | ---------- | ------------------------ |
| GPIO0     | Boot strap | Use INPUT_PULLUP         |
| GPIO10–13 | Boot strap | Avoid fixed pull-up/down |
| GPIO36/37 | Input-only | Do not configure OUTPUT  |
| GPIO19/20 | USB        | Do not reuse             |
| GPIO1     | ADC / UART | Avoid Serial TX          |

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

 publishing.
