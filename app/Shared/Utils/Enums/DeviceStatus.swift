//
//  DeviceStatus.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 04/03/2025.
//

import SwiftUI

public enum DeviceStatus: String, Decodable {
    case Operational = "operational"
    case Delayed = "delayed"
    case Offline = "offline"
    
    public var image: Image {
        switch self {
        case .Operational:
            return .init(systemName: "dot.radiowaves.left.and.right")
        case .Delayed:
            return .init(systemName: "hourglass")
        case .Offline:
            return .init(systemName: "antenna.radiowaves.left.and.right.slash")
        }
    }
    
    public var color: Color {
        switch self {
        case .Operational:
            return .green
        case .Delayed:
            return .orange
        case .Offline:
            return .red
        }
    }
}
