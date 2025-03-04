#ifndef OPTIONS_H
#define OPTIONS_H

constexpr const char* SERIAL_NUMBER = "646974F7-0EBC-4207-8403-9E551B1FE601";
constexpr const char* PUSH_KEY = "47402C22-65EE-434A-850F-561A8E6AC410";

constexpr const char* SERVER_MEASUREMENTS_ENDPOINT = "http://airquality.skrebe.me/api/measurements";

#define LED_PIN 17
#define BUTTON_PIN 14
#define I2C_SDA_PIN 18
#define I2C_SCL_PIN 19

#define LONG_PRESS_FOR (1000 * 3) // in seconds
#define RUN_PAIRING_FOR (1000 * 60 * 5) // in seconds

#define SGP30_RECALIBRATE_EVERY (1000 * 60 * 10) // in seconds
#define UPDATE_SENORS_EVERY (1000 * 1) // in seconds
#define LOG_DATA_EVERY (1000 * 60) // in seconds

#define FLASH_TARGET_OFFSET (1536 * 1024)

#endif
