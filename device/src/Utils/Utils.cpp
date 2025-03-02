//
// Created by Martynas Skrebė on 23/01/2025.
//

#include "Utils.h"
#include <cstdio>
#include <malloc.h>

void logMessage(LogLevel level, const char* format, va_list args) {
    const char* levelStr = "";

    switch (level) {
        case Debug:
            levelStr = "DEBUG";
            break;
        case Warning:
            levelStr = "WARN";
            break;
        case Error:
            levelStr = "ERROR";
            break;
    }

    printf("[%s] ", levelStr);
    vprintf(format, args);
    printf("\n");
}

void debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    logMessage(Debug, format, args);
    va_end(args);
}

void warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    logMessage(Warning, format, args);
    va_end(args);
}

void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    logMessage(Error, format, args);
    va_end(args);
}

bool initialiseNetworkDriver() {
    if (!isNetworkInitialised()) {
        if (cyw43_arch_init_with_country(CYW43_COUNTRY_LITHUANIA)) {
            return false;
        }
    }

    return true;
}

bool isNetworkInitialised() {
    return cyw43_is_initialized(&cyw43_state);
}

void deinitialiseNetworkDriver() {
    cyw43_arch_deinit();
}

uint32_t getTotalHeap() {
    extern char __StackLimit, __bss_end__;

    return &__StackLimit  - &__bss_end__;
}

uint32_t getFreeHeap() {
    struct mallinfo m = mallinfo();

    return getTotalHeap() - m.uordblks;
}
