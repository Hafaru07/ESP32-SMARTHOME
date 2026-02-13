# 🏠 ESP32 Smart Home Project

Project Smart Home berbasis **ESP32** dengan fitur:

- 🌐 Web Control (WiFi)
- 👣 PIR Motion Sensor
- 🌙 Light Sensor (LDR - Analog)
- 💡 5 Output Lamp Control
- 🔄 Sistem stabil dengan hysteresis & anti-noise

---

## 🚀 Features

### 1️⃣ Web Server Control
- Kontrol **Lamp 1 (D13)** dan **Lamp 2 (D12)**
- Tampilan modern (Dark UI)
- Responsive untuk HP & Desktop
- Status ON/OFF realtime

### 2️⃣ PIR Motion System
- PIR Sensor di **D25**
- Lamp 3 (D14) → Standby Indicator
- Lamp 4 (D27) → Motion Trigger Indicator
- Sistem toggle:
  - Start → Lamp3 ON
  - Motion detected → Lamp4 ON
  - Motion detected lagi → kembali standby

### 3️⃣ Smart Light Sensor (LDR)
- Analog input di **D33**
- Lamp 5 (D26) menyala saat benar-benar gelap
- Menggunakan:
  - Averaging (anti noise)
  - Hysteresis (beda batas ON & OFF)
  - Delay konfirmasi 3 detik
- Tidak sensitif terhadap bayangan atau awan lewat

---

## 📌 Pin Configuration

| Component | ESP32 Pin |
|-----------|----------|
| LDR (AO) | D33 |
| PIR OUT | D25 |
| Lamp 1 | D13 |
| Lamp 2 | D12 |
| Lamp 3 (PIR Standby) | D14 |
| Lamp 4 (PIR Trigger) | D27 |
| Lamp 5 (LDR Auto) | D26 |

> ⚠️ Semua VCC & GND sudah diatur sesuai kebutuhan hardware.

---

## 🔧 Installation

1. Install **Arduino IDE**
2. Install Board ESP32:
3. Install board via: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Pilih board ESP32 kamu
5. Upload kode
6. Buka Serial Monitor (115200 baud)
7. Akses IP Address yang muncul di browser

---

## ⚙️ LDR Calibration

Jika terlalu sensitif, ubah bagian ini di kode:

```cpp
int darkLevel = 3300;
int brightLevel = 2600;
