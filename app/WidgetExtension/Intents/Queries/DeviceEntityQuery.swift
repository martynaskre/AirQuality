//
//  DeviceEntityQuery.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import AppIntents
import SwiftData
import Shared
import OSLog

struct DeviceEntityQuery: EntityQuery {
    @Dependency
    private var deviceManager: DeviceManager
    
    func entities(for identifiers: [UUID]) async throws -> [DeviceEntity] {
        Logger.entityQueryLogging.debug("[DeviceEntityQuery] Query for IDs \(identifiers)")
        
        return await deviceManager.get(for: identifiers).map {
            DeviceEntity(device: $0)
        }
    }
    
    func suggestedEntities() async throws -> [DeviceEntity] {
        Logger.entityQueryLogging.debug("[DeviceEntityQuery] Request for suggested entities")
        
        return await deviceManager.get().map {
            DeviceEntity(device: $0)
        }
    }
}

extension DeviceEntityQuery: EntityStringQuery {
    func entities(matching string: String) async throws -> [DeviceEntity] {
        Logger.entityQueryLogging.debug("[DeviceEntityQuery] String query for term \(string)")

        return await deviceManager.get(matching: string).map {
            DeviceEntity(device: $0)
        }
    }
}
