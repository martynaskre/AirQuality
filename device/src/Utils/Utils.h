//
// Created by Martynas Skrebė on 23/01/2025.
//

#ifndef AIRQUALITY_UTILS_H
#define AIRQUALITY_UTILS_H


#include <cstdarg>
#include "pico/cyw43_arch.h"

enum LogLevel {
    Debug,
    Warning,
    Error
};

void log(LogLevel level, const char* format, va_list args);
void debug(const char* format, ...);
void warning(const char* format, ...);
void error(const char* format, ...);

bool initialiseNetworkDriver();
bool isNetworkInitialised();
void deinitialiseNetworkDriver();

uint32_t getTotalHeap();
uint32_t getFreeHeap();

#endif //AIRQUALITY_UTILS_H
