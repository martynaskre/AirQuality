//
//  DeviceEntity.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import AppIntents
import Shared

struct DeviceEntity: AppEntity {
    static var typeDisplayRepresentation: TypeDisplayRepresentation {
        TypeDisplayRepresentation(name: "Device")
    }
    
    static var defaultQuery = DeviceEntityQuery()
        
    var id: UUID
    
    @Property(title: "Device Name")
    var name: String
    
    var displayRepresentation: DisplayRepresentation {
        return DisplayRepresentation(title: "\(name)", subtitle: "AirQuality device")
    }
    
    init(device: Device) {
        self.id = device.id
        self.name = device.name
    }
}
