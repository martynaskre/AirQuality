//
// Created by Martynas Skrebė on 30/11/2024.
//

#include "AirQuality.h"
#include "options.h"
#include <cmath>

SemaphoreHandle_t AirQuality::dataMutex = nullptr;
AirQualityMeasurement AirQuality::measurement = {0};
std::unique_ptr<pimoroni::I2C> AirQuality::i2cBus = nullptr;
std::unique_ptr<pimoroni::LTR559> AirQuality::ltr559 = nullptr;
std::unique_ptr<pimoroni::BME280> AirQuality::bme280 = nullptr;
std::unique_ptr<pimoroni::SGP30> AirQuality::sgp30 = nullptr;

float AirQuality::calculateSaturationVaporPressure(float temperature) {
    return 6.112f * std::exp((17.67f * temperature) / (temperature + 243.5f));
}

uint32_t AirQuality::calculateAbsoluteHumidity(float temperature, float humidity) {
    const float R = 461.5f; // Gas constant for water vapor (J/(kg·K))

    float temperatureKelvin = temperature + 273.15f;
    float saturationVaporPressure = calculateSaturationVaporPressure(temperature); // saturation vapor pressure in hPa
    float humidityDecimal = humidity / 100.0f;
    float absoluteHumidity = (humidityDecimal * saturationVaporPressure * 100) / (R * temperatureKelvin);

    return static_cast<uint32_t>(absoluteHumidity * 1000.0f);
}

void AirQuality::changeMeasurement(const std::function<void()> &function) {
    if (dataMutex == nullptr) {
        dataMutex = xSemaphoreCreateMutex();
    }

    if (dataMutex != nullptr) {
        xSemaphoreTake(dataMutex, pdMS_TO_TICKS(500));

        function();

        xSemaphoreGive(dataMutex);
    }
}

void AirQuality::init() {
    i2cBus = std::make_unique<pimoroni::I2C>(I2C_SDA_PIN, I2C_SCL_PIN);

    ltr559 = std::make_unique<pimoroni::LTR559>(i2cBus.get());
    bme280 = std::make_unique<pimoroni::BME280>(i2cBus.get(), 0x77);
    sgp30 = std::make_unique<pimoroni::SGP30>(i2cBus.get());

    ltr559->init();
    bme280->init();
    sgp30->init();
}

void AirQuality::calibrate() {
    changeMeasurement([]() {
        measurement.calibrated = false;
    });

    sgp30->soft_reset();

    vTaskDelay(pdMS_TO_TICKS(500));

    auto bme280Reading = bme280->read_forced();

    sgp30->set_humidity(calculateAbsoluteHumidity(bme280Reading.temperature, bme280Reading.humidity));

    vTaskDelay(pdMS_TO_TICKS(100));

    sgp30->start_measurement(false);

    vTaskDelay(pdMS_TO_TICKS(1000 * 20));

    changeMeasurement([]() {
        measurement.calibrated = true;
    });
}

void AirQuality::update() {
    changeMeasurement([]() {
        if (ltr559->get_reading()) {
            measurement.lux = ltr559->data.lux;
            measurement.proximity = ltr559->data.proximity;
        }

        auto bme280Reading = bme280->read_forced();

        measurement.temperature = bme280Reading.temperature;
        measurement.humidity = bme280Reading.humidity;
        measurement.pressure = bme280Reading.pressure;

        if (measurement.calibrated) {
            sgp30->get_air_quality(&measurement.eCO2, &measurement.TVOC);
            sgp30->get_air_quality_raw(&measurement.rawH2, &measurement.rawEthanol);
        }
    });
}

AirQualityMeasurement &AirQuality::getMeasurement() {
    return measurement;
}
