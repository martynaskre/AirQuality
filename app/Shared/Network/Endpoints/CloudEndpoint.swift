//
//  PairingEndpoint.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

public enum CloudEndpoint: Endpoint {
    case devices
    case measurements
    
    func path() -> String {
        switch self {
        case .devices:
            return "devices"
        case .measurements:
            return "measurements"
        }
    }
}
