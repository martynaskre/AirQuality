//
// Created by Martynas Skrebė on 07/02/2025.
//

#ifndef AIRQUALITY_BLUETOOTHPAIRINGTASK_H
#define AIRQUALITY_BLUETOOTHPAIRINGTASK_H


#include <optional>
#include "Tasks/FreeRTOSTask.h"
#include "Network/Bluetooth.h"

class BluetoothPairingTask: public FreeRTOSTask {
    struct __attribute__((packed)) NetworkCredentials {
        char ssid[33];
        char password[64];
    };

    std::optional<NetworkCredentials> networkCredentials;
    std::optional<hci_con_handle_t> respondTo;

    static void credentialsHandler(hci_con_handle_t connectionHandle, uint8_t *buffer, uint16_t bufferSize, void *data);
protected:
    void execute() override;
public:
    explicit BluetoothPairingTask();
};


#endif //AIRQUALITY_BLUETOOTHPAIRINGTASK_H
