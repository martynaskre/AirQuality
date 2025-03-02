//
// Created by Martynas Skrebė on 06/02/2025.
//

#ifndef AIRQUALITY_BLUETOOTH_H
#define AIRQUALITY_BLUETOOTH_H


#include <vector>
#include <bluetooth.h>
#include <ble/att_db.h>
#include "pico/cyw43_arch.h"
#include "bluetooth_gatt_config.h"

#define NETWORK_STATUS_HANDLE ATT_CHARACTERISTIC_0x2A02_01_VALUE_HANDLE

enum BluetoothStatus {
    On,
    Off
};

class Bluetooth {
    typedef void (*CredentialsHandler)(hci_con_handle_t connectionHandle, uint8_t *buffer, uint16_t bufferSize, void *data);

    const static uint8_t advertisementData[];
    static BluetoothStatus status;

    static CredentialsHandler credentialsHandler;
    static void* credentialsHandlerData;

    static void attPacketHandler(uint8_t packetType, uint16_t channel, uint8_t *packet, uint16_t size);

    static uint16_t readCallback(hci_con_handle_t connectionHandle, uint16_t attributeHandle, uint16_t offset, uint8_t * buffer, uint16_t bufferSize);
    static int writeCallback(hci_con_handle_t connectionHandle, uint16_t attributeHandle, uint16_t transactionMode, uint16_t offset, uint8_t *buffer, uint16_t bufferSize);
public:
    static err_t start();
    static err_t stop();

    static void setCredentialsHandler(CredentialsHandler handler, void *data = nullptr);
};


#endif //AIRQUALITY_BLUETOOTH_H
