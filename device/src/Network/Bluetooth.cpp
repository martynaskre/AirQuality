//
// Created by Martynas Skrebė on 06/02/2025.
//

#include "Bluetooth.h"
#include "btstack.h"
#include "Utils/Utils.h"

const uint8_t Bluetooth::advertisementData[] = {
    2, BLUETOOTH_DATA_TYPE_FLAGS, 0x06,
    8, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'S', 't', 'a','t', 'i', 'o', 'n',
    17, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS, 0x42, 0x84, 0xF3, 0xE9, 0xE5, 0x9F, 0x50, 0x82, 0x01, 0x4E, 0xD6, 0x90, 0x64, 0x65, 0x13, 0xA6
};

BluetoothStatus Bluetooth::status = Off;

Bluetooth::CredentialsHandler Bluetooth::credentialsHandler = nullptr;
void* Bluetooth::credentialsHandlerData = nullptr;

void Bluetooth::attPacketHandler(uint8_t packetType, uint16_t channel, uint8_t *packet, uint16_t size) {
    if (packetType != HCI_EVENT_PACKET) {
        return;
    }

    switch (hci_event_packet_get_type(packet)) {
        case ATT_EVENT_CONNECTED:
            debug("Bluetooth client connected");

            break;
        case ATT_EVENT_DISCONNECTED:
            debug("Bluetooth client disconnected");

            break;
        default:
            break;
    }
}

uint16_t Bluetooth::readCallback(hci_con_handle_t connectionHandle, uint16_t attributeHandle, uint16_t offset, uint8_t *buffer, uint16_t bufferSize) {
    if (attributeHandle == ATT_CHARACTERISTIC_0x2A01_01_USER_DESCRIPTION_HANDLE) {
        char description[] = "Network Credentials";

        return att_read_callback_handle_blob(reinterpret_cast<const uint8_t *>(description), strlen(description), offset, buffer, bufferSize);
    } else if (attributeHandle == ATT_CHARACTERISTIC_0x2A02_01_USER_DESCRIPTION_HANDLE) {
        char description[] = "Connection Status";

        return att_read_callback_handle_blob(reinterpret_cast<const uint8_t *>(description), strlen(description), offset, buffer, bufferSize);
    }

    return 0;
}

int Bluetooth::writeCallback(hci_con_handle_t connectionHandle, uint16_t attributeHandle, uint16_t transactionMode, uint16_t offset, uint8_t *buffer, uint16_t bufferSize) {
    if (attributeHandle == ATT_CHARACTERISTIC_0x2A01_01_VALUE_HANDLE) {
        if (credentialsHandler != nullptr) {
            credentialsHandler(connectionHandle, buffer, bufferSize, Bluetooth::credentialsHandlerData);
        }
    }

    return 0;
}

err_t Bluetooth::start() {
    if (Bluetooth::status == On) {
        return ERR_ALREADY;
    } else if (!initialiseNetworkDriver()) {
        return ERR_CONN;
    }

    l2cap_init();
    sm_init();

    att_server_init(profile_data, Bluetooth::readCallback, Bluetooth::writeCallback);
    att_server_register_packet_handler(Bluetooth::attPacketHandler);

    gap_advertisements_set_params(0x0030, 0x0030, 0, 0, nullptr, 0x07, 0x00);
    gap_advertisements_set_data(sizeof(advertisementData), const_cast<uint8_t *>(advertisementData));
    gap_advertisements_enable(true);

    if (hci_power_control(HCI_POWER_ON) != 0) {
        printf("unable to turn on bt \n");

        gap_advertisements_enable(false);
        att_server_deinit();
        sm_deinit();
        l2cap_deinit();

        return ERR_CONN;
    }

    Bluetooth::status = On;

    return ERR_OK;
}

err_t Bluetooth::stop() {
    if (Bluetooth::status == Off) {
        return ERR_CONN;
    }

    hci_power_control(HCI_POWER_OFF);

    gap_advertisements_enable(false);

    att_server_deinit();

    Bluetooth::credentialsHandler = nullptr;
    Bluetooth::credentialsHandlerData = nullptr;

    sm_deinit();
    l2cap_deinit();

    Bluetooth::status = Off;

    return ERR_OK;
}

void Bluetooth::setCredentialsHandler(Bluetooth::CredentialsHandler handler, void *data) {
    Bluetooth::credentialsHandler = handler;
    Bluetooth::credentialsHandlerData = data;
}
