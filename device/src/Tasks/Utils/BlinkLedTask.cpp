//
// Created by Martynas Skrebė on 29/01/2025.
//

#include "BlinkLedTask.h"
#include "pico/stdlib.h"
#include "options.h"

BlinkLedTask::BlinkLedTask(uint64_t delay): FreeRTOSTask("BlinkLedTask", 1024, 5), delay(delay) {}

BlinkLedTask::~BlinkLedTask() {
    gpio_put(LED_PIN, false);
}

void BlinkLedTask::execute() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, !gpio_get(LED_PIN));

        vTaskDelay(pdMS_TO_TICKS(delay));
    }
}
