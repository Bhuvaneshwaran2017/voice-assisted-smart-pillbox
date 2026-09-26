# Hardware and Wiring

## DS3231 RTC
- VCC -> ESP32 3.3 V
- GND -> common GND
- SDA -> GPIO 21
- SCL -> GPIO 22
- SQW and 32K unused

## DFPlayer Mini
- VCC -> +5 V rail
- GND -> common GND
- RX -> GPIO 17 / TX2
- TX -> GPIO 16 / RX2
- SPK1 and SPK2 -> speaker

Keep the speaker across SPK1 and SPK2. Do not connect the speaker to ESP32 GND.

## LEDs
- Green -> GPIO 26 through 220 ohm resistor -> GND
- Yellow -> GPIO 27 through 220 ohm resistor -> GND
- Red -> GPIO 32 through 220 ohm resistor -> GND

## Buzzer
- Positive -> GPIO 25
- Negative -> common GND

The buzzer starts only after the voice announcement finishes.

## Compartment buttons
- Morning -> GPIO 13 and GND
- Afternoon -> GPIO 14 and GND
- Night -> GPIO 18 and GND

Buttons use INPUT_PULLUP.

## TF card
Create an MP3 folder containing 0001.mp3, 0002.mp3, and 0003.mp3.

A button press represents compartment opening; it does not prove medicine consumption.
