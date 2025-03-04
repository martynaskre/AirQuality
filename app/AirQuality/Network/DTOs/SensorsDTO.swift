//
//  SensorsDTO.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation

struct SensorsDTO: Decodable {
    public enum CodingKeys: CodingKey {
        case lux, temperature, pressure, humidity, eco2, tvoc, collectedAt
    }
    
    let lux: Measurement<UnitIlluminance>
    let temperature: Measurement<UnitTemperature>
    let pressure: Measurement<UnitPressure>
    let humidity: Double
    let eCO2: Double
    let TVOC: Double
    let collectedAt: Date
    
    init(from decoder: any Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        
        self.lux = Measurement(value: try container.decode(Double.self, forKey: .lux), unit: .lux)
        self.temperature = Measurement(value: try container.decode(Double.self, forKey: .temperature), unit: .celsius)
        self.pressure = Measurement(value: try container.decode(Double.self, forKey: .pressure), unit: .hectopascals)
        self.humidity = try container.decode(Double.self, forKey: .humidity)
        self.eCO2 = try container.decode(Double.self, forKey: .eco2)
        self.TVOC = try container.decode(Double.self, forKey: .tvoc)
        self.collectedAt = try container.decode(Date.self, forKey: .collectedAt)
    }
}
