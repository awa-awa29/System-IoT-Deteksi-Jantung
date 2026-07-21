# 🩺 Intelligent Wearable Health Monitoring System
### AI-Based ECG, Fall Detection, and Body Temperature Monitoring using ESP32

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Display](https://img.shields.io/badge/Display-OLED_SSD1306-green)
![Sensor](https://img.shields.io/badge/Sensor-MPU6050-orange)
![Sensor](https://img.shields.io/badge/Sensor-DS18B20-red)
![AI](https://img.shields.io/badge/AI-ECG_Classification-purple)
![Dataset](https://img.shields.io/badge/Dataset-PTB--XL-yellow)
![Simulator](https://img.shields.io/badge/Simulator-Wokwi-lightgrey)

---

## 📋 Daftar Isi

- [Latar Belakang](#-latar-belakang)
- [Tujuan](#-tujuan)
- [Fitur Utama](#-fitur-utama)
- [Arsitektur Sistem](#-arsitektur-sistem)
- [Komponen yang Digunakan](#-komponen-yang-digunakan)
- [Pin ESP32](#-pin-esp32)
- [Struktur Folder](#-struktur-folder)
- [Penjelasan File](#-penjelasan-file)
- [Diagram Alur Sistem](#-diagram-alur-sistem)
- [Algoritma ECG](#-algoritma-ecg)
- [Algoritma Fall Detection](#-algoritma-fall-detection)
- [Monitoring Suhu Tubuh](#-monitoring-suhu-tubuh)
- [Tampilan OLED](#-tampilan-oled)
- [Cara Menjalankan Simulasi](#-cara-menjalankan-simulasi)
- [Hasil Pengujian](#-hasil-pengujian)
- [Future Enhancement](#-future-enhancement)
- [Kesimpulan](#-kesimpulan)

---

## 🏥 Latar Belakang

Pemantauan kondisi kesehatan secara real-time menjadi kebutuhan penting, terutama untuk pasien lanjut usia, penderita penyakit jantung, maupun individu yang memerlukan pengawasan kesehatan secara berkelanjutan.

Perkembangan teknologi **Internet of Things (IoT)** dan **Artificial Intelligence (AI)** memungkinkan pengembangan perangkat wearable yang mampu melakukan pemantauan kesehatan secara otomatis dan portabel.

Proyek ini mengintegrasikan:

- 📈 Monitoring sinyal ECG
- 🤖 Klasifikasi ECG menggunakan AI
- 🌡 Monitoring suhu tubuh
- 🚶 Deteksi jatuh (Fall Detection)
- 📺 Tampilan real-time pada OLED

Sistem menggunakan **ESP32** sebagai mikrokontroler utama dengan data ECG yang berasal dari dataset **PTB-XL**.

---

## 🎯 Tujuan

- Menampilkan sinyal ECG secara real-time
- Mengklasifikasikan ECG menjadi Normal dan Abnormal
- Mendeteksi kejadian jatuh menggunakan MPU6050
- Memantau suhu tubuh menggunakan DS18B20
- Menampilkan informasi kesehatan pada OLED
- Memberikan peringatan menggunakan LED dan buzzer

---

## ✨ Fitur Utama

| Fitur | Deskripsi |
|---------|------------|
| 📈 ECG Playback | Menampilkan data ECG dari dataset PTB-XL |
| 🤖 AI Classification | Klasifikasi Normal / Abnormal |
| 📺 OLED Display | Menampilkan status sistem |
| 🌡 Temperature Monitoring | Monitoring suhu tubuh |
| 🚶 Fall Detection | Deteksi jatuh menggunakan MPU6050 |
| 🟢🔴 LED Indicator | Indikator status |
| 🔔 Buzzer Alarm | Alarm kondisi abnormal |
| ⚡ Real-Time System | Pembacaan data non-blocking |

---

## 🏗 Arsitektur Sistem

```text
                PTB-XL Dataset
                       │
                       ▼
               ECG Playback
                       │
                       ▼
               AI Classification
                       │
        ┌──────────────┴──────────────┐
        │                             │
        ▼                             ▼
   ECG Normal                  ECG Abnormal
        │                             │
        ▼                             ▼
 Green LED                     Red LED + Buzzer

                       │
                       ▼

        MPU6050 ───► Fall Detection

                       │
                       ▼

        DS18B20 ───► Body Temperature

                       │
                       ▼

               OLED Display
```

---

## 🔧 Komponen yang Digunakan

| Komponen | Fungsi |
|-----------|---------|
| ESP32 DevKit | Mikrokontroler utama |
| OLED SSD1306 | Display |
| MPU6050 | Accelerometer dan Gyroscope |
| DS18B20 | Sensor suhu |
| LED Hijau | Status normal |
| LED Merah | Status abnormal |
| Buzzer | Alarm |

---

## 🔌 Pin ESP32

| Komponen | GPIO |
|-----------|-------|
| OLED SDA | 21 |
| OLED SCL | 22 |
| DS18B20 | 4 |
| LED Hijau | 18 |
| LED Merah | 19 |
| Buzzer | 23 |

---

## 📁 Struktur Folder

```text
Project/
│
├── sketch.ino
├── ecg_data.h
├── diagram.json
├── libraries.txt
├── README.md
```

---

## 📄 Penjelasan File

### `sketch.ino`

Program utama yang berisi:

- Inisialisasi OLED
- Inisialisasi MPU6050
- Inisialisasi DS18B20
- Playback ECG
- AI Classification
- Fall Detection
- Kontrol LED
- Kontrol buzzer
- Tampilan OLED

---

### `ecg_data.h`

Berisi dataset ECG hasil ekstraksi dari PTB-XL.

Contoh:

```cpp
const float ecg_normal[1000];
const float ecg_abnormal[1000];
```

---

### `diagram.json`

Konfigurasi wiring Wokwi.

---

## 🔄 Diagram Alur Sistem

```text
Start
   │
   ▼
Inisialisasi Sistem
   │
   ▼
Load ECG Dataset
   │
   ▼
Baca ECG
   │
   ▼
AI Classification
   │
   ▼
Baca MPU6050
   │
   ▼
Deteksi Jatuh
   │
   ▼
Baca DS18B20
   │
   ▼
Update OLED
   │
   ▼
LED & Buzzer
   │
   ▼
Repeat
```

---

## 🤖 Algoritma ECG

Dataset ECG berasal dari:

- PTB-XL
- Lead II
- 1000 sampel
- Frekuensi sampling 100 Hz

Klasifikasi AI:

```text
ECG
   │
   ▼
Preprocessing
   │
   ▼
Model AI
   │
   ▼
Normal / Abnormal
```

---

## 🚶 Algoritma Fall Detection

Metode deteksi:

```text
Acceleration Magnitude

↓

Magnitude < 2 m/s²

↓

Impact > 18 m/s²

↓

Orientation Change > 60°

↓

Fall Detected
```

Rumus percepatan:

```text
Magnitude = √(ax² + ay² + az²)
```

---

## 🌡 Monitoring Suhu Tubuh

Sensor:

- DS18B20
- OneWire Protocol
- DallasTemperature Library

Suhu ditampilkan pada OLED:

```text
Temp : 36.7°C
```

---

## 📺 Tampilan OLED

```text
+------------------------+
| ECG : NORMAL           |
| Temp: 36.7 C           |
| Fall: NO               |
|                        |
| ECG Graph              |
+------------------------+
```

---

## 🚀 Cara Menjalankan Simulasi

1. Buka Wokwi
2. Pilih ESP32
3. Upload:
   - sketch.ino
   - ecg_data.h
   - diagram.json
4. Install library:
   - Adafruit SSD1306
   - Adafruit GFX
   - Adafruit MPU6050
   - DallasTemperature
   - OneWire
5. Klik Start Simulation

---

## 📊 Hasil Pengujian

✅ ECG tampil realtime

✅ AI Classification berjalan

✅ Fall Detection aktif

✅ Suhu tubuh tampil

✅ OLED berjalan normal

✅ LED indikator aktif

✅ Buzzer alarm aktif

---

## 🔮 Future Enhancement

- Integrasi MQTT
- Dashboard Node-RED
- Cloud Database
- Mobile Application
- Sensor ECG real

---

## 📌 Kesimpulan

Sistem berhasil mengintegrasikan AI dan IoT untuk monitoring kesehatan berbasis wearable menggunakan ESP32. Sistem mampu menampilkan ECG, mendeteksi kondisi abnormal, memantau suhu tubuh, mendeteksi jatuh, serta memberikan notifikasi melalui OLED, LED, dan buzzer secara real-time.

---

<div align="center">

**AI-Based Wearable Health Monitoring System**

ESP32 • PTB-XL • MPU6050 • DS18B20 • OLED • AI

</div>
