//
// Created by Martynas Skrebė on 01/12/2024.
//

#include "Flash.h"
#include "options.h"
#include <cstring>
#include <cstdio>
#include "hardware/sync.h"
#include "hardware/flash.h"

void Flash::rangeWrite(void *parameter) {
    auto* wrappedData = static_cast<FlashDataWrapper*>(parameter);

    auto* dataAsBytes = (uint8_t*) wrappedData;

    int writeSize = (sizeof(FlashDataWrapper) / FLASH_PAGE_SIZE) + 1;
    unsigned int sectorCount = ((writeSize * FLASH_PAGE_SIZE) / FLASH_SECTOR_SIZE) + 1;

    if (get_core_num() == 0) {
        uint32_t interrupts = save_and_disable_interrupts();

        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE * sectorCount);
        flash_range_program(FLASH_TARGET_OFFSET, dataAsBytes, FLASH_PAGE_SIZE * writeSize);

        restore_interrupts(interrupts);
    } else {
        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE * sectorCount);
        flash_range_program(FLASH_TARGET_OFFSET, dataAsBytes, FLASH_PAGE_SIZE * writeSize);
    }
}

void Flash::rangeErase(void *parameter) {
    int writeSize = (sizeof(FlashDataWrapper) / FLASH_PAGE_SIZE) + 1;
    unsigned int sectorCount = ((writeSize * FLASH_PAGE_SIZE) / FLASH_SECTOR_SIZE) + 1;

    if (get_core_num() == 0) {
        uint32_t interrupts = save_and_disable_interrupts();

        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE * sectorCount);

        restore_interrupts(interrupts);
    } else {
        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE * sectorCount);
    }
}

std::optional<FlashData> Flash::get() {
    FlashDataWrapper wrappedData{};

    const auto *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
    memcpy(&wrappedData, flash_target_contents, sizeof(wrappedData));

    if (wrappedData.validation == FLASH_VALIDATION) {
        return wrappedData.data;
    }

    return {};
}

void Flash::set(FlashData data) {
    FlashDataWrapper wrappedData = { FLASH_VALIDATION, data };

    auto rc = flash_safe_execute(Flash::rangeWrite, &wrappedData, UINT32_MAX);
    hard_assert(rc == PICO_OK);
}

void Flash::clear() {
    if (Flash::get().has_value()) {
        auto rc = flash_safe_execute(Flash::rangeErase, nullptr, UINT32_MAX);
        hard_assert(rc == PICO_OK);
    }
}
