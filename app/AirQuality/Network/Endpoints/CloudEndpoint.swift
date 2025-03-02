//
//  PairingEndpoint.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

enum CloudEndpoint: Endpoint {
    case measurements
    
    func path() -> String {
        switch self {
        case .measurements:
            return "measurements"
        }
    }
}
