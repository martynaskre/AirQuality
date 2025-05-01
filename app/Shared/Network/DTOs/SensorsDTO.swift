//
//  SensorsDTO.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation

public struct SensorsDTO: Decodable {
    public enum CodingKeys: CodingKey {
        case lux, temperature, pressure, humidity, eco2, tvoc, collectedAt
    }
    
    public let score: Int
    public let lux: Measurement<UnitIlluminance>
    public let temperature: Measurement<UnitTemperature>
    public let pressure: Measurement<UnitPressure>
    public let humidity: Double
    public let eCO2: Double
    public let TVOC: Double
    public let collectedAt: Date
    
    public init(from decoder: any Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        
        self.lux = Measurement(value: try container.decode(Double.self, forKey: .lux), unit: .lux)
        self.temperature = Measurement(value: try container.decode(Double.self, forKey: .temperature), unit: .celsius)
        self.pressure = Measurement(value: try container.decode(Double.self, forKey: .pressure), unit: .newtonsPerMetersSquared)
        self.humidity = try container.decode(Double.self, forKey: .humidity)
        self.eCO2 = try container.decode(Double.self, forKey: .eco2)
        self.TVOC = try container.decode(Double.self, forKey: .tvoc)
        
        self.score = AirQualityPredictor.shared.predict(
            lux: self.lux.value,
            temperature: self.temperature.value,
            pressure: self.pressure.value,
            humidity: self.humidity,
            eCO2: self.eCO2,
            TVOC: self.TVOC
        )
        
        do {
            self.collectedAt = try container.decode(Date.self, forKey: .collectedAt)
        } catch {
            self.collectedAt = Date()
        }
    }
}
