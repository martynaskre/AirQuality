//
//  DeviceManager.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import Foundation
import SwiftData
import OSLog

@MainActor
public class DeviceManager: @unchecked Sendable {
    public static let shared = DeviceManager()
    
    public var container: ModelContainer {
        return DataController.shared.container
    }
    
    public func get() -> [Device] {
        do {
            let descriptor = FetchDescriptor<Device>(sortBy: [SortDescriptor(\.name)])
                        
            return try container.mainContext.fetch(descriptor)
        } catch {
            Logger.dataManagerLogging.error("[DeviceManager] An error occurred while fetching devices: \(error)")
            
            return []
        }
    }

    public func get(_ identifier: UUID) -> Device? {
        do {
            var descriptor = FetchDescriptor<Device>(predicate: #Predicate { device in
                return device.id == identifier
            })
            descriptor.fetchLimit = 1
            
            let devices = try container.mainContext.fetch(descriptor)
            
            return devices.first
        } catch {
            Logger.dataManagerLogging.error("[DeviceManager] An error occurred while fetching device by identifier \(identifier): \(error)")
            
            return nil
        }
    }

    public func get(for identifiers: [UUID]) -> [Device] {
        do {
            let descriptor = FetchDescriptor<Device>(predicate: #Predicate { device in
                return identifiers.contains(device.id)
            })
            
            return try container.mainContext.fetch(descriptor)
        } catch {
            Logger.dataManagerLogging.error("[DeviceManager] An error occurred while fetching devices by identifiers \(identifiers.map { $0.uuidString }.joined(separator: ", ")): \(error)")
            
            return []
        }
    }

    public func get(matching string: String) -> [Device] {
        do {
            let descriptor = FetchDescriptor<Device>(predicate: #Predicate { device in
                if string.isEmpty {
                    return true
                } else {
                    return device.name.localizedStandardContains(string)
                }
            })
            
            return try container.mainContext.fetch(descriptor)
        } catch {
            Logger.dataManagerLogging.error("[DeviceManager] An error occurred while searching devices by \(string): \(error)")
            
            return []
        }
    }

    public func create(_ device: Device) {
        container.mainContext.insert(device)
    }
}
