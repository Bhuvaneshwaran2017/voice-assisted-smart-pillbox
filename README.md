# Voice-Assisted Smart Pillbox

ESP32-based medicine reminder prototype using DS3231 RTC, DFPlayer Mini, speaker, LEDs, buzzer, and compartment buttons.

## Features
- 08:00 morning reminder
- 13:00 afternoon reminder
- 18:30 night reminder
- Voice announcement through DFPlayer Mini
- Buzzer starts after voice finishes
- Correct-compartment button acknowledgement
- DS3231 RTC timekeeping
- Prototype ESP32 Wi-Fi web interface

> This prototype indicates that a compartment was opened; it does not verify that medicine was consumed.

## Hardware
- ESP32 WROOM DevKit V1
- DS3231 RTC + AT24C32
- DFPlayer Mini
- Speaker
- 5 V active buzzer
- Green, yellow, red LEDs
- 220 ohm resistors
- 3 tactile push buttons
- microSD/TF card
- Breadboards and jumper wires

## GPIO map
| Function | GPIO |
|---|---:|
| DS3231 SDA | 21 |
| DS3231 SCL | 22 |
| DFPlayer RX | 17 / TX2 |
| DFPlayer TX | 16 / RX2 |
| Buzzer | 25 |
| Green LED | 26 |
| Yellow LED | 27 |
| Red LED | 32 |
| Morning button | 13 |
| Afternoon button | 14 |
| Night button | 18 |

## Software
- Arduino IDE 2.x
- Espressif ESP32 board package
- Adafruit RTClib
- DFRobotDFPlayerMini

## TF card audio
Create an `MP3` folder and add `0001.mp3`, `0002.mp3`, and `0003.mp3` for the three voice reminders.

## Wi-Fi prototype
SSID: `SMART-PILLBOX`

Password: `pillbox123`

Typical address: `192.168.4.1`

These are demo credentials only.

## Structure
- `firmware/` — Arduino firmware
- `docs/` — hardware, web interface, and future roadmap notes
- `audio/` — DFPlayer audio instructions
