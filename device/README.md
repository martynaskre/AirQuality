# AirQuality IoT Device

This IoT device is built on the **Raspberry Pi Pico W** using **pico-sdk** and **FreeRTOS**.

## Components

The following components are used in the device:

- Raspberry Pi Pico W
- [Qwiic / Stemma QT 5 Port Hub](https://www.adafruit.com/product/5625)
- [SGP30](https://www.adafruit.com/product/3709) VOC and eCO2 sensor
- [BME280](https://www.adafruit.com/product/2652) temperature, humidity and pressure sensor
- [LTR-559](https://shop.pimoroni.com/products/ltr-559-light-proximity-sensor-breakout) light & proximity sensor
- [Mini LED Arcade Button](https://www.adafruit.com/product/3429)
- Cables:
    - 2x [STEMMA QT / Qwiic JST SH 4-Pin Cable - 300mm long](https://www.adafruit.com/product/5384)
    - 2x [Qwiic Cable - Breadboard Jumper](https://www.sparkfun.com/qwiic-cable-breadboard-jumper-4-pin.html)

## Setup

- Copy `config/options.h.example` to `config/options.h` and fill with appropriate values.
- Build using CMake.
- Flash the firmware using `flash.sh` 