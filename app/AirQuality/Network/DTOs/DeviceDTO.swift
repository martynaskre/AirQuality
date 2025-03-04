//
//  DeviceDTO.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 04/03/2025.
//

import Foundation


struct DeviceDTO: Decodable {
    let status: DeviceStatus
    let latestMeasurementAt: Date
}
