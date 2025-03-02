//
//  PairedAccessory.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 08/02/2025.
//

import Foundation
import AccessorySetupKit
import CoreBluetooth
import SwiftUI

@Observable
class PairingSession: NSObject {
    enum Status {
        case waiting
        case error
        case connected
        case paired
    }
    
    private(set) var accessory: ASAccessory
    private(set) var serialNumber: UUID? = nil
    private var manager: CBCentralManager
    private var peripheral: CBPeripheral? = nil
    
    private var networkCredentialsCharacteristic: CBCharacteristic?
    
    private(set) var status: Status = .waiting
    private(set) var connectionInProgress: Bool = false
    private(set) var connectionSuccessful: Bool? = nil
    
    init(_ accessory: ASAccessory) {
        self.accessory = accessory
        self.manager = CBCentralManager(delegate: nil, queue: nil)

        super.init()

        self.manager.delegate = self
    }

    func disconnect() {
        guard let peripheral = self.peripheral else { return }
        
        manager.cancelPeripheralConnection(peripheral)
    }
    
    func setNetworkCredentials(ssid: String, password: String) {
        guard let peripheral = self.peripheral, let credentialsCharacteristic = self.networkCredentialsCharacteristic else {
            return
        }

        self.connectionSuccessful = nil
        self.connectionInProgress = true
        
        let ssidData = ssid.padNull(toLength: 33)
        let passwordData = password.padNull(toLength: 64)

        peripheral.writeValue(ssidData + passwordData, for: credentialsCharacteristic, type: .withoutResponse)
    }
}

/// MARK: Bluetooth GATT Characteristics
extension PairingSession {
    private static let networkCredentialsCharacteristicUUID: CBUUID = {
        return CBUUID(string: "0x2A01")
    }()
    
    private static let networkStatusCharacteristicUUID: CBUUID = {
        return CBUUID(string: "0x2A02")
    }()
}

/// MARK: - CBCentralManagerDelegate
extension PairingSession: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        switch central.state {
        case .poweredOn:
            if let peripheralUUID = accessory.bluetoothIdentifier {
                peripheral = central.retrievePeripherals(withIdentifiers: [peripheralUUID]).first
                peripheral?.delegate = self
                
                if let peripheral = self.peripheral {
                    manager.connect(peripheral)
                }
            }
        default:
            peripheral = nil
        }
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        peripheral.delegate = self
        peripheral.discoverServices([CBUUID(string: "A6136564-90D6-4E01-8250-9FE5E9F38442")]) // TODO: extract this
        
        self.status = .connected
    }

    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: (any Error)?) {
        if self.status != .paired {
            self.status = .error
        }
    }

    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: (any Error)?) {
        if self.status != .paired {
            self.status = .error
        }
    }
}

/// MARK: - CBPeripheralDelegate
extension PairingSession: CBPeripheralDelegate {
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: (any Error)?) {
        guard error == nil, let services = peripheral.services else {
            return
        }

        for service in services {
            peripheral.discoverCharacteristics([
                Self.networkCredentialsCharacteristicUUID,
                Self.networkStatusCharacteristicUUID
            ], for: service)
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: (any Error)?) {
        guard error == nil, let characteristics = service.characteristics else {
            return
        }

        for characteristic in characteristics {
            print(characteristic)
            if characteristic.uuid == Self.networkStatusCharacteristicUUID {
                peripheral.setNotifyValue(true, for: characteristic)
                peripheral.readValue(for: characteristic)
            } else if characteristic.uuid == Self.networkCredentialsCharacteristicUUID {
                self.networkCredentialsCharacteristic = characteristic
            }
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: (any Error)?) {
        guard error == nil, characteristic.uuid == Self.networkStatusCharacteristicUUID, let data = characteristic.value else {
            return
        }
        
        DispatchQueue.main.async {
            if data.count == 36, let dataAsString = String(data: data, encoding: .utf8), let uuid = UUID(uuidString: dataAsString) {
                self.serialNumber = uuid
                self.connectionSuccessful = true
            } else {
                self.connectionSuccessful = false
            }
            
            if self.connectionSuccessful == true {
                self.status = .paired
            }
            
            self.connectionInProgress = false
        }
    }
}
