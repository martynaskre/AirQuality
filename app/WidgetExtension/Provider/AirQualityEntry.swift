//
//  AirQualityEntry.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import WidgetKit

struct AirQualityEntry: TimelineEntry {
    let score: Int
    let temperature: Measurement<UnitTemperature>
    let humidity: Double
    let pressure: Measurement<UnitPressure>
    let eCO2: Double
    let date: Date
}
