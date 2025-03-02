//
//  DeviceManager.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 30/01/2025.
//

import Foundation
import AccessorySetupKit
import CoreBluetooth
import SwiftUI

@Observable
class AccessoryManager: NSObject {
    private let session = ASAccessorySession()
    private var accessory: ASAccessory?
    
    private(set) var activated: Bool = false
    private(set) var pairingSession: PairingSession? = nil
    
    var pairedAccessories: [ASAccessory] {
        return self.session.accessories
    }
    
    override init() {
        super.init()
        
        self.session.activate(on: DispatchQueue.main, eventHandler: handleSessionEvent(event:))
    }
    
    func presentPicker() {
        session.showPicker(for: [Self.stationDevice]) { error in
            if let error {
                print("Failed to show picker due to: \(error.localizedDescription)")
            }
        }
    }
    
    func removeAccessory(_ accessory: ASAccessory) {
        self.session.removeAccessory(accessory) { error in
            if let error {
                print("Failed to remove accessory: \(error.localizedDescription)")
            }
        }
    }
    
    func terminatePairingSession(_ pairingSession: PairingSession) {
        self.removeAccessory(pairingSession.accessory)
    }
    
    func reset() {
        self.accessory = nil
        self.pairingSession = nil
    }
    
    private func handleSessionEvent(event: ASAccessoryEvent) {
        print("AccessoryManager event \(event.eventType.rawValue)")
        switch event.eventType {
        case .activated:
            self.activated = true
        case .accessoryAdded:
            self.accessory = event.accessory
        case .pickerDidDismiss:
            if let accessory = self.accessory {
                self.pairingSession = PairingSession(accessory)
            } else {
                self.reset()
            }
        case .invalidated:
            self.activated = false
        case .pickerSetupFailed:
            self.activated = false
        default:
            break
        }
    }
}

/// MARK: Available Devices
extension AccessoryManager {
    private static let stationDevice: ASPickerDisplayItem = {
        let descriptor = ASDiscoveryDescriptor()
        descriptor.bluetoothServiceUUID = CBUUID(string: "A6136564-90D6-4E01-8250-9FE5E9F38442") // TODO: extract this
                
        return ASPickerDisplayItem(
            name: "Station",
            productImage: UIImage(systemName: "air.purifier.fill")!,
            descriptor: descriptor
        )
    }()
}
