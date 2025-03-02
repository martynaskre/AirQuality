//
// Created by Martynas Skrebė on 01/12/2024.
//

#ifndef AIRQUALITY_FLASH_H
#define AIRQUALITY_FLASH_H


#include <optional>
#include "pico/flash.h"

#define FLASH_VALIDATION 0xDEADBEEF

struct FlashData {
    char ssid[33];
    char password[64];
};

class Flash {
    struct FlashDataWrapper {
        uint32_t validation;
        FlashData data;
    };

    static void rangeWrite(void* parameter);
    static void rangeErase(void* parameter);
public:
    static std::optional<FlashData> get();
    static void set(FlashData data);
    static void clear();
};


#endif //AIRQUALITY_FLASH_H
