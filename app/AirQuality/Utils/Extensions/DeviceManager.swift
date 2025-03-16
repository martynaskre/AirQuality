//
//  DeviceManager.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import Shared
import OSLog
import Foundation

extension DeviceManager {
    func delete(for identifiers: [UUID], accessoryManager: AccessoryManager? = nil) {
        do {
            if let accessoryManager = accessoryManager {
                let devices = get(for: identifiers)
                
                for device in devices {
                    if let accessory = accessoryManager.pairedAccessories.first(where: { $0.bluetoothIdentifier == device.accessoryId }) {
                        accessoryManager.removeAccessory(accessory)
                    }
                }
            }
            
            try container.mainContext.delete(model: Device.self, where: #Predicate { device in
                return identifiers.contains(device.id)
            })
        } catch {
            Logger.dataManagerLogging.error("[DeviceManager] An error occurred while deleting devices by ID \(identifiers.map { $0.uuidString }.joined(separator: ",")): \(error)")
        }
    }
}

