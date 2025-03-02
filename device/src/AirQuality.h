//
// Created by Martynas Skrebė on 30/11/2024.
//

#ifndef AIRQUALITY_AIRQUALITY_H
#define AIRQUALITY_AIRQUALITY_H


#undef bind
#undef read

#include <memory>
#include <functional>

#include "drivers/ltr559/ltr559.hpp"
#include "drivers/bme280/bme280.hpp"
#include "drivers/sgp30/sgp30.hpp"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct AirQualityMeasurement {
    uint16_t proximity;
    float lux;
    float temperature;
    float pressure;
    float humidity;
    uint16_t eCO2;
    uint16_t TVOC;
    uint16_t rawH2;
    uint16_t rawEthanol;
    bool calibrated;
};

class AirQuality {
    static SemaphoreHandle_t dataMutex;
    static AirQualityMeasurement measurement;

    static std::unique_ptr<pimoroni::I2C> i2cBus;

    static std::unique_ptr<pimoroni::LTR559> ltr559;
    static std::unique_ptr<pimoroni::BME280> bme280;
    static std::unique_ptr<pimoroni::SGP30> sgp30;

    static float calculateSaturationVaporPressure(float temperature);
    static uint32_t calculateAbsoluteHumidity(float temperature, float humidity);

    static void changeMeasurement(const std::function<void()>& function);
public:
    static void init();
    static void calibrate();
    static void update();

    static AirQualityMeasurement& getMeasurement();
};


#endif //AIRQUALITY_AIRQUALITY_H
