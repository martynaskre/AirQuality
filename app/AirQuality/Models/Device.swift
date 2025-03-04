//
//  Device.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 09/02/2025.
//

import Foundation
import SwiftData

@Model
final class Device {
    var id = UUID()
    var name: String
    var accessoryId: UUID
    var serialNumber: UUID
    
    var localUrl: String {
        return "airquality.local"
    }
    
    var webSocketUrl: URL {
        return URL(string: "ws://\(self.localUrl)/ws")!
    }
    
    init(name: String, accessoryId: UUID, serialNumber: UUID) {
        self.name = name
        self.accessoryId = accessoryId
        self.serialNumber = serialNumber
    }
    
    func getCloudService() -> CloudNetworkService {
        return .init(serialNumber: self.serialNumber)
    }
}
