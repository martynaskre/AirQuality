//
//  Device.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 09/02/2025.
//

import Foundation
import SwiftData

@Model
public final class Device {
    public var id = UUID()
    public var name: String
    public var accessoryId: UUID
    public var serialNumber: UUID
    
    public var localUrl: String {
        return "airquality.local"
    }
    
    public var webSocketUrl: URL {
        return URL(string: "ws://\(self.localUrl)/ws")!
    }
    
    public init(name: String, accessoryId: UUID, serialNumber: UUID) {
        self.name = name
        self.accessoryId = accessoryId
        self.serialNumber = serialNumber
    }
    
    public func getCloudService() -> CloudNetworkService {
        return .init(serialNumber: self.serialNumber)
    }
}
