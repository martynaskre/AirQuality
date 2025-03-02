#include "pico/stdlib.h"
#include "AirQuality.h"
#include "Application.h"

int main() {
    stdio_init_all();

    AirQuality::init();
    Application::start();

    return 0;
}
