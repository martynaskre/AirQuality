cmake_minimum_required(VERSION 3.21)

set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/lib/pico-sdk)
set(FREERTOS_KERNEL_PATH ${CMAKE_SOURCE_DIR}/lib/FreeRTOS-Kernel)
set(MONGOOSE_PATH ${CMAKE_SOURCE_DIR}/lib/mongoose)
set(PIMORONI_PICO_PATH ${CMAKE_SOURCE_DIR}/lib/pimoroni-pico)
set(SRC_PATH ${CMAKE_SOURCE_DIR}/src)
set(GATT_FILE_PATH ${CMAKE_SOURCE_DIR}/config/bluetooth_gatt_config.gatt)

set(PICO_BOARD pico_w)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
include(${PIMORONI_PICO_PATH}/pimoroni_pico_import.cmake)

project(AirQuality C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(${SRC_PATH})

add_executable(AirQuality
    main.cpp
    ${MONGOOSE_PATH}/mongoose.c
    ${MONGOOSE_PATH}/mongoose.h
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${CA_CERT_HEADER}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/config
    PRIVATE ${SRC_PATH}
    PRIVATE ${MONGOOSE_PATH}
    PRIVATE "${CMAKE_BINARY_DIR}/files"
)

include(common/pimoroni_i2c)
include(${PIMORONI_PICO_PATH}/drivers/ltr559/ltr559.cmake)
include(${PIMORONI_PICO_PATH}/drivers/bme280/bme280.cmake)
include(${PIMORONI_PICO_PATH}/drivers/sgp30/sgp30.cmake)

target_link_libraries(${PROJECT_NAME}
    pico_cyw43_arch_lwip_sys_freertos
    pico_stdlib
    pico_lwip_iperf
    pico_lwip_mdns
    pico_btstack_ble
    pico_btstack_cyw43
    FreeRTOS-Kernel-Heap4
    pimoroni_i2c
    ltr559
    bme280
    sgp30
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_btstack_make_gatt_header(${PROJECT_NAME} PRIVATE ${GATT_FILE_PATH})
pico_add_extra_outputs(${PROJECT_NAME})
