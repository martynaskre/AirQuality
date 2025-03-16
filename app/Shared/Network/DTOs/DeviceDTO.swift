//
//  DeviceDTO.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 04/03/2025.
//

import Foundation

public struct DeviceDTO: Decodable {
    public let status: DeviceStatus
    public let latestMeasurementAt: Date
}
